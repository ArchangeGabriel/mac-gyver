#include <list>
#include <math.h>
#include <unistd.h>
#include "path_planning.h"
#include "strategie.h"
#include "dist_transform.h"
#include "cinematik.h"
#include "picAPI.h"
#include "visualizer.h"
#include <cstdio>

#define PC_INCLUDE
#include "../common/const.h"
#include "../common/bitmap.h"

using namespace std;

pthread_mutex_t mutex_situ;

dt_map *pathMap = NULL;

void calc_path(const position_t &from, const position_t &to, int type, bool append = false);

//------------------------------------------------------------------------------

#define MAX_SPEED    1.3    // Vitesse maximale atteignable

//#define DEST_RADIUS  0.02   // distance minimum (en mètres) pour valider le waypoint
//#define DEST_ORIENT  0.01  // écart de direction minimum (en radian) pour valider le waypoint

#define DEST_RADIUS  0.05   // distance minimum (en mètres) pour valider le waypoint
#define DEST_ORIENT  0.17   // écart de direction minimum

#define APPROACH_RADIUS  0.01 // distance minimum (en mètres) pour valider le waypoint
#define APPROACH_ORIENT  0.01 // écart de direction minimum (en radian) pour valider le waypoint

#define LEAVE_RADIUS  0.05    // distance minimum (en mètres) pour valider le waypoint
#define LEAVE_ORIENT  1.5     // écart de direction minimum (en radian) pour valider le waypoint

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
  const position_t dest;      // destination de l'étape
  int type;             // destination ou étape?
  
  bool reached;         // destination déjà atteinte?
  int iter;             // compte le nombre d'itération successives où le but est atteint
  
  pthread_mutex_t *mutex;

  direct_path_t(const position_t &dest, int type):dest(dest),type(type){mutex = NULL; reached = false; iter = 0;} // constructeur de classe
  
  bool is_dest_reached(const position_t &pos);      // renvoie vrai si la destination est atteinte (à peu près)
  tension_t get_tension(const position_t &pos);     // renvoie la tension des moteurs pour atteindre le but
};

list<direct_path_t> path;
//------------------------------------------------------------------------------
bool direct_path_t::is_dest_reached_xy(const position_t &pos)
{
  if(reached) return true;
  
  vector_t dist = vector_t(pos.x-dest.x, pos.y-dest.y);

  switch(type)
  {
    case tpDEST:
    return (dist.norme()<DEST_RADIUS); 
    case tpAPPROACH:
    return (dist.norme()<APPROACH_RADIUS);    
    case tpLEAVE:
    return (dist.norme()<LEAVE_RADIUS);        
    case tpWAYPOINT:
    return (dist.norme()<WAYPOINT_RADIUS);    
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
    case tpAPPROACH:
    return (fabsf(normalize(pos.a-dest.a))<APPROACH_ORIENT);
    case tpLEAVE:
    return (fabsf(normalize(pos.a-dest.a))<LEAVE_ORIENT);    
    case tpWAYPOINT:
    return (fabsf(normalize(pos.a-dest.a))<WAYPOINT_ORIENT);   
    default:
    return true;
  }  
}
//------------------------------------------------------------------------------
bool direct_path_t::is_dest_reached(const position_t &pos)
{
  return is_dest_reached_xy(pos) && is_dest_reached_a(pos) && iter>5;
}
//------------------------------------------------------------------------------
tension_t direct_path_t::get_tension(const position_t &pos)
{ 
  vector_t N = vector_t(cos(pos.a), sin(pos.a));
  
  // on calcule l'angle vers la destination
  vector_t v = dest.v() - pos.v();
   
  float max_pw = MAX_SPEED;
    
  if(is_dest_reached_xy(pos))    // on a déjà atteint la destination: on continue à tourner
  {  
    if(is_dest_reached_a(pos)) 
    {
      iter++;
      return tension_t(0.,0.);      
    }
    else
    {
      iter = 0;
      float da = normalize(pos.a-dest.a);
      float pw;
      if(fabsf(da)>M_PI/3.)
        pw = fabsf(da)/(1.2 * M_PI) * max_pw;
      else
        pw = (fabsf(da)<M_PI/18.?0.04:0.08) + fabsf(da) / M_PI * max_pw;
      
      if(da>=0)            // on tourne sur soi-même
        return tension_t(-pw,pw);
      else   if(fabsf(da)>M_PI/3.)
        pw = fabsf(da)/(1.2 * M_PI) * max_pw;
      else
        pw = (fabsf(da)<M_PI/18.?0.04:0.08) + fabsf(da) / M_PI * max_pw;
        return tension_t(pw,-pw);  
    }     
  }   
  else
  {      
    float da = normalize(pos.a-v.to_angle()); 
    
    if(fabs(da)>M_PI_2 && v.norme()<0.05)
    {
      if(v.norme()<0.05)
        return tension_t(-0.5,-0.5);
      else
        return tension_t(-max_pw,-max_pw);
    }
    else if(fabsf(da)>M_PI/5.)   // Ecart d'angle trop grand
    {
      float d;
      float pw;
   
      if(fabsf(da)>M_PI/3.)
      {
        d = 1;
        pw = fabsf(da)/(1.2 * M_PI) * max_pw;
      }
      else
      {
        d = fabsf(da) / M_PI/5.;      
        pw = (fabsf(da)<M_PI/18.?0.04:0.08) + fabsf(da) / M_PI * max_pw;      
      }
      
      if(v.norme()<0.1)
      {
        if(da>=0)            // on tourne sur soi-même
          return tension_t(-pw,pw);
        else
          return tension_t(pw,-pw);         
      }
      else
      {
        if(da>=0)            // on tourne sur soi-même
          return tension_t(-d*pw,pw);
        else
          return tension_t(pw,-d*pw);    
      }  
    }
    else                  // écart acceptable: on avance en ajustant
    {
      float d = 1. - (fabsf(da) / (M_PI/5.) / (v.norme()<1.?1.2:1.6)) ;
      float pw = max_pw;
      float dist;
      
      if(type != tpWAYPOINT)
        dist = v.norme();
      else
      {
        list<direct_path_t> :: iterator iter;
        for(iter = path.begin(); iter != path.end(); ++iter)
          if((*iter).type != tpWAYPOINT)
          {
            vector_t v = (*iter).dest.v() - pos.v();
            dist = v.norme();
            break;
          }
      }
        
      
      if(dist<0.5)
      {
        if(dist>0.4)
          pw = pow(dist / 0.5, 0.8) * max_pw;
        else
          pw = (dist<0.04?0.02:0.04) + pow(dist / 1.5, 1.2) * max_pw;        
      }
                
      if(pw > max_pw) pw = max_pw;
      
      if(da>=0)
        return tension_t(d*pw,pw);
      else
        return tension_t(pw,d*pw);
    }
  }
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
pthread_mutex_t* pp_go_to(const position_t &pos, int type, bool append)
{
  calc_path(cine_get_position(), pos, type, append);

  pthread_mutex_t *mutex = new pthread_mutex_t;
  pthread_mutex_init(mutex, NULL);
  pthread_mutex_lock(mutex);
  path.back().mutex = mutex;
  return mutex;
}
//------------------------------------------------------------------------------
pthread_mutex_t* pp_add_step(const position_t &pos)
{
  return pp_go_to(pos, tpDEST, true);  
}
//------------------------------------------------------------------------------
void calc_path(const position_t &from, const position_t &to, int type, bool append)
{
  dt_path new_path = pathMap->find_path(from, to);
  
  visu_draw_dt_path(&new_path);

  if(!append)
    pp_clear_path();
    
  //path.push_back(direct_path_t(to, type)); 
}
//------------------------------------------------------------------------------
position_t pp_get_dest()
{
  if(path.empty())
    return cine_get_position();
  else
   // return path.back().dest;
   return path.front().dest;   
}
//------------------------------------------------------------------------------
void pp_clear_path()
{
  path.clear();
}
//------------------------------------------------------------------------------
void ppMainLoop()
{

  pthread_mutex_init(&mutex_situ, NULL);

  fprintf(stderr,"PP thread...                    ok\n");  fflush(stdout);
  while(true)
  {
    if(path.empty())
      usleep(10000);
    else
    {
      position_t pos = cine_get_position();
      if(path.front().is_dest_reached(pos))
      {
        cine_motors(0.,0.);
        if(path.front().mutex)
          pthread_mutex_unlock(path.front().mutex);
        path.pop_front();
      }
      else
      {
        tension_t tension = path.front().get_tension(pos);
        cine_motors(tension.left, tension.right);
        usleep(10000);        
      }
    }
  }
}
//------------------------------------------------------------------------------
void pp_stop(int id)
{
  picMotorsPower(0.,0.);
  position_t pos = cine_get_position();
  vector_t N = vector_t(cos(pos.a), sin(pos.a));
  if(id == 0 || id == 1) N = -N;
  pos.x += N.x * 0.03;
  pos.y += N.y * 0.03;
  path.push_front(direct_path_t(pos,tpDEST));
}
//------------------------------------------------------------------------------
void pp_init(int config_terrain)
{
  pathMap = new dt_map(_LONGUEUR_TER, _LARGEUR_TER, _LARGEUR_ROBOT, _LONGUEUR_ROBOT);
  
  int configE = (config_terrain%100)/10;
  int configI = config_terrain%10;
  
  if(configE != 0 && configI !=0)
  {
    bool FakeCornE[3][3];
    bool FakeCornI[2];
    FakeCornE[0][0] = (configE==2 || configE==5 || configE==8);
    FakeCornE[1][0] = (configE==4 || configE==5 || configE==6);  
    FakeCornE[2][0] = (configE==7 || configE==8 || configE==9);    
    FakeCornE[0][1] = (configE==1 || configE==2 || configE==7);
    FakeCornE[1][1] = (configE==1 || configE==2 || configE==3);  
    FakeCornE[2][1] = (configI==2 || configI==4);
    FakeCornE[0][2] = (configE==3 || configE==4 || configE==9);
    FakeCornE[1][2] = (configI==1 || configI==3);
    FakeCornE[2][2] = false;      
    FakeCornI[0] = (configI==1 || configI==2);
    FakeCornI[1] = (configI==3 || configI==4);
    
    for(int i=0;i<3;++i)  
      for(int j=0;j<3;++j)    
        if(i != 2 || j != 2)
        {
          if(FakeCornE[i][j])
          {
            double x,y;
            x = 0.15 + 0.45*i;
            y = 2.1 - 1.378 + 0.5*j + 0.25*i;   
            pathMap->fillDisc(x, y, 0.025);

            x = _LONGUEUR_TER - 0.15 - 0.45*i;
            y = 2.1 - 1.378 + 0.5*j + 0.25*i;
            pathMap->fillDisc(x, y, 0.025);
          }
        }
        
    for(int i=0;i<2;++i) 
    {
      if(FakeCornI[i])
      {
        double x = _LONGUEUR_TER/2.;
        double y = 2.1 - 0.628 + 0.5*i; 
        pathMap->fillDisc(x, y, 0.025);
      }
    }
  }   
  pathMap->fillBox(0.74, 0., 1.519, 0.52); 
  pathMap->fillBox(0., 0., _LONGUEUR_TER, DT_DIST_RESOL); 
  pathMap->fillBox(0., 0.,  DT_DIST_RESOL, _LARGEUR_TER);
  pathMap->fillBox(0., _LARGEUR_TER, _LONGUEUR_TER, DT_DIST_RESOL); 
  pathMap->fillBox(_LONGUEUR_TER, 0.,  DT_DIST_RESOL, _LARGEUR_TER);
  
  pathMap->compute_distance_transform();
  
  int w,h;
  char file[100];
  for(int i=0; i<DT_ANGLE_RESOL; i++)
  {
    uint16_t *pix = pathMap->to_bitmap(i, w, h);
    sprintf(file, "pathMap%d.bmp", i);
    save_buff_to_bitmap(file, w, h, pix);
    delete[] pix;
  }
}
