#include <list>
#include <math.h>
#include <unistd.h>
#include "path_tracker.h"
#include "strategie.h"
#include "cinematik.h"
#include "picAPI.h"
#include "webcamAPI.h"
#include <cstdio>

#define PC_INCLUDE
#include "../common/const.h"

using namespace std;

pthread_mutex_t mutex_situ;

bool pt_stopped;

void calc_path(const position_t &from, const position_t &to, int type, bool append = false);

//------------------------------------------------------------------------------

#ifdef SIMULATION
#define MAX_wheel_speed    0.25   // Vitesse maximale atteignable
#define MIN_wheel_speed    0.10   // Vitesse minimal atteignable
#else
#define MAX_wheel_speed    0.3    // Vitesse maximale atteignable
#define MIN_wheel_speed    0.25    // Vitesse minimal atteignable
#endif

#define DEST_RADIUS  0.01    // distance minimum (en mètres) pour valider le waypoint
#define DEST_ORIENT  0.003    // écart de direction minimum

#define WAYPOINT_RADIUS  0.1  // distance minimum (en mètres) pour valider le waypoint
#define WAYPOINT_ORIENT  1.5  // écart de direction minimum (en radian) pour valider le waypoint


//----------------------------------------------------------------------------//
//                              DIRECT_PATH_T                                 //
//----------------------------------------------------------------------------//
class direct_path_t
{
  private:
  float normalize(float angle);   // renvoie un angle compris entre -M_PI et M_PI

  
  bool is_dest_reached_xy(const position_t &pos);   // renvoie vrai si la destination est atteinte (à peu près) en x y
  bool is_dest_reached_a(const position_t &pos);    // renvoie vrai si la destination est atteinte (à peu près) en angle
  
  public:
  position_t dest;      // destination de l'étape
  int type;             // destination ou étape?
  
  pthread_mutex_t *mutex;

  direct_path_t(const position_t &dest, int type):dest(dest),type(type){mutex = NULL;} // constructeur de classe
  
  bool is_dest_reached(const position_t &pos);      // renvoie vrai si la destination est atteinte (à peu près)
  wheel_speed_t get_wheel_speed(const position_t &pos);     // renvoie la wheel_speed des moteurs pour atteindre le but
};

list<direct_path_t> path;
list<direct_path_t> resume;
//------------------------------------------------------------------------------
bool direct_path_t::is_dest_reached_xy(const position_t &pos)
{
  vector_t dist = vector_t(dest.x-pos.x, dest.y-pos.y);

  switch(type)
  {
    case tpDEST:
    case tpPREDEST:
    return (dist.norme()<DEST_RADIUS); 
    case tpWAYPOINT:
    return (dist.norme()<WAYPOINT_RADIUS);    
    case tpMOVE:
    {
      vector_t N(cos(dest.a), sin(dest.a)); 
      return dist.norme()<0.01 || (dist|N) <= 0.;    
    }    
    case tpTURN:
    return true;
    default:
    return true;
  }  
}
//------------------------------------------------------------------------------
bool direct_path_t::is_dest_reached_a(const position_t &pos)
{
  switch(type)
  {
    case tpDEST:
    return (fabsf(normalize(pos.a-dest.a))<DEST_ORIENT);   
    case tpPREDEST:
    return true;
    case tpWAYPOINT:
    return (fabsf(normalize(pos.a-dest.a))<WAYPOINT_ORIENT);   
    case tpMOVE:
    return true;
    case tpTURN:
    return (fabsf(normalize(pos.a-dest.a))< 3. * M_PI / 180.);   
    default:
    return true;
  }  
}
//------------------------------------------------------------------------------
bool direct_path_t::is_dest_reached(const position_t &pos)
{
  return is_dest_reached_xy(pos) && is_dest_reached_a(pos);
}
//------------------------------------------------------------------------------
wheel_speed_t direct_path_t::get_wheel_speed(const position_t &pos)
{ 
  vector_t N = vector_t(cos(pos.a), sin(pos.a));
  
  double max_pw = MAX_wheel_speed;
  double min_pw = MIN_wheel_speed;
  
  // on calcule l'angle vers la destination
  vector_t v = dest.v() - pos.v();
  
  if(type == tpMOVE)
  {
    double dist = v.norme();
    double da = normalize(pos.a-v.to_angle());   
    double d = 1. - fabsf(da) / (M_PI/3.5);
    double pw = (dist > 0.5) ? max_pw : max_pw * pow(dist / 0.5, 1);
    pw = pw < min_pw ? min_pw : pw;    
    d = d < 0.7 ? 0.7 : d;    
        
    if(da>=0)
      return wheel_speed_t(d*pw,pw);
    else
      return wheel_speed_t(pw,d*pw);    
  }
  
  if(type == tpTURN)
  {
    double da = normalize(pos.a-dest.a);
    double pw = (fabs(da) > 60. * M_PI / 180.) ? max_pw : max_pw * pow(fabs(da) / (60 * M_PI / 180.), 1.2);
    pw = pw < min_pw ? min_pw : pw;
    
    if(da>=0)            // on tourne sur soi-même
      return wheel_speed_t(-pw,pw);
    else
      return wheel_speed_t(pw,-pw);  
  }
  
  return wheel_speed_t(0,0);  
  /*
  if(is_dest_reached_xy(pos))    // on a déjà atteint la destination: on continue à tourner
  {    
    if(is_dest_reached_a(pos)) 
      return wheel_speed_t(0.,0.);      
    else
    {
      float da = normalize(pos.a-dest.a);
      float pw;
      if(fabsf(da)>M_PI/3.)
        pw = 0.3*max_pw; //fabsf(da)/(1.2 * M_PI) * max_pw;
      else
        pw = 0.2*max_pw; //(fabsf(da)<M_PI/18.?0.04:0.08) + fabsf(da) / M_PI * max_pw;
      pw = pw < min_pw ? min_pw : pw;   
                  
      if(da>=0)            // on tourne sur soi-même
        return wheel_speed_t(-pw,pw);
      else
        return wheel_speed_t(pw,-pw);  
    }     
  }   
  else
  {      
    double da = normalize(pos.a-v.to_angle());   
    if(fabs(da)>M_PI_2 && v.norme()<0.05)  // on recule si la cible est pas loin derrière
    {
      float pw = 0.2*max_pw;
      pw = pw < min_pw ? min_pw : pw;      
      return wheel_speed_t(-pw,-pw);
    }
      
    double dist;
    if(type != tpWAYPOINT)
      dist = v.norme();
    else
    {
      list<direct_path_t> :: iterator iter;
      for(iter = path.begin(); iter != path.end(); ++iter)
      {
        vector_t v = (*iter).dest.v() - pos.v();
        dist = v.norme();        
        if((*iter).type != tpWAYPOINT)
          break;
      }
    }      
    double pw = (dist > 0.5) ? max_pw : max_pw * pow(dist / 0.5, 0.5);
    pw = pw < min_pw ? min_pw : pw;
    
    if(fabsf(da)>M_PI/6.)   // Ecart d'angle trop grand
    {
      double d;
   
      if(fabsf(da)>M_PI/3.)
        d = 1;
      else
        d = fabsf(da) / M_PI/6.;         
      
      if(dist<0.1)
      {
        pw *= 0.3;
        pw = pw < min_pw ? min_pw : pw;
        if(da>=0)            // on tourne sur soi-même
          return wheel_speed_t(-pw,pw);
        else
          return wheel_speed_t(pw,-pw);         
      }
      else
      {
        pw *= (d<0.3 ? 0.3 : pow(d,0.5));
        pw = pw < min_pw ? min_pw : pw;        
        if(da>=0)            // on dévie
          return wheel_speed_t(-d*pw,pw);
        else
          return wheel_speed_t(pw,-d*pw);    
      }  
    }
    else                  // écart acceptable: on avance en ajustant
    {
      float d = 1. - (fabsf(da) / (M_PI/5.) / (v.norme()<1.?0.8:1.2)) ;      
        
      if(da>=0)
        return wheel_speed_t(d*pw,pw);
      else
        return wheel_speed_t(pw,d*pw);
    }
  }*/
}
//------------------------------------------------------------------------------
float direct_path_t::normalize(float angle)
{
  while(angle<-M_PI) angle+= 2.*M_PI;
  while(angle>=M_PI) angle-= 2.*M_PI;  
  return angle;
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void* pt_MainLoop(void*)
{
  bool first_empty = true;
  pthread_mutex_init(&mutex_situ, NULL);
  
  // Attend que le robot soit prêt
  while(!strat_is_ready()) usleep(10000);
  
  // Try to find configuration
  int config = wc_reco_config();
  pt_init(config);       
  strat_set_config_terrain(config);  
  
  while(true)
  {
    if(!pt_stopped)
    {
      if(path.empty())
      {
        if(first_empty) cine_reset_asserv();
        first_empty = false;
        cine_motors(0,0);
        usleep(10000);
      }
      else
      {
        first_empty = true;
        position_t pos = cine_get_wheel_center_position();
        if(path.front().is_dest_reached(pos))
        {
          if(path.front().mutex)
            pthread_mutex_unlock(path.front().mutex);
          path.pop_front();
          cine_reset_asserv();
        }
        else
        {
          wheel_speed_t wheel_speed = path.front().get_wheel_speed(pos);
          cine_motors(wheel_speed.left, wheel_speed.right);
          usleep(10000);        
        }
      }
    }
  }
}
//------------------------------------------------------------------------------
void pt_init(int config_terrain)
{
  pp_init();

  char file[300];  
  sprintf(file, "data/config%d.dat", config_terrain);
  if(!pp_load_from_file(file))
  {
    printf("LOADING OF PRECOMPUTED DISTANCE MAPS FAILED !!\n");
    printf("You should use tools/energyMap_tool to generate them and place the configxx.dat file into the ./data directory (create it if necessary).\n");
    pp_draw_config((config_terrain%100)/10, config_terrain%10);
    pp_compute_distance_transform();
  } 
}
//------------------------------------------------------------------------------
pthread_mutex_t* pt_go_to(const position_t &pos, int type, bool append)
{
  position_t p;
  vector_t N(cos(pos.a), sin(pos.a));
  if(type == tpWAYPOINT)
    p = pos;
  else
  {
    p.x = pos.x+_ROUE_X*cos(pos.a);
    p.y = pos.y+_ROUE_X*sin(pos.a);
    p.a = pos.a;
  }

  if(append && !path.empty())
  {
    if(type == tpDEST)
    {
      calc_path(path.back().dest, p, tpPREDEST, true);
      path.push_back(direct_path_t(p, tpDEST));
    }
    else
      calc_path(path.back().dest, p, type, true);
  }
  else
  {
    if(type == tpDEST)
    {
      calc_path(cine_get_wheel_center_position(), p, tpPREDEST, false);
      path.push_back(direct_path_t(p, tpDEST));
    }
    else
      calc_path(cine_get_wheel_center_position(), p, type, false);
  }

  pthread_mutex_t *mutex = new pthread_mutex_t;
  pthread_mutex_init(mutex, NULL);
  pthread_mutex_lock(mutex);
  path.back().mutex = mutex;
  return mutex;
}
//------------------------------------------------------------------------------
pthread_mutex_t* pt_add_step(const position_t &pos, int type)
{
  position_t p;
/*  if(type == tpMOVE)
  {
    p.x = pos.x+_ROUE_X*cos(pos.a);
    p.y = pos.y+_ROUE_X*sin(pos.a);
    p.a = pos.a;
  }
  else
    p = pos;*/
  p=pos;
    
  path.push_back(direct_path_t(p, type));

  pthread_mutex_t *mutex = new pthread_mutex_t;
  pthread_mutex_init(mutex, NULL);
  pthread_mutex_lock(mutex);
  path.back().mutex = mutex;
  return mutex;  
}
//------------------------------------------------------------------------------
void calc_path(const position_t &from, const position_t &to, int type, bool append)
{
  pp_path new_path = pp_find_path(from, to);

  if(!append)
    pt_clear_path(); 

  for(unsigned int i = 0; i<new_path.size(); i++)
    path.push_back(direct_path_t(new_path[i], i==new_path.size()-1?type:tpWAYPOINT));
}
//------------------------------------------------------------------------------
position_t pt_get_dest()
{
  if(path.empty())
    return cine_get_wheel_center_position();
  else
   return path.front().dest;   
}
//------------------------------------------------------------------------------
void pt_clear_path()
{
  path.clear();
}
//------------------------------------------------------------------------------
void pt_stop()
{
  if(pt_stopped)
    return;
  resume = path;
  pt_clear_path();  
  cine_onoff(false);        
  pic_MotorsPower(0.0,0.0); 
  cine_motors(0.0,0.0);
  pt_stopped = true;
}
//------------------------------------------------------------------------------
void pt_resume()
{
  path = resume;
  cine_onoff(true);         
  pt_stopped = false;
  printf("RESUME PATH!: %d steps remaining\n", path.size());
}
//-------------------------------------------------------------------------------
bool pt_is_stopped()
{
  return pt_stopped;
}
//-------------------------------------------------------------------------------
pp_path pt_get_path()
{
  list<direct_path_t> :: iterator iter = path.begin();
  vector<position_t> res;
  res.resize(path.size()+1);
  
  res[0] = cine_get_wheel_center_position();
  for(int i=1; iter != path.end(); iter++, i++)
    res[i] = iter->dest;

  return res;
}
//------------------------------------------------------------------------------
