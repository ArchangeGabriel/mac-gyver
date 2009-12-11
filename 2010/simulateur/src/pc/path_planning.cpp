#include <list>
#include <math.h>
#include <unistd.h>
#include "path_planning.h"
#include "strategie.h"
#include "cinematik.h"
#include "picAPI.h"
#include <cstdio>

#define PC_INCLUDE
#include "../common/const.h"

using namespace std;

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

#define DANGER_ZONE  0.15     // Largeur de la zone de danger (on fait attention près des bords).
#define SLOWER_ZONE  0.20     // Largeur de la zone de ralentissement (on ralentit près des bords).

#define MAX_DANGER_SPEED  0.7  // Vitesse max en zone de danger
#define MAX_SLOWER_SPEED  1.   // Vitesse max en zone de ralentissement
#define MAX_FREE_SPEED    1.3  // Vitesse max en zone libre

#define DISTRIB_OFFSET    0.24 // En mètre: espace entre le centre du robot et le bord quand collé au distrib
#define DROP_OFFSET       0.24 // En mètre: espace entre le centre du robot et le bord quand drop

#define RADIUS_PROTECT    0.35 // En mètre: rayon du cercle central à éviter   

//------------------------------------------------------------------------------

pthread_mutex_t mutex_situ;

int pp_situation = 0;   // 0 pas de danger, 1 ralentir
int pp_dangerous_captor_id;

typedef struct zones_t
{
  int ZH;         // Zone de danger grand bord:  -1 pas dedans, 0 zone côté départ, 1 zone côté dépot
  vector_t NZH;   // Normale à la zone de danger, vers l'extérieure
  int ZV;         // Zone de danger petit bord:  -1 pas dedans, 0 zone côté vert, 1 zone côté rouge
  vector_t NZV;   // Normale à la zone de danger, vers l'extérieure
  int ZC;         // Zone de danger du centre:   -1 pas dedans, 0 sinon
  vector_t NZC;   // Normale à la zone de danger, vers l'extérieure
} zones_t;

zones_t get_zones(position_t *pos, float sensitivity);  // Calcule les zones de danger

//----------------------------------------------------------------------------//
//                                 ZONE_T                                     //
//----------------------------------------------------------------------------//
zones_t get_zones(position_t *pos, float sensitivity)
{
  zones_t zones;

  if(pos->y<sensitivity+DROP_OFFSET)
  {
    zones.ZH = 0;
    zones.NZH = vector_t(0.,1.);
  }
  else if(pos->y>_LARGEUR_TER-sensitivity-0.1-DROP_OFFSET)
  {
    zones.ZH = 1;
    zones.NZH = vector_t(0.,-1.);    
  }
  else
  {
    zones.ZH = -1;
    zones.NZH = vector_t(0.,0.);  
  }
  
  if(pos->x<sensitivity+DROP_OFFSET)
  {
    zones.ZV = 0;
    zones.NZV = vector_t(1.,0.);
  }
  else if(pos->x>_LONGUEUR_TER-sensitivity-DROP_OFFSET)
  {
    zones.ZV = 1;
    zones.NZV = vector_t(-1.,0.);    
  }
  else
  {
    zones.ZV = -1;
    zones.NZV = vector_t(0.,0.);  
  }
  
  vector_t to_centre = vector_t(pos->x-_LONGUEUR_TER/2., pos->y-_LARGEUR_TER/2.);
  if(to_centre.norme() < 0.15+sensitivity+DROP_OFFSET)
  {
    zones.ZC = 0;
    zones.NZC = to_centre/to_centre.norme();
  }
  else
  {
    zones.ZC = -1;
    zones.NZC = vector_t(0.,0.);  
  }
  
  return zones;
}
//------------------------------------------------------------------------------





//----------------------------------------------------------------------------//
//                              DIRECT_PATH_T                                 //
//----------------------------------------------------------------------------//
class direct_path_t
{
  private:
  float normalize(float angle);   // renvoie un angle compris entre -M_PI et M_PI

  
  bool is_dest_reached_xy(position_t *pos);   // renvoie vrai si la destination est atteinte (à peu près) en x y
  bool is_dest_reached_a(position_t *pos);    // renvoie vrai si la destination est atteinte (à peu près) en angle
  
  public:
  position_t dest;      // destination de l'étape
  int type;             // destination ou étape?
  
  bool reached;         // destination déjà atteinte?
  int iter;             // compte le nombre d'itération successives où le but est atteint
  
  pthread_mutex_t *mutex;

  direct_path_t(position_t dest, int type):dest(dest),type(type){mutex = NULL; reached = false; iter = 0;} // constructeur de classe
  
  bool is_dest_reached(position_t *pos);      // renvoie vrai si la destination est atteinte (à peu près)
  tension_t get_tension(position_t *pos);     // renvoie la tension des moteurs pour atteindre le but
};

list<direct_path_t> path;
//------------------------------------------------------------------------------
bool direct_path_t::is_dest_reached_xy(position_t *pos)
{
  if(reached) return true;
  
  vector_t dist = vector_t(pos->x-dest.x, pos->y-dest.y);

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
bool direct_path_t::is_dest_reached_a(position_t *pos)
{
  switch(type)
  {
    case tpDEST:
    return (fabsf(normalize(pos->a-dest.a))<DEST_ORIENT);   
    case tpAPPROACH:
    return (fabsf(normalize(pos->a-dest.a))<APPROACH_ORIENT);
    case tpLEAVE:
    return (fabsf(normalize(pos->a-dest.a))<LEAVE_ORIENT);    
    case tpWAYPOINT:
    return (fabsf(normalize(pos->a-dest.a))<WAYPOINT_ORIENT);   
    default:
    return true;
  }  
}
//------------------------------------------------------------------------------
bool direct_path_t::is_dest_reached(position_t *pos)
{
  return is_dest_reached_xy(pos) && is_dest_reached_a(pos) && iter>5;
}
//------------------------------------------------------------------------------
tension_t direct_path_t::get_tension(position_t *pos)
{ 
  zones_t danger_zones;
  zones_t slower_zones;
  
  // Calcul des zones de danger
  danger_zones = get_zones(pos, DANGER_ZONE);
  slower_zones = get_zones(pos, SLOWER_ZONE);
  
  int status;    // 0 rien, 1 slow down, 2 danger
  vector_t N = vector_t(cos(pos->a), sin(pos->a));
  
       if(danger_zones.ZH != -1 && (N|danger_zones.NZH)<-0.2) status = 2;
  else if(danger_zones.ZV != -1 && (N|danger_zones.NZV)<-0.2) status = 2;
  else if(danger_zones.ZC != -1 && (N|danger_zones.NZC)<-0.2) status = 2;
  else if(slower_zones.ZH != -1 && (N|slower_zones.NZH)<-0.2) status = 1;
  else if(slower_zones.ZV != -1 && (N|slower_zones.NZV)<-0.2) status = 1;
  else if(slower_zones.ZC != -1 && (N|slower_zones.NZC)<-0.2) status = 1;
  else status = 0;
  
  // on calcule l'angle vers la destination
  vector_t v = dest.v() - pos->v();
   
  float max_pw;
    
       if(status == 0) max_pw = MAX_FREE_SPEED;
  else if(status == 1) max_pw = MAX_SLOWER_SPEED;
  else if(status == 2) max_pw = MAX_DANGER_SPEED;
  
  if(pp_situation == 1) max_pw /= 2.;
  
  if(is_dest_reached_xy(pos))    // on a déjà atteint la destination: on continue à tourner
  {
   // reached = true;
   
    if(is_dest_reached_a(pos)) 
    {
      iter++;
      return tension_t(0.,0.);      
    }
    else
    {
      iter = 0;
      float da = normalize(pos->a-dest.a);
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
    float da = normalize(pos->a-v.to_angle()); 
    
  /*  if(status == 2 && fabs(da)>M_PI_2 )  // l'objectif est derrière: on recule
    {  
      if(danger_zones.ZV!=-1 && danger_zones.ZH!=-1)  // trop de danger et on ne sait pas conduire: on recule tout droit
      {
        return tension_t(-max_pw,-max_pw);
     }
     else   // sinon, on tente le créneau
      {
        float pw = pow(fabsf(da)/(1.5*M_PI),1.3) * max_pw;     
        float d = 1. - (fabsf(da) / (2.* M_PI));
        
        if(da>=0)
          return tension_t(-d*pw,-pw);
        else
          return tension_t(-pw,-d*pw);
      }
    }*/
    if(fabs(da)>M_PI_2 && (status == 2 || v.norme()<0.05))
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
            vector_t v = (*iter).dest.v() - pos->v();
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



void calc_path(position_t from, position_t to, int type, bool append = false, bool do_approach = true, bool do_leave = true, pthread_mutex_t **approach = NULL, pthread_mutex_t **leave = NULL);


//------------------------------------------------------------------------------
pthread_mutex_t* pp_go_to(position_t pos, int type, bool append, bool do_approach, bool do_leave, pthread_mutex_t **approach, pthread_mutex_t **leave)
{
  calc_path(cine_get_position(), pos, type, append, do_approach, do_leave, approach, leave);

  pthread_mutex_t *mutex = new pthread_mutex_t;
  pthread_mutex_init(mutex, NULL);
  pthread_mutex_lock(mutex);
  path.back().mutex = mutex;
  return mutex;
}
//------------------------------------------------------------------------------
pthread_mutex_t* pp_add_step(position_t pos)
{
  calc_path(pp_get_dest(), pos, tpDEST, true);  

  pthread_mutex_t *mutex = new pthread_mutex_t;
  pthread_mutex_init(mutex, NULL);
  pthread_mutex_lock(mutex);
  path.back().mutex = mutex;
  return mutex;  
}
//------------------------------------------------------------------------------
void calc_path(position_t from, position_t to, int type, bool append, bool do_approach, bool do_leave, pthread_mutex_t **approach, pthread_mutex_t **leave)
{
 /* vector_t c = vector_t(_LONGUEUR_TER/2., _LARGEUR_TER/2.);
  vector_t u = to.v() - from.v();
  vector_t v = c - from.v();
  
  float l = u.norme();
  u = u/u.norme();

  float proj = v|u;
      
  if(do_leave) // que pour les placements en zone de danger
  {
     vector_t N = vector_t(cos(from.a), sin(from.a));
  
    // on regarde si la provenance est critique
    zones_t zones = get_zones(&from, DANGER_ZONE);
    if(zones.ZC != -1  && (N|zones.NZC)<-0.2)  // on veut aller près du centre
    {
      vector_t p = c + zones.NZC * (0.15 + DANGER_ZONE + DROP_OFFSET);
      position_t pos;
      pos.x = p.x;
      pos.y = p.y;
      pos.a = (-zones.NZC).to_angle();
      calc_path(from, pos, tpLEAVE, append, false, false, approach, leave);
      calc_path(pos , to , type    , true  , do_approach, false, approach, leave);

      if(leave)
      {
        *leave = new pthread_mutex_t;
        pthread_mutex_init(*leave, NULL);
        pthread_mutex_lock(*leave);      
      }
      return;
    } 

    // si l'un des coté pose problème
    if((zones.ZH != -1 && (N|zones.NZH)<-0.2) || 
       (zones.ZV != -1 && (N|zones.NZV)<-0.2))
    {
      position_t pos;
      vector_t d = vector_t(cos(from.a), sin(from.a));
      d = from.v() - d*(SLOWER_ZONE);
      pos.x = d.x;
      pos.y = d.y;
      pos.a = to.a;
      calc_path(from, pos, tpLEAVE, append, false, false, approach, leave);
      calc_path(pos , to , type   , true  , do_approach, false, approach, leave);   
      
      if(leave)
      {
        *leave = new pthread_mutex_t;
        pthread_mutex_init(*leave, NULL);
        pthread_mutex_lock(*leave);      
      }
      return;
    }      
  }
  
  if(do_approach)
  {
    // on regarde si la destination est critique
    zones_t zones = get_zones(&to, DANGER_ZONE);
    
    if(zones.ZC != -1)  // on veut aller près du centre
    {
      vector_t p = c + zones.NZC * (0.15 + DANGER_ZONE + DROP_OFFSET);
      position_t pos;
      pos.x = p.x;
      pos.y = p.y;
      pos.a = (-zones.NZC).to_angle();
      calc_path(from, pos, tpAPPROACH, append, false, do_leave, approach, leave);
      calc_path(pos , to , type      , true  , false, false, approach, leave);
      
      if(approach)
      {
        *approach = new pthread_mutex_t;
        pthread_mutex_init(*approach, NULL);
        pthread_mutex_lock(*approach);      
      }
      return;
    } 
    
    // si l'un des coté pose problème
    if(zones.ZH != -1 || zones.ZV != -1)
    {
      position_t pos;
      vector_t d = vector_t(cos(to.a), sin(to.a));
      d = to.v() - d*(SLOWER_ZONE);
      pos.x = d.x;
      pos.y = d.y;
      pos.a = to.a;
      calc_path(from, pos, tpAPPROACH, append, false, leave);
      calc_path(pos , to , type      , true  , false, false);   

      if(approach)
      {
        *approach = new pthread_mutex_t;
        pthread_mutex_init(*approach, NULL);
        pthread_mutex_lock(*approach);
      }
      return;
    }
  }
  
  if(proj>=0. && proj<=l)  // Le segment traverse éventuellement le centre
  {
    if(fabsf(u^v) < RADIUS_PROTECT)  // effectivement: il traverse le centre
    {
      vector_t n;
      if((u^v)>=0) n = u.rotate(-M_PI_2);
            else n = u.rotate(M_PI_2);
      n = c + n*(1.2*RADIUS_PROTECT);
      position_t pos;
      pos.x = n.x;
      pos.y = n.y;
      pos.a = u.to_angle();
      calc_path(from, pos, tpWAYPOINT, append, false, do_leave, approach, leave);
      calc_path(pos , to , type      , true  , do_approach, false, approach, leave);      
      return;
    }
  }*/
      
  // si on arrive içi, la ligne droite est sure.
  if(!append)
    pp_clear_path();
    
  path.push_back(direct_path_t(to, type));   
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

  fprintf(stderr,"PP thread ok...\n");  fflush(stdout);
  while(true)
  {
    if(path.empty())
      usleep(10000);
    else
    {
      position_t pos = cine_get_position();
      if(path.front().is_dest_reached(&pos))
      {
        cine_motors(0.,0.);
        if(path.front().mutex)
          pthread_mutex_unlock(path.front().mutex);
        path.pop_front();
        if(pp_situation == 2)
          pp_stop(pp_dangerous_captor_id);
      }
      else
      {
        tension_t tension = path.front().get_tension(&pos);
        cine_motors(tension.left, tension.right);
        usleep(10000);        
      }
    }
  }
}
//------------------------------------------------------------------------------
void pp_lock_situ()
{
  pthread_mutex_lock(&mutex_situ);
  pp_situation = 0;
}
//------------------------------------------------------------------------------
void pp_unlock_situ()
{
  pthread_mutex_unlock(&mutex_situ);
}
//------------------------------------------------------------------------------
void pp_slow_down()
{
  if(pp_situation == 0)
    pp_situation = 1;
}
//------------------------------------------------------------------------------
void pp_stop(int id)
{
  pp_situation = 2;
  pp_dangerous_captor_id = id;
  picMotorsPower(0.,0.);
  position_t pos = cine_get_position();
  vector_t N = vector_t(cos(pos.a), sin(pos.a));
  if(id == 0 || id == 1) N = -N;
  pos.x += N.x * 0.03;
  pos.y += N.y * 0.03;
  path.push_front(direct_path_t(pos,tpDEST));
}
//------------------------------------------------------------------------------
void pp_no_danger()
{
  pp_situation = 0;
}
//------------------------------------------------------------------------------
void pp_resume()
{
  //FIXME
}
//------------------------------------------------------------------------------

