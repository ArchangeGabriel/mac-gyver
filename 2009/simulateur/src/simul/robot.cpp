#include "../common/comm.h"

#include "robot.h"

//----------------------------------------------------------------------------
robot_t::robot_t()
{
  objects.clear();
  rect_obj_t *robot;
  robot=new rect_obj_t;  
  robot->type=OBJ_ROBOT;
  robot->couleur=getColorSDL(clDarkGray);    
  add_object(robot);
  robot=new rect_obj_t;  
  robot->type=OBJ_ROBOT;
  robot->couleur=getColorSDL(clDarkGray);      
  add_object(robot);
  robot=new rect_obj_t;
  robot->type=OBJ_ROBOT;
  robot->couleur=getColorSDL(clDarkGray);      
  add_object(robot);
  
  union_obj_t *pince=new union_obj_t;
  pince->type=OBJ_ROBOT;
  rect_obj_t *pince1=new rect_obj_t;
  pince1->dimX=0.01;
  pince1->dimY=0.12;
  pince1->hauteur=0.3;
  pince1->z=0.3/2;   
  pince1->couleur=getColorSDL(clLightGray);      
  pince1->type=OBJ_ROBOT;
  pince->add_object(pince1);
  circle_obj_t *pince2=new circle_obj_t;
  pince2->rayon=0.04;
  pince2->hauteur=0.115;
  pince2->z=0.115/2;  
  pince2->type=OBJ_PINCE_OUVERTE;  
  pince2->couleur=getColorSDL(clGray);   
  pince->add_object(pince2);
  pince->objects_pos[0].x=0.;
  pince->objects_pos[0].y=0.;  
  pince->objects_pos[1].x=0.08;
  pince->objects_pos[1].y=0.;
  add_object(pince);

  for(int i=0;i<2;i++)
    tension_moteur[i]=0;
  tension_courroie=0;
  
  webcams.clear();
  color_captors.clear();
  dist_captors.clear();  
  element_in_clamp.clear();
}
//----------------------------------------------------------------------------
robot_t::~robot_t()
{
  delete data;
}
//----------------------------------------------------------------------------
void robot_t::init_params()
{
  printf("--> Loading robot's params...   ");
  fflush(stdout); 
  double J_roue;          // moment d'inertie des roues
  double moment_massique_additionnel;

  manual    = int_param_value("commande_manuelle"); 
  G_init.x  = double_param_value("centre_gravite_x");  
  G_init.y  = double_param_value("centre_gravite_y");  
  roues_x   = double_param_value("roues_x");  
  roues_y   = double_param_value("roues_y");   

  moment_massique_additionnel=pow(G_init.x-roues_x,2)+pow(G_init.y,2);
  G_init.x=roues_x;
  G_init.y=0.;
  roue[0].x=roues_x;
  roue[0].y=-roues_y/2;
  roue[1].x=roues_x;  
  roue[1].y=roues_y/2;  
  roue[0]=roue[0]-G_init;
  roue[1]=roue[1]-G_init;  

  masse           = double_param_value("masse");
  J               = double_param_value("moment_inertie_robot") + masse*moment_massique_additionnel;
  J_roue          = double_param_value("moment_inertie_roue");     
  matiere         = int_param_value("matiere");
  dimX            = double_param_value("dimension_x");
  dimY            = double_param_value("dimension_y");  
  hauteur         = double_param_value("hauteur"); 
  double surelev  = double_param_value("sureleve");   
  longueur_recept = double_param_value("longueur_recept"); 
  clamp_midX      = double_param_value("pince_x_mid");
  h_speed_move    = double_param_value("vitesse_depl_h");      
  double clampH   = double_param_value("hauteur_pince");
  clamp_minZ      = double_param_value("pince_z_min");   
  clamp_maxZ      = double_param_value("pince_z_max");
  v_speed_move    = double_param_value("vitesse_depl_v"); 
  proba_miss_step = double_param_value("proba_miss_step"); 
  
  z = surelev + hauteur/2.;
  ((rect_obj_t*)objects[0])->dimX=dimX;
  ((rect_obj_t*)objects[0])->dimY=(dimY-0.085)/2.;
  ((rect_obj_t*)objects[0])->hauteur=hauteur;  
  ((rect_obj_t*)objects[0])->z=z;    
  ((rect_obj_t*)objects[1])->dimX=dimX-longueur_recept;
  ((rect_obj_t*)objects[1])->dimY=0.085;
  ((rect_obj_t*)objects[1])->hauteur=hauteur;  
  ((rect_obj_t*)objects[1])->z=z;        
  ((rect_obj_t*)objects[2])->dimX=dimX;
  ((rect_obj_t*)objects[2])->dimY=(dimY-0.085)/2.;
  ((rect_obj_t*)objects[2])->hauteur=hauteur;    
  ((rect_obj_t*)objects[2])->z=z; 
  ((rect_obj_t*)objects[3])->z=z;           
  objects_pos[0].x=0.;  
  objects_pos[0].y=(dimY+0.085)/4.;
  objects_pos[1].x=-longueur_recept/2.;  
  objects_pos[1].y=0.;  
  objects_pos[2].x=0.;
  objects_pos[2].y=-(dimY+0.085)/4.;   

  //pince  
  clamp_minX =  -(longueur_recept-dimX/2)-0.04;
  clamp_midX += clamp_minX;
  clamp_maxX =  dimX/2-0.02;  
  clamp_X = posClampBack;
  objects_pos[3].x=clamp_minX;
  objects_pos[3].y=0.0;
  ((circle_obj_t*)((union_obj_t*)objects[3])->objects[1])->hauteur=clampH;    
  ((union_obj_t*)objects[3])->objects[1]->z=clamp_minZ + clampH/2.;   
  clamp_Z = clamp_minZ;
  
  position.x       = double_param_value("position_x");  
  position.y       = double_param_value("position_y");    
  speed.x          = double_param_value("vitesse_x");  
  speed.y          = double_param_value("vitesse_y");  
  angle            = double_param_value("angle")*M_PI/180.;  
  omega            = double_param_value("omega")*M_PI/180.;
  coeff_frott_t    = double_param_value("coeff_frott_t");
  coeff_frott_n    = double_param_value("coeff_frott_n");
  rayon_roue       = double_param_value("rayon_roue");
  tension          = double_param_value("tension_batteries");   
  capacite_max     = double_param_value("capacite_batteries")*3600.;     
  capacite         = capacite_max;
  periodes_codeuse = double_param_value("periodes_codeuse")/(2.*M_PI);
  nbr_pics         = int_param_value("nbr_pic");
  data = new PIC_DATA[nbr_pics];
  
  if(matiere>=NBR_MATIERES) 
  {
    matiere=0;
    printf("Erreur au chargement de l'objet: matière incorrecte, remplacée par du bois...\n");
  }  
  couleur=clBlack;
  maj_const_vars();
  printf("ok\n");
  fflush(stdout); 
  
  
  char *file;
  
  /* Motors */
  file = string_param_value("params_file_motor");
  for(int i=0;i<2;i++)
  {
    moteur[i].dt=simul_info->dt;
    moteur[i].r=rayon_roue;
    moteur[i].m=masse/2.;
    moteur[i].g=simul_info->g;
    moteur[i].e=simul_info->coeff_frott_roue;
    moteur[i].J=J_roue;
    moteur[i].load_params(file);    
  }
  delete file;
  
  /* Color Captors */  
  nbr_color_captors = int_param_value("nbr_color_captors"); 
  if(nbr_color_captors>0)
  {
    char param_txt[100];
    color_captors.resize(nbr_color_captors);
    for(int i=0;i<nbr_color_captors;i++)
    {
      sprintf(param_txt,"params_file_color_captor%d",i+1);
      file = string_param_value(param_txt);
      color_captors[i].robot=this;
      color_captors[i].load_params(file);      
      delete file;
    }
  }
  
  /* Distance Captors */  
  nbr_dist_captors = int_param_value("nbr_distance_captors"); 
  if(nbr_dist_captors>0)
  {
    char param_txt[100];
    dist_captors.resize(nbr_dist_captors);
    for(int i=0;i<nbr_dist_captors;i++)
    {
      sprintf(param_txt,"params_file_distance_captor%d",i+1);
      file = string_param_value(param_txt);
      dist_captors[i].robot=this;
      dist_captors[i].load_params(file);      
      delete file;
    }
  }
  
  /* Webcams */
  int nbr_webcams = int_param_value("nbr_webcams"); 
  if(nbr_webcams>0)
  {
    char param_txt[100];
    webcams.resize(nbr_webcams);
    for(int i=0;i<nbr_webcams;i++)
    {
      sprintf(param_txt,"params_file_webcam%d",i+1);
      file = string_param_value(param_txt);
      webcams[i].load_params(file);
      webcams[i].create(&N, &T, &G_rot, &position, &z, hauteur); 
      webcams[i].start();
      delete file;
    }
  }  
}
//----------------------------------------------------------------------------
int robot_t::pic_io(robot_t* robot,int captor_type, int captor_id, int value)
{
  switch(captor_type)
  {
    case JACK: return 1; break;
    case CODER: 
      if(captor_id==0 || captor_id==1) 
        return ((int)(robot->moteur[captor_id].a*robot->periodes_codeuse)); 
    break;
    case COUL:
      if(0<=captor_id && captor_id<robot->nbr_color_captors)
        return robot->color_captors[captor_id].measure();
    break;
    case DIST:
      if(0<=captor_id && captor_id<robot->nbr_dist_captors)
        return robot->dist_captors[captor_id].measure();
    break;   
    case MOTORS:
      if(captor_id==0 || captor_id==1) 
        robot->tension_moteur[captor_id]=value;
      return 0;
    break;
    case HPOS:
      if(captor_id != -1)
        robot->clamp_X=value;
      else
      {
        if(fabs(robot->objects_pos[3].x - robot->clamp_minX)<0.001) return posClampBack;
        if(fabs(robot->objects_pos[3].x - robot->clamp_midX)<0.001) return posClampMiddle;
        if(fabs(robot->objects_pos[3].x - robot->clamp_maxX)<0.001) return posClampFront;   
        return -1;             
      }  
    break;
    case VPOS:
      if(captor_id != -1)    
        robot->clamp_Z=robot->clamp_minZ + ((double)value)/1000.;
      else
        return ((int)((((union_obj_t*)robot->objects[3])->objects[1]->z - ((union_obj_t*)robot->objects[3])->objects[1]->hauteur/2. - robot->clamp_minZ) * 1000.));     
    break;    
    case BELT:     
        robot->tension_courroie=value;
    break;
    case CLAMP:
    if(captor_id == -1) 
      return (((union_obj_t*)robot->objects[3])->objects[1]->type==OBJ_PINCE_OUVERTE)?stClampOpen:stClampClosed;
    else if(value == stClampOpen)
      robot->open_clamp();  
    else
      robot->close_clamp();
    break;
  }
  return -1;
}
//----------------------------------------------------------------------------
void robot_t::calc_forces()
{
  vector_t R[2],F;
  double force_value;

  R[0]=roue[0].rotate_spec(cosinus,sinus);
  R[1]=roue[1].rotate_spec(cosinus,sinus); 
  
  move_clamp();
  move_palets();
  calc_frottements();

/* Antidérapage de le pic */
/*  
  double U[2];
  double Umax[2];
  double Umin[2];  
  for(int i=0;i<2;i++)  
  {
    U[i] = ((double)tension_moteur[i])*tension/((double)RANGE_MOTOR);
    Umax[i] = moteur[i].w/moteur[i].Kv+(moteur[i].m*moteur[i].g*moteur[i].e*rayon_roue)/(1.5*moteur[i].Kc*moteur[i].k*moteur[i].n*0.98);
    Umin[i] = moteur[i].w/moteur[i].Kv-(moteur[i].m*moteur[i].g*moteur[i].e*rayon_roue)/(1.5*moteur[i].Kc*moteur[i].k*moteur[i].n*0.98);    
  }
  
  for(int i=0;i<2;i++)  
  {  
    if(U[i]>Umax[i])
      U[i] = Umax[i];
    if(U[i]<Umin[i])
      U[i] = Umin[i];
  }
*/
  
  for(int i=0;i<2;i++)
  {
    force_value=moteur[i].calc_force(((double)tension_moteur[i])*tension/((double)RANGE_MOTOR),get_speed(R[i])|N);
    F = N*force_value;
    add_force(R[i],F);
    capacite-=fabs(moteur[i].I)*simul_info->dt; 
  }
}
//----------------------------------------------------------------------------
void robot_t::move_clamp()
{
  /* Horizontal move */
  double x_obj;
  switch(clamp_X)
  {
    case posClampBack:   x_obj = clamp_minX; break;
    case posClampMiddle: x_obj = clamp_midX; break;
    case posClampFront:  x_obj = clamp_maxX; break;        
  }
  double dx = h_speed_move * simul_info->dt;
  if(fabs(objects_pos[3].x - x_obj)<dx)
  {
    objects_pos[3].x = x_obj;
    objects[3]->speed.x=0.;
    objects[3]->speed.y=0.;    
  }
  else
  {
    if(objects_pos[3].x > x_obj) dx = -dx;
    objects_pos[3].x += dx;       
    objects_last_pos[3].x += dx;                           
    objects[3]->speed=N*dx;                        
  }

  /*Vertical move */
  if(clamp_Z < clamp_minZ) clamp_Z = clamp_minZ;
  if(clamp_Z > clamp_maxZ) clamp_Z = clamp_maxZ;
  if(rand() > (RAND_MAX * proba_miss_step))
  {
    object_t *clamp = ((union_obj_t*)objects[3])->objects[1];
    double dh = v_speed_move * simul_info->dt;
    double h = clamp->z-((circle_obj_t*)clamp)->hauteur/2.;
    if(fabs(h-clamp_Z)<dh)
      clamp->z=clamp->hauteur/2.+clamp_Z;
    else
    {
      if(h>clamp_Z) dh = -dh;
      clamp->z += dh;               
      for(unsigned int i=0; i<element_in_clamp.size(); i++)
        element_in_clamp[i]->z += dh;
    }
  }
    
  objects[3]->G_init=G_init-objects_pos[3]; 
  objects[3]->maj_const_vars();
}
//----------------------------------------------------------------------------
void robot_t::open_clamp()
{   
  ((union_obj_t*)objects[3])->objects[1]->type=OBJ_PINCE_OUVERTE;
  ((union_obj_t*)objects[3])->objects[1]->couleur=getColorSDL(clGray);
  
  for(unsigned int i=0; i<element_in_clamp.size(); i++)
  {
    simul_info->rattach_palet(element_in_clamp[i]);    
    ((union_obj_t*)objects[3])->detach_object(element_in_clamp[i]);
    element_in_clamp[i]->position -= element_in_clamp[i]->G_rot;
    element_in_clamp[i]->G_init.y = 0.;        
    element_in_clamp[i]->G_init.x = 0.;
    element_in_clamp[i]->G_init.y = 0.;    
    element_in_clamp[i]->G_rot.x = 0.;
    element_in_clamp[i]->G_rot.y = 0.;        
    element_in_clamp[i]->set_free();
  }
  
  element_in_clamp.clear();
}
//----------------------------------------------------------------------------
void robot_t::close_clamp()
{
  object_t *clamp = ((union_obj_t*)objects[3])->objects[1];
  vector_t clamp_pos = clamp->position -
                       clamp->G_rot;
  double r2 = ((circle_obj_t*)clamp)->rayon;
  r2 *= r2;
  vector_t pos = ((union_obj_t*)objects[3])->objects_pos[1];
  
  for(unsigned int i=0; i<simul_info->palets.size(); i++)
    if((simul_info->palets[i]->position - clamp_pos).norme2() < r2 &&
       clamp->z-clamp->hauteur/2. < simul_info->palets[i]->z+simul_info->palets[i]->hauteur/2. &&
       clamp->z+clamp->hauteur/2. > simul_info->palets[i]->z-simul_info->palets[i]->hauteur/2.)
    {
      ((palet_t*)(simul_info->palets[i]))->set_taken();
      element_in_clamp.resize(element_in_clamp.size()+1);
      element_in_clamp[element_in_clamp.size()-1] = (palet_t*)simul_info->palets[i];
      ((union_obj_t*)objects[3])->attach_object(simul_info->palets[i],pos);
      simul_info->palets[i]->G_init=G_init-pos;       
      simul_info->detach_palet(simul_info->palets[i]);
    }
    
  clamp->type=OBJ_PINCE_FERMEE;
  clamp->couleur=getColorSDL(clDarkGray);    
}
//----------------------------------------------------------------------------
void robot_t::move_palets()
{
  vector_t C;
  vector_t F;
  double d;
  double v=(33./6.)*((double)tension_courroie)*(tension/12)*M_PI*0.022;
  double modif;
  object_t *clamp = ((union_obj_t*)objects[3])->objects[1];  
  palet_t *palet;
  vector< palet_t* > palets_in;
  palets_in.clear();
  
  for(unsigned int i=0;i<simul_info->palets.size();i++)
  {
    palet=((palet_t*)simul_info->palets[i]);
    if(palet->z-palet->hauteur/2. != 0.) continue;    
    C=(palet->position-position).rotate_spec(cosinus,-sinus)+G_init;
    if(C.y>-0.04 && C.y<0.04 && C.x<dimX/2. && C.x>(dimX/2.-longueur_recept))
    {
      palets_in.resize(palets_in.size()+1);
      palets_in[palets_in.size()-1] = palet;
    }
  }
    
  modif=(v<0?0.05 + 0.07*(palets_in.size()-1):0.);
  if(clamp->type==OBJ_PINCE_FERMEE && clamp->z-clamp->hauteur/2.-clamp_minZ<0.03)
    modif += 0.07;
    
  for(unsigned int i=0;i<palets_in.size();i++)
  {
    C=(palets_in[i]->position-position).rotate_spec(cosinus,-sinus)+G_init;
    if(C.y>-0.04 && C.y<0.04 && C.x<dimX/2. && C.x>(dimX/2.-longueur_recept+modif))
    {
    /*  s=((2.*(speed|N)+v)/2.-(palets_in[i]->speed|N))*palets_in[i]->masse/simul_info->dt;
      d=(v/(2.*palets_in[i]->rayon)+palets_in[i]->omega)*palets_in[i]->J/(palets_in[i]->rayon*simul_info->dt);
      C=T*palets_in[i]->rayon;
      F=N*(s+d)/2.;
      palets_in[i]->add_force(C,F);
      C=C+palets_in[i]->position-position;
      F=-F;
      add_force(C,F);           
      C=-T*palets_in[i]->rayon;
      F=N*(s-d)/2.;
      palets_in[i]->add_force(C,F);
      C=C+palets_in[i]->position-position;
      F=-F;
      add_force(C,F);  */  
      d = (speed|N) + v - (palets_in[i]->speed|N);
      C = vector_t(0., 0.);
      F = N * (d * palets_in[i]->masse/simul_info->dt);
      palets_in[i]->add_force(C,F);       
      F = -F;
      add_force(C,F);      
    }
  }
}
//----------------------------------------------------------------------------
void robot_t::calc_frottements()
{
  vector_t O,F;
  F = N*(-coeff_frott_n*(speed|N));
  add_force(O,F);  
  F = T*(-coeff_frott_t*(speed|T)*masse/simul_info->dt);  
  add_force(O,F);
}
//----------------------------------------------------------------------------
void robot_t::draw()
{
  vector_t _N,_T,u,v;
  point_t point[4];
  Uint32 C;
  double demi_largeur_roue=0.005;
  int i;
  if(nbr_pics == 2) i = 1; else i = 0;
  // Robot supposé
  _N.x=cos(data[i].angle);
  _N.y=sin(data[i].angle);
  _T.x=-sin(data[i].angle);
  _T.y=cos(data[i].angle);
  v=G_init.rotate(data[i].angle);
  point[0].x=((int)((data[i].posX+_N.x*dimX/2.+_T.x*dimY/2.-v.x)*simul_info->scale));     
  point[0].y=((int)((data[i].posY+_N.y*dimX/2.+_T.y*dimY/2.-v.y)*simul_info->scale));
  point[1].x=((int)((data[i].posX+_N.x*dimX/2.-_T.x*dimY/2.-v.x)*simul_info->scale));
  point[1].y=((int)((data[i].posY+_N.y*dimX/2.-_T.y*dimY/2.-v.y)*simul_info->scale));
  point[2].x=((int)((data[i].posX-_N.x*dimX/2.-_T.x*dimY/2.-v.x)*simul_info->scale));
  point[2].y=((int)((data[i].posY-_N.y*dimX/2.-_T.y*dimY/2.-v.y)*simul_info->scale));
  point[3].x=((int)((data[i].posX-_N.x*dimX/2.+_T.x*dimY/2.-v.x)*simul_info->scale));
  point[3].y=((int)((data[i].posY-_N.y*dimX/2.+_T.y*dimY/2.-v.y)*simul_info->scale)); 
  PolylineSDL(point, 4, getColorSDL(clWhite));
  LigneSDL(point[2].x, point[2].y, ((int)(data[i].posX*simul_info->scale)), ((int)(data[i].posY*simul_info->scale)), getColorSDL(clWhite)); 
  LigneSDL(point[3].x, point[3].y, ((int)(data[i].posX*simul_info->scale)), ((int)(data[i].posY*simul_info->scale)), getColorSDL(clWhite));
  // Robot  
  for(unsigned int i=0; i<objects.size(); i++)
    objects[i]->draw();   
  // Destination
  _N.x=cos(data[i].destA);
  _N.y=sin(data[i].destA);
  _T.x=-sin(data[i].destA)*0.03;
  _T.y=cos(data[i].destA)*0.03; 
  LigneSDL(((int)(simul_info->scale*data[i].destX)), ((int)(simul_info->scale*data[i].destY)), 
           ((int)(simul_info->scale*(data[i].destX+_N.x*0.1))), ((int)(simul_info->scale*(data[i].destY+_N.y*0.1))), getColorSDL(clBlack));
  LigneSDL(((int)(simul_info->scale*(data[i].destX+_N.x*0.1))), ((int)(simul_info->scale*(data[i].destY+_N.y*0.1))), 
           ((int)(simul_info->scale*(data[i].destX+_N.x*0.07+_T.x))), ((int)(simul_info->scale*(data[i].destY+_N.y*0.07+_T.y))), getColorSDL(clBlack));
  LigneSDL(((int)(simul_info->scale*(data[i].destX+_N.x*0.1))), ((int)(simul_info->scale*(data[i].destY+_N.y*0.1))), 
           ((int)(simul_info->scale*(data[i].destX+_N.x*0.07-_T.x))), ((int)(simul_info->scale*(data[i].destY+_N.y*0.07-_T.y))), getColorSDL(clBlack));                      
  // roues
  for(int i=0;i<2;i++)
  {
    u=position+roue[i].rotate_spec(cosinus,sinus);
    v=u+N*rayon_roue+T*demi_largeur_roue;
    v*=simul_info->scale;
    point[0].x=((int)v.x);
    point[0].y=((int)v.y);
    v=u+N*rayon_roue-T*demi_largeur_roue;
    v*=simul_info->scale;    
    point[1].x=((int)v.x);
    point[1].y=((int)v.y);
    v=u-N*rayon_roue-T*demi_largeur_roue;
    v*=simul_info->scale;    
    point[2].x=((int)v.x);
    point[2].y=((int)v.y);
    v=u-N*rayon_roue+T*demi_largeur_roue;
    v*=simul_info->scale;    
    point[3].x=((int)v.x);
    point[3].y=((int)v.y);  
    C=makeColorSDL(moteur[i].w<0?127:0, moteur[i].w>0?127:0, moteur[i].derape?127:0);
    PolylineSDL(point, 4, C);  
    LigneSDL(point[0].x, point[0].y, point[2].x, point[2].y, C);
    LigneSDL(point[1].x, point[1].y, point[3].x, point[3].y, C);    
    LigneSDL((point[0].x+point[1].x)/2, (point[0].y+point[1].y)/2, (point[2].x+point[3].x)/2, (point[2].y+point[3].y)/2, C);
  }
  for(int i=0;i<nbr_color_captors;i++)
    color_captors[i].draw();   
}

//---------------------------------------------------------------------------
