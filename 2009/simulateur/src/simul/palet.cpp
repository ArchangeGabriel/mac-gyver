#include "palet.h"


//----------------------------------------------------------------------------
palet_t::palet_t(int c)
{
  fixe=false;
  rayon=0.035;
  masse=0.1;
  J=0.05*0.035*0.035;
  hauteur=0.03;
  matiere=mBois;
  state = stFREE;
  bottom = NULL;
  z_speed=0.;
  
  color=c;
  if(c==RED) couleur=getColorSDL(clRed);
  else couleur=getColorSDL(clGreen);
}
//----------------------------------------------------------------------------
void palet_t::calc_forces()
{
  calc_frottements();
  if(state == stFREE)
  {
    if(bottom == NULL)
      find_bottom();
    calc_vert_move();
  }
}
//----------------------------------------------------------------------------
void palet_t::calc_frottements()
{
  // frottements
  vector_t C1,C2,F;
  double norme;
  norme=-10*simul_info->coeff_frott[mBois][mBois]*masse;
  F = speed*norme;
  add_force(G_rot,F);    
  C1=N*rayon;
  C2=-C1;
  norme=(get_speed(C1)^N)/rayon*0.008;
  F=T*norme;
  add_force(C1,F);
  F=-F;
  add_force(C2,F);
}
//----------------------------------------------------------------------------
void palet_t::draw()
{
  int unit=0.01*simul_info->scale;
  vector_t centre=position-G_rot;    
  centre*=simul_info->scale;
  DisqueSDL(((int)centre.x),((int)centre.y),((int)(rayon*simul_info->scale)),couleur);  
  
  vector_t radius;
  radius.x=rayon;
  radius=radius.rotate_spec(cosinus,sinus);  radius*=simul_info->scale;
  LigneSDL(((int)(centre.x+radius.x)),((int)(centre.y+radius.y)),((int)(centre.x-radius.x)),((int)(centre.y-radius.y)),getColorSDL(clBlack));
  
  switch(((int)(z/hauteur+0.5)))
  {
    case 1:
    LigneSDL(((int)centre.x),((int)centre.y)-2*unit,((int)centre.x),((int)centre.y)+2*unit,getColorSDL(clWhite)); 
    break;
    case 2:
    LigneSDL(((int)centre.x)-unit,((int)centre.y)-2*unit,((int)centre.x)-unit,((int)centre.y)+2*unit,getColorSDL(clWhite));
    LigneSDL(((int)centre.x)+unit,((int)centre.y)-2*unit,((int)centre.x)+unit,((int)centre.y)+2*unit,getColorSDL(clWhite));      
    break;
    case 3:
    LigneSDL(((int)centre.x)-2*unit,((int)centre.y)-2*unit,((int)centre.x)-2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));
    LigneSDL(((int)centre.x),((int)centre.y)-2*unit,((int)centre.x),((int)centre.y)+2*unit,getColorSDL(clWhite));      
    LigneSDL(((int)centre.x)+2*unit,((int)centre.y)-2*unit,((int)centre.x)+2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));
    break;
    case 4:
    LigneSDL(((int)centre.x)-2*unit,((int)centre.y)-2*unit,((int)centre.x)-2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));
    LigneSDL(((int)centre.x)-unit,((int)centre.y)-2*unit,((int)centre.x)+unit,((int)centre.y)+2*unit,getColorSDL(clWhite));      
    LigneSDL(((int)centre.x)+3*unit,((int)centre.y)-2*unit,((int)centre.x)+unit,((int)centre.y)+2*unit,getColorSDL(clWhite));
    break; 
    case 5:
    LigneSDL(((int)centre.x)-2*unit,((int)centre.y)-2*unit,((int)centre.x),((int)centre.y)+2*unit,getColorSDL(clWhite));      
    LigneSDL(((int)centre.x)+2*unit,((int)centre.y)-2*unit,((int)centre.x),((int)centre.y)+2*unit,getColorSDL(clWhite));
    break;  
    case 6:
    LigneSDL(((int)centre.x)-3*unit,((int)centre.y)-2*unit,((int)centre.x)-unit,((int)centre.y)+2*unit,getColorSDL(clWhite));      
    LigneSDL(((int)centre.x)+unit,((int)centre.y)-2*unit,((int)centre.x)-unit,((int)centre.y)+2*unit,getColorSDL(clWhite));
    LigneSDL(((int)centre.x)+2*unit,((int)centre.y)-2*unit,((int)centre.x)+2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));    
    break;             
    case 7:
    LigneSDL(((int)centre.x)-3*unit,((int)centre.y)-2*unit,((int)centre.x)-2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));      
    LigneSDL(((int)centre.x)-unit,((int)centre.y)-2*unit,((int)centre.x)-2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));
    LigneSDL(((int)centre.x),((int)centre.y)-2*unit,((int)centre.x),((int)centre.y)+2*unit,getColorSDL(clWhite));    
    LigneSDL(((int)centre.x)+2*unit,((int)centre.y)-2*unit,((int)centre.x)+2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));        
    break;   
    case 8:
    LigneSDL(((int)centre.x)-2*unit,((int)centre.y)-2*unit,((int)centre.x)-2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));
    LigneSDL(((int)centre.x),((int)centre.y)-2*unit,((int)centre.x),((int)centre.y)+2*unit,getColorSDL(clWhite));      
    LigneSDL(((int)centre.x)+2*unit,((int)centre.y)-2*unit,((int)centre.x)+2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));       
    LigneSDL(((int)centre.x)-2*unit,((int)centre.y)-2*unit,((int)centre.x)+2*unit,((int)centre.y)-2*unit,getColorSDL(clWhite));       
    break;                
    case 9:
    LigneSDL(((int)centre.x)-2*unit,((int)centre.y)-2*unit,((int)centre.x)-2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));
    LigneSDL(((int)centre.x)-unit,((int)centre.y)-2*unit,((int)centre.x)+3*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));      
    LigneSDL(((int)centre.x)+3*unit,((int)centre.y)-2*unit,((int)centre.x)-unit,((int)centre.y)+2*unit,getColorSDL(clWhite)); 
    break; 
    default:
    LigneSDL(((int)centre.x)-2*unit,((int)centre.y)-2*unit,((int)centre.x)+2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite));      
    LigneSDL(((int)centre.x)+2*unit,((int)centre.y)-2*unit,((int)centre.x)-2*unit,((int)centre.y)+2*unit,getColorSDL(clWhite)); 
    break;                    
  }  
}
//----------------------------------------------------------------------------  
void palet_t::find_bottom()
{
  obj_z = 0.;
  object_t *obj=this;
  for(unsigned int i=0;i<simul_info->objets.size();i++)
    if(simul_info->objets[i] != this && simul_info->objets[i]->z+simul_info->objets[i]->hauteur/2. <= z - hauteur/2.)
      if(just_dist_of_obj(simul_info->objets[i])<rayon && simul_info->objets[i]->z+simul_info->objets[i]->hauteur/2.>obj_z)
      {
        obj_z = simul_info->objets[i]->z+simul_info->objets[i]->hauteur/2.;
        obj = simul_info->objets[i];
      }
  bottom = obj;
  if(obj_z == z-hauteur/2.) z_speed=0.;
}
//----------------------------------------------------------------------------  
void palet_t::calc_vert_move()
{
  if(z-hauteur/2. > obj_z)
  {
    z_speed += simul_info->g*simul_info->dt;
    z -= z_speed*simul_info->dt;
  }
  if(z-hauteur/2. <= obj_z)
  {
    z = obj_z+hauteur/2.;
    if((bottom !=this && bottom->z + bottom->hauteur/2. != obj_z) ||
       just_dist_of_obj(bottom)>=rayon)
      bottom = NULL; 
  }
}
//----------------------------------------------------------------------------  
double palet_t::just_dist_of_obj(object_t *obj)
{
  object_t *ss_obj = this;
  vector_t n;
  vector_t v=position-G_rot-obj->position;
  double d = obj->dist_of_obj(v,n,ss_obj);
  if(ss_obj)
    return d;
  else
  {
    if(d==0.) return 100000.;
    else return 0.;
  }
}
//----------------------------------------------------------------------------  
void palet_t::set_free()
{
  state = stFREE;
  bottom = NULL;
}
//----------------------------------------------------------------------------  
void palet_t::set_taken()
{
  state = stTAKEN;  
}
//----------------------------------------------------------------------------  
