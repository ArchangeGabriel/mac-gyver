#include "../common/comm.h"
#include "the_cup/colors.h"

#include "robot.h"

//----------------------------------------------------------------------------
robot_t::robot_t()
{
  objects.clear();
  ObjBox *robot;
  robot=new ObjBox;
  robot->type=OBJ_ROBOT;
  robot->couleur=clRobot;    
  add_object(robot);

  for(int i=0;i<2;i++)
    tension_moteur[i]=0;
  
  webcams.clear();
  color_captors.clear();
  dist_captors.clear();  
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
  
  z = surelev + hauteur/2.;
  ((ObjBox*)objects[0])->dimX=dimX;
  ((ObjBox*)objects[0])->dimY=dimY;
  ((ObjBox*)objects[0])->hauteur=hauteur;  
  ((ObjBox*)objects[0])->z=z;    
  objects_pos[0].x=0.;  
  objects_pos[0].y=0.;

  position.x       = double_param_value("position_x");  
  position.y       = double_param_value("position_y");
  angle            = double_param_value("angle")*M_PI/180.;  
  position = position + G_init.rotate(angle);
  speed.x          = double_param_value("vitesse_x");  
  speed.y          = double_param_value("vitesse_y");
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
    case MSG_MOTORS:
      if(captor_id==0 || captor_id==1) 
        robot->tension_moteur[captor_id]=double(value-128)/127.;
      return 0;
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
  
  calc_frottements();

  for(int i=0;i<2;i++)
  {
    force_value=moteur[i].calc_force(tension_moteur[i]*tension,get_speed(R[i])|N);
    F = N*force_value;
    add_force(R[i],F);
    capacite-=fabs(moteur[i].I)*simul_info->dt; 
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
  point[0].x=((int)((data[i].posX+_N.x*dimX/2.+_T.x*dimY/2.)*simul_info->scale));     
  point[0].y=((int)((data[i].posY+_N.y*dimX/2.+_T.y*dimY/2.)*simul_info->scale));
  point[1].x=((int)((data[i].posX+_N.x*dimX/2.-_T.x*dimY/2.)*simul_info->scale));
  point[1].y=((int)((data[i].posY+_N.y*dimX/2.-_T.y*dimY/2.)*simul_info->scale));
  point[2].x=((int)((data[i].posX-_N.x*dimX/2.-_T.x*dimY/2.)*simul_info->scale));
  point[2].y=((int)((data[i].posY-_N.y*dimX/2.-_T.y*dimY/2.)*simul_info->scale));
  point[3].x=((int)((data[i].posX-_N.x*dimX/2.+_T.x*dimY/2.)*simul_info->scale));
  point[3].y=((int)((data[i].posY-_N.y*dimX/2.+_T.y*dimY/2.)*simul_info->scale)); 
  PolylineSDL(point, 4, clWhite);
  LigneSDL(point[2].x, point[2].y, ((int)(data[i].posX*simul_info->scale)), ((int)(data[i].posY*simul_info->scale)), clWhite); 
  LigneSDL(point[3].x, point[3].y, ((int)(data[i].posX*simul_info->scale)), ((int)(data[i].posY*simul_info->scale)), clWhite);
  // Robot  
  for(unsigned int i=0; i<objects.size(); i++)
    objects[i]->draw();   
  // Destination
  _N.x=cos(data[i].destA);
  _N.y=sin(data[i].destA);
  _T.x=-sin(data[i].destA)*0.03;
  _T.y=cos(data[i].destA)*0.03; 
  LigneSDL(((int)(simul_info->scale*data[i].destX)), ((int)(simul_info->scale*data[i].destY)), 
           ((int)(simul_info->scale*(data[i].destX+_N.x*0.1))), ((int)(simul_info->scale*(data[i].destY+_N.y*0.1))), clBlack);
  LigneSDL(((int)(simul_info->scale*(data[i].destX+_N.x*0.1))), ((int)(simul_info->scale*(data[i].destY+_N.y*0.1))), 
           ((int)(simul_info->scale*(data[i].destX+_N.x*0.07+_T.x))), ((int)(simul_info->scale*(data[i].destY+_N.y*0.07+_T.y))), clBlack);
  LigneSDL(((int)(simul_info->scale*(data[i].destX+_N.x*0.1))), ((int)(simul_info->scale*(data[i].destY+_N.y*0.1))), 
           ((int)(simul_info->scale*(data[i].destX+_N.x*0.07-_T.x))), ((int)(simul_info->scale*(data[i].destY+_N.y*0.07-_T.y))), clBlack);                      
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
