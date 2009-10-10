#include "object.h"

//----------------------------------------------------------------------------//
//                                                                            //
//             Classe de base pour les paramètres de simulation               //
//                                                                            //
//----------------------------------------------------------------------------//
simul_info_t::simul_info_t()
{
  files_robot=NULL;
  simul_3D_view_file=NULL;
  reset();
}
//----------------------------------------------------------------------------
simul_info_t::~simul_info_t()
{
  if(files_robot) 
  {
    for(int i=0;i<nbr_robots;i++)
      delete files_robot[i];
    delete files_robot;
    files_robot=NULL;     
  } 
}
//----------------------------------------------------------------------------
void simul_info_t::init_params()
{
  dt               = double_param_value("dt")*0.001;
  scale            = double_param_value("scale");   
  config_terrain   = int_param_value("config_terrain");  
  nbr_robots       = int_param_value("nbr_robots"); 
  coeff_frott[mBois][mBois]   = double_param_value("coeff_frott_bois_bois");
  coeff_frott[mMetal][mBois]  = double_param_value("coeff_frott_bois_metal");
  coeff_frott[mBois][mMetal]  = double_param_value("coeff_frott_bois_metal");
  coeff_frott[mMetal][mMetal] = double_param_value("coeff_frott_metal_metal");       
  coeff_frott_roue            = double_param_value("coeff_frott_roue");    
  g                           = double_param_value("champ_gravitation");
  simul_3D_view_file          = string_param_value("3DView_position");
   
  if(nbr_robots>0)
  {
    files_robot = new char*[nbr_robots];
    char param_txt[100];
    for(int i=0;i<nbr_robots;i++)
    {
      sprintf(param_txt,"params_file_robot%d",i+1);
      files_robot[i] = string_param_value(param_txt);
    }
  }  
}
//----------------------------------------------------------------------------
void simul_info_t::add_palet(object_t *pal)
{
  palets.resize(palets.size()+1); 
  palets[palets.size()-1]=pal;
  add_object(pal);
}
//----------------------------------------------------------------------------
void simul_info_t::detach_palet(object_t *pal)
{
  pal->detached = true;    
}
//----------------------------------------------------------------------------
void simul_info_t::rattach_palet(object_t *pal)
{
  pal->detached = false; 
}
//----------------------------------------------------------------------------
void simul_info_t::add_object(object_t *obj)
{
  obj->my_id=nbr_obj;
  nbr_obj++;
  coll_checked.resize(nbr_obj-1);
  for(int i=0;i<nbr_obj-1;i++)
     coll_checked[i].resize(i+1);  
  clean_coll_checked();
  if(obj->fixe)
  {
    objets_fixes.resize(objets_fixes.size()+1);
    objets_fixes[objets_fixes.size()-1]=obj;
  }
  else
  {
    objets_mobiles.resize(objets_mobiles.size()+1);
    objets_mobiles[objets_mobiles.size()-1]=obj;
  } 
  objets.resize(objets.size()+1);
  objets[objets.size()-1]=obj;
  obj->TTL=rand()%_TTL;
}
//----------------------------------------------------------------------------
void simul_info_t::reset()
{
  if(files_robot) {delete(files_robot); files_robot=NULL;} 
  if(simul_3D_view_file) {delete simul_3D_view_file; simul_3D_view_file=NULL;}
  files_robot=NULL;
  Iterations=0;
  robot_proceed=0;
  nbr_obj=0;
  objets_fixes.resize(0);  
  objets_mobiles.resize(0);  
  objets.resize(0);  
  palets.resize(0);
  coll_checked.resize(0);  
  param_list.clear();
}
//----------------------------------------------------------------------------
inline void simul_info_t::set_coll_checked(int i,int j,bool is_coll)
{
  if(i<j)
  {
    if(i==-1) return;
    coll_checked[j-1][i]=is_coll;  
  }
  else
  {
    if(j==-1) return;
    coll_checked[i-1][j]=is_coll;  
  }  
}
//----------------------------------------------------------------------------
inline void simul_info_t::reset_coll_checked(int i,int j)
{
  if(i<j)
  {
    if(i==-1) return;
    coll_checked[j-1][i]=false;  
  }
  else
  {
    if(j==-1) return;
    coll_checked[i-1][j]=false;  
  }  
}
//----------------------------------------------------------------------------
inline bool simul_info_t::is_coll_checked(int i,int j)
{
  if(i<j)
  {
    if(i==-1) return false;  
    return coll_checked[j-1][i];
  }
  else
  {
    if(j==-1) return false;  
    return coll_checked[i-1][j];        
  }
}
//----------------------------------------------------------------------------
void simul_info_t::clean_coll_checked()
{  
  vector<bool> is_coll;
  is_coll.resize(nbr_obj);
  for(int i=0;i<nbr_obj;i++)  
    is_coll[i]=false;
    
  for(int i=0;i<nbr_obj-1;i++)
    for(int j=0;j<i+1;j++)
      if(coll_checked[i][j])
      {
        coll_checked[i][j]=false;  
        is_coll[i+1]=true;
        is_coll[j]=true;
      }

  for(int i=0;i<nbr_obj;i++)  
    if(is_coll[i])
    {
      objets[i]->TTL=0;
      objets[i]->calc_frottements();
      objets[i]->calc_vitesse(); 
      objets[i]->reset_last_pos();     
      objets[i]->calc_position(); 
    }
    
  for(int i=0;i<nbr_obj;i++)  
    if(is_coll[i]) 
    {    
      if(objets[i]->check_collisions())
      {
        objets[i]->calc_frottements();
        objets[i]->calc_vitesse(); 
        objets[i]->reset_last_pos();     
        objets[i]->calc_position(); 
      }
      objets[i]->set_last_pos();        
    } 

  for(int i=0;i<nbr_obj-1;i++)
    for(int j=0;j<i+1;j++)
      coll_checked[i][j]=false;  
}
//----------------------------------------------------------------------------
void simul_info_t::clean_coll_of(int i)
{
  if(i==-1) return;
  for(int j=0;j<i;j++)
    coll_checked[i-1][j]=false;
  for(int j=i+1;j<nbr_obj;j++)
    coll_checked[j-1][i]=false;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------//
//                                                                            //
//                         Classe de base pour les objets                     //
//                                                                            //
//----------------------------------------------------------------------------//
object_t::object_t()
{
  hide=false;
  fixe=false;
  linked=false;
  inverse=false;
  detached = false;
 
  angle=0.;
  omega=0.;
  last_angle=0.;
  cosinus=1.;
  sinus=0.;

  speed.x=0;
  speed.y=0;  
  omega=0;
  
  sum_forces.x=0.;
  sum_forces.y=0.;
  sum_moments=0.;
  
  TTL=0;
  
  texture=-1;
}
//---------------------------------------------------------------------------  
object_t::~object_t()
{      
}
//---------------------------------------------------------------------------
void object_t::reset_last_pos(bool link_ignore)
{
  if(linked && !link_ignore)
    linked_obj->reset_last_pos();
  else
  {
    position=last_position;
    angle=last_angle;  
  }
}
//---------------------------------------------------------------------------
void object_t::set_last_pos(bool link_ignore)
{
  if(linked && !link_ignore)
    linked_obj->set_last_pos();
  else
  {
    last_position=position;
    last_angle=angle;
  }
}
//---------------------------------------------------------------------------
vector_t object_t::vitesse()
{
  if(linked)
    return linked_obj->vitesse()+speed;
  else
    return speed;
}
//---------------------------------------------------------------------------
void object_t::init_params()
{
  G_init.x   = double_param_value("centre_gravite_x");  
  G_init.y   = double_param_value("centre_gravite_y");  
  masse      = double_param_value("masse");
  J          = double_param_value("moment");  
  matiere    = int_param_value("matiere");
  position.x = double_param_value("position_x");  
  position.y = double_param_value("position_y");
  speed.x    = double_param_value("vitesse_x");      
  speed.y    = double_param_value("vitesse_y");        
  angle      = double_param_value("angle")*M_PI/180.;    omega      = double_param_value("omega")*M_PI/180.;
  
  if(matiere>=NBR_MATIERES) 
  {
    matiere=0;
    printf("Erreur au chargement de l'objet: matière incorrecte, remplacée par du bois...\n");
  }
  maj_const_vars();
}
//---------------------------------------------------------------------------
void object_t::maj_const_vars()
{
  last_position=position;
  last_angle=angle; 
}
//---------------------------------------------------------------------------
void object_t::maj_dyn_vars()
{
  cosinus=cos(angle);
  sinus=sin(angle);
  N.x=cosinus;
  N.y=sinus;
  T.x=-sinus;
  T.y=cosinus;
  G_rot=G_init.rotate_spec(cosinus,sinus);
}
//---------------------------------------------------------------------------
void object_t::add_force(vector_t &point, vector_t &force) 
{
  if(!fixe)
  {
    if(linked)
      linked_obj->add_force(point,force);
    else
    {
      //Théorème de la résultante cinétique
      sum_forces+=force;
      //Théorème du moment
      sum_moments+=point^force;    
    }
  }  
}
//---------------------------------------------------------------------------
bool object_t::check_collisions()
{
  if(detached) return false;
  bool is_coll=false;
  bool coll;
  if(!(fixe && inverse))
  {
    if(fixe)
    {
      for (unsigned int i=0;i<simul_info->objets_mobiles.size(); i++)
        if(!simul_info->objets_mobiles[i]->detached && !simul_info->is_coll_checked(my_id,simul_info->objets_mobiles[i]->my_id))
        {
          coll=check_collision_with(simul_info->objets_mobiles[i]);
          simul_info->set_coll_checked(my_id,simul_info->objets_mobiles[i]->my_id,coll);
          is_coll=is_coll || coll;
        }
    }
    else
    {
      for (unsigned int i=0;i<simul_info->objets_mobiles.size(); i++)
        if(!simul_info->objets_mobiles[i]->detached &&  simul_info->objets_mobiles[i]!=this)
          if(!simul_info->is_coll_checked(my_id,simul_info->objets_mobiles[i]->my_id))
          {
            coll=check_collision_with(simul_info->objets_mobiles[i]);
            simul_info->set_coll_checked(my_id,simul_info->objets_mobiles[i]->my_id,coll);
            is_coll=is_coll || coll;
          }                                         
      for (unsigned int i=0;i<simul_info->objets_fixes.size(); i++)  
      {
        if(!simul_info->objets_fixes[i]->detached &&  !simul_info->is_coll_checked(my_id,simul_info->objets_fixes[i]->my_id))
        {
          coll=check_collision_with(simul_info->objets_fixes[i]);
          simul_info->set_coll_checked(my_id,simul_info->objets_fixes[i]->my_id,coll);
          is_coll=is_coll || coll;
        }                                       
      }           
    }
  }
  return is_coll;
}
//---------------------------------------------------------------------------
void object_t::calc_forces()
{
}
//---------------------------------------------------------------------------
void object_t::calc_frottements()
{
}
//---------------------------------------------------------------------------
void object_t::calc_vitesse()
{
  if(!fixe)
  {
    if(linked)
      linked_obj->calc_vitesse();
    else
    { 
      sum_forces/=masse;
      sum_moments/=J;  
      speed+=(sum_forces*simul_info->dt);
      omega+=(sum_moments*simul_info->dt);
      sum_forces.x=0.;
      sum_forces.y=0.;
      sum_moments=0.;    
    }
  } 
}
//---------------------------------------------------------------------------
void object_t::calc_position()
{
  if(!fixe)
  {
    if(linked)
      linked_obj->calc_position();
    else
    {
      last_position=position;
      last_angle=angle;
      position+=speed*simul_info->dt;  
      angle+=omega*simul_info->dt;
      maj_dyn_vars();
    }
  }
}
//---------------------------------------------------------------------------
bool object_t::check_collision_with(object_t* obj)
{
  // On peut décider rapidement si la collision n'a pas lieu
  bool is_coll;
  if(obj->inverse)
    is_coll=obj->fast_collision_check(this);
  else
    is_coll=fast_collision_check(obj);
  if(!is_coll)
    return false;  
  // la décision rapide a échouée, analyse plus fine  
  if(slow_collision_check(obj))
    return true;    
  return obj->slow_collision_check(this);
}
//---------------------------------------------------------------------------
bool object_t::fast_collision_check(object_t*)
{
  return true;
}
//---------------------------------------------------------------------------
bool object_t::slow_collision_check(object_t*)
{
  return false;
}
//---------------------------------------------------------------------------
bool object_t::calc_impulsion(vector_t &N_vect, vector_t &p1, object_t *obj,vector_t &p2)
{
  if((obj->z-obj->hauteur/2.) >= (z + hauteur/2.))
    return false;
  if((z-hauteur/2.) >= (obj->z + obj->hauteur/2.))
    return false;         
  vector_t F;
  const vector_t T_vect = N_vect.rotate_spec(0.,1.);
  const vector_t diff_speed = obj->get_speed(p2) - get_speed(p1);
//  printf("type:me,obj (%d,%d)\n",type,obj->type); 
//  printf("id,obj (%d,%d)\n",my_id,obj->my_id); 
//  printf("p1(%f,%f)\n",get_speed(p1).x,get_speed(p1).y);
//  printf("p2(%f,%f)\n",obj->get_speed(p2).x,obj->get_speed(p2).y);  
//  printf("N(%f,%f)\n",N_vect.x,N_vect.y);  
//  printf("col (%f)\n",(diff_speed|N_vect));
  if((diff_speed|N_vect)<=-0.03) return false;   
  double kn = (fixe?0.:(1/masse + pow(p1^N_vect,2)/J))   +   (obj->fixe?0.:(1/obj->masse + pow(p2^N_vect,2)/obj->J));
  double fn  = (simul_info->coeff_frott[matiere][obj->matiere])*((diff_speed|N_vect)+0.03)/kn;
  double kt = (fixe?0.:(1/masse + pow(p1^T_vect,2)/J))   +   (obj->fixe?0.:(1/obj->masse + pow(p2^T_vect,2)/obj->J));
  double ft  = (diff_speed|T_vect)/kt;
  
  if(fabsl(ft)>fabsl(fn)*simul_info->coeff_frott[matiere][obj->matiere])
  { 
    if(ft>0)
      ft=fabsl(fn)*simul_info->coeff_frott[matiere][obj->matiere];
    else
      ft=-fabsl(fn)*simul_info->coeff_frott[matiere][obj->matiere];
  }
   
  F = (N_vect*fn+T_vect*ft)/simul_info->dt; 
  add_force(p1,F);
  F = -F;
  obj->add_force(p2,F); 
/*  calc_vitesse();
  obj->calc_vitesse();   
  calc_position();
  obj->calc_position();    
  calc_frottements();
  obj->calc_frottements();
  calc_vitesse();
  obj->calc_vitesse(); 
  reset_last_pos();
  obj->reset_last_pos();     
  calc_position();
  obj->calc_position();      
  set_last_pos();
  obj->set_last_pos();  *//*
//----- <Contestable>
  fn  = (1.+simul_info->coeff_frott[matiere][obj->matiere])*((obj->get_speed(p2)-get_speed(p1))|N_vect)/kn;
  f1.C=p1;
  f1.F=(N_vect*fn)/simul_info->dt;
  f2.C=p2;
  f2.F=-f1.F;
  add_force(f1);
  obj->add_force(f2);
  calc_vitesse();
  obj->calc_vitesse();  printf("fn: %lf, ft : %lf\n",fn,ft);
//----- </Contestable>*/

 /* printf("\nme: %d, obj : %d\n",my_id, obj->my_id);
  printf("diff_speed: (%f,%f)\n",diff_speed.x,diff_speed.y);  
  printf("diff_speed|N_vect: %f\n",diff_speed|N_vect);
  printf("vit(p1): (%f,%f)\n",get_speed(p1).x,get_speed(p1).y);
  printf("vit(p2): (%f,%f)\n",obj->get_speed(p2).x,obj->get_speed(p2).y);  
  printf("N (%f,%f)\n",N_vect.x,N_vect.y);
  printf("kn: %f, kt : %f\n",kn,kt);
  printf("fn: %f, ft : %f\n",fn,ft);
  fflush(stdout); */
  return true;
}
//---------------------------------------------------------------------------
double object_t::distance_to(vector_t &, vector_t &, double)
{
  return -1.;
}
//---------------------------------------------------------------------------
void object_t::draw() 
{
}
//---------------------------------------------------------------------------
void object_t::draw3D() 
{
}
//---------------------------------------------------------------------------
vector_t object_t::get_speed(vector_t &point)
{
  if(fixe)
  {
    vector_t s;
    return s;
  }
  else
  {
    vector_t s=vitesse();
    s.x-=point.y*omega;
    s.y+=point.x*omega;
    return s;
  }
}
//---------------------------------------------------------------------------
void object_t::get_overlapping_interval(vector_t&, vector_t&, double*, double*)
{
}
//---------------------------------------------------------------------------
bool object_t::is_point_in_obj(vector_t&, object_t*&)
{
  return true;
}
//---------------------------------------------------------------------------
double object_t::dist_of_obj(vector_t&, vector_t&, object_t*&)
{
  return 0.;
}
//---------------------------------------------------------------------------
vector_t object_t::get_normal(vector_t &)
{
  vector_t n;
  return n;
}
//---------------------------------------------------------------------------

//----------------------------------------------------------------------------//
//                                                                            //
//                                      Pavé                                  //
//                                                                            //
//----------------------------------------------------------------------------//
ObjBox::~ObjBox()
{
}
//---------------------------------------------------------------------------
void ObjBox::init_params()
{
  G_init.x   = double_param_value("centre_gravite_x");  
  G_init.y   = double_param_value("centre_gravite_y");  
  masse      = double_param_value("masse");
  J          = double_param_value("moment");  
  matiere    = int_param_value("matiere");
  position.x = double_param_value("position_x");  
  position.y = double_param_value("position_y");
  speed.x    = double_param_value("vitesse_x");      
  speed.y    = double_param_value("vitesse_y");        
  angle      = double_param_value("angle")*M_PI/180.;    omega      = double_param_value("omega")*M_PI/180.;
  dimX       = double_param_value("dimension_x");
  dimY       = double_param_value("dimension_y");
  hauteur    = double_param_value("hauteur"); 
  
  if(matiere>=NBR_MATIERES) 
  {
    matiere=0;
    printf("Erreur au chargement de l'objet: matière incorrecte, remplacée par du bois...\n");
  }       
  
  maj_const_vars();
}
//---------------------------------------------------------------------------
void ObjBox::maj_const_vars()
{
  last_position=position;
  last_angle=angle;
  coins[0].x=dimX/2.;
  coins[0].y=dimY/2.;
  coins[1].x=dimX/2.;
  coins[1].y=-dimY/2.;
  coins[2].x=-dimX/2.;
  coins[2].y=-dimY/2.;
  coins[3].x=-dimX/2.;
  coins[3].y=dimY/2.;
  for(int i=0;i<4;i++)
  {
    coins[i]-=G_init;
    coins_rot[i]=coins[i].rotate(angle);
  }  
  maj_dyn_vars();
}
//---------------------------------------------------------------------------
void ObjBox::maj_dyn_vars()
{
  cosinus=cos(angle);
  sinus=sin(angle);
  N.x=cosinus;
  N.y=sinus;
  T.x=-sinus;
  T.y=cosinus;
  G_rot=G_init.rotate_spec(cosinus,sinus);    
  for(int i=0;i<4;i++)
    coins_rot[i]=coins[i].rotate_spec(cosinus,sinus);
}
//---------------------------------------------------------------------------
void ObjBox::draw()
{
  vector_t coins_pos[4];
  point_t point[4];
  for(int i=0;i<4;i++)
  {
    coins_pos[i]=coins_rot[i]+position;
    coins_pos[i]*=simul_info->scale;
    point[i].x=((int)coins_pos[i].x);
    point[i].y=((int)coins_pos[i].y);
  } 
  PolylineSDL(point, 4, couleur);  
}
//---------------------------------------------------------------------------
void ObjBox::draw3D() 
{  
  GLfloat B = ((GLfloat)(couleur & 255))/255.;
  GLfloat G = ((GLfloat)((couleur>>8) & 255))/255.;
  GLfloat R = ((GLfloat)((couleur>>16) & 255))/255.;  

  glColor3f(R,G,B); 

  GLfloat spec[] = {0.3,0.3,0.3,1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 20);
  
  GLfloat cz = -z; 
  GLfloat dz = hauteur/2.;      
  
  GLfloat x[8],y[8];
  GLfloat z[8]={cz+dz,cz+dz,cz+dz,cz+dz,cz-dz,cz-dz,cz-dz,cz-dz};
  
  for(int i=0;i<4;i++)
  {
    x[i] = x[i+4] = coins_rot[i].x+position.x;
    y[i] = y[i+4] = coins_rot[i].y+position.y;    
  }
    
  glBegin(GL_QUADS); 
  glNormal3f(0., 0., 1.);   
  glVertex3f(x[0],y[0],z[0]); glVertex3f(x[3],y[3],z[3]); glVertex3f(x[2],y[2],z[2]); glVertex3f(x[1],y[1],z[1]); 
  glNormal3f(0., 0., -1.);   
  glVertex3f(x[4],y[4],z[4]); glVertex3f(x[5],y[5],z[5]); glVertex3f(x[6],y[6],z[6]); glVertex3f(x[7],y[7],z[7]); 
  glNormal3f(T.x, T.y, 0);   
  glVertex3f(x[0],y[0],z[0]); glVertex3f(x[4],y[4],z[4]); glVertex3f(x[7],y[7],z[7]); glVertex3f(x[3],y[3],z[3]); 
  glNormal3f(-T.x, -T.y, 0);    
  glVertex3f(x[1],y[1],z[1]); glVertex3f(x[2],y[2],z[2]); glVertex3f(x[6],y[6],z[6]); glVertex3f(x[5],y[5],z[5]); 
  glNormal3f(N.x, N.y, 0);   
  glVertex3f(x[1],y[1],z[1]); glVertex3f(x[5],y[5],z[5]); glVertex3f(x[4],y[4],z[4]); glVertex3f(x[0],y[0],z[0]); 
  glNormal3f(-N.x, -N.y, 0);  
  glVertex3f(x[3],y[3],z[3]); glVertex3f(x[7],y[7],z[7]); glVertex3f(x[6],y[6],z[6]); glVertex3f(x[2],y[2],z[2]);

  glEnd();
}
//---------------------------------------------------------------------------
bool ObjBox::is_point_in_obj(vector_t &point,object_t* &ss_obj)
{
  ss_obj=this;
  vector_t v=point.rotate_spec(cosinus,-sinus);
  v+=G_init;
  if(inverse) return fabsl(v.x)>dimX/2. || fabsl(v.y)>dimY/2.;
  else return fabsl(v.x)<dimX/2. && fabsl(v.y)<dimY/2.;
}
//---------------------------------------------------------------------------
double ObjBox::dist_of_obj(vector_t &point, vector_t &u,object_t* &ss_obj)
{
  ss_obj=this;
  vector_t v=point.rotate_spec(cosinus,-sinus);
  v+=G_init;
  if(inverse)
  {
    if(fabsl(v.x)<dimX/2. && fabsl(v.y)<dimY/2.)
    {
      double dx,dy;
      dx=(v.x>=0.)?dimX/2.-v.x:dimX/2.+v.x;
      dy=(v.y>=0.)?dimY/2.-v.y:dimY/2.+v.y;
      if(dx<dy)
      {
        if(v.x>=0) u=N; else u=-N;
        return dx;
      }
      else
      {
        if(v.y>=0) u=T; else u=-T;
        return dy;
      }
    }
    else
    {
      u.x=0.;
      u.y=0.;
      return 0.;
    }
  }
  else
  {
    if(fabsl(v.x)<=dimX/2. && fabsl(v.y)<=dimY/2.)
    {
      u.x=0.;
      u.y=0.;
      return 0.;
    }
    else
    {
      if(fabsl(v.x)<=dimX/2.)
      {
        if(v.y>=0) u=-T; else u=T;   
        return fabsl(v.y)-dimY/2.;
      }
      else if(fabsl(v.y)<=dimY/2.)
      {
        if(v.x>=0) u=-N; else u=N;
        return fabsl(v.x)-dimX/2.;
      }
      else
      {
        int c;
        if(v.x>0)
          if(v.y>0) c=0; else c=1;
        else
          if(v.y>0) c=3; else c=2;
          
        u=coins_rot[c]-point;
        double norme=u.norme();
        u=u/norme;
        return norme;
      }
    }
  }
}
//---------------------------------------------------------------------------
void ObjBox::get_overlapping_interval(vector_t &O, vector_t &Dir,double *min, double *max)
{
  double proj1=(position-G_rot-O)|Dir;
  double proj2=fabsl(N|Dir)*dimX/2.+fabsl(T|Dir)*dimY/2.;
  *min=proj1-proj2;
  *max=proj1+proj2;
}
//---------------------------------------------------------------------------
bool ObjBox::fast_collision_check(object_t *obj)
{
  vector_t p;
  double min,max;
  p=position-G_rot;
  if(inverse)
  {
    obj->get_overlapping_interval(p,N,&min,&max);
    if(min<=-dimX/2. || max>=dimX/2.)
      return true;  
    obj->get_overlapping_interval(p,T,&min,&max);
    if(min<=-dimY/2. || max>=dimY/2.)
      return true;             
    return obj->inverse;
  }
  else
  {
    obj->get_overlapping_interval(p,N,&min,&max);
    if(min>=dimX/2. || max<=-dimX/2.)
      return obj->inverse;  
    obj->get_overlapping_interval(p,T,&min,&max);
    if(min>=dimY/2. || max<=-dimY/2.)
      return obj->inverse;
    return true;      
  }
}
//---------------------------------------------------------------------------
bool ObjBox::slow_collision_check(object_t *obj)
{
  object_t *ss_obj;
  vector_t p;
  bool is_coll=false;
  for(int i=0;i<4;i++)
  {
    p=coins_rot[i]+position-obj->position;  
    if(obj->is_point_in_obj(p,ss_obj))
    {
      vector_t n = obj->get_normal(p);
      if(calc_impulsion(n,coins_rot[i],ss_obj,p))
        is_coll=true;      
    }  
  }       
  return is_coll;
}
//---------------------------------------------------------------------------
double ObjBox::distance_to(vector_t &point, vector_t &dir, double capt_z)
{
  // on vérifie rapidement que l'objet intersecte le rayon
  if(z + hauteur/2. < capt_z || z - hauteur/2. > capt_z)
    return -1.;  
  vector_t t = dir.rotate_spec(0.,1.);  // Rotation de PI sur 2
  double min,max;
  get_overlapping_interval(point,t,&min,&max);   
  if(min>0. || max<0.)
    return -1.;
  
  vector_t D;
  vector_t I;
  double dist = -1.;
  double mu;
  
  D = (position + coins_rot[0]) - point;
  mu = dir.y * N.x - dir.x * N.y;
  if(mu != 0.)
  {
    double tmp_dist = (D.y * N.x - D.x * N.y) / mu;
    if(tmp_dist>=0 && (dist == -1. || tmp_dist<dist)) dist = tmp_dist;
  }
  mu = dir.y * T.x - dir.x * T.y;
  if(mu != 0.)
  {
    double tmp_dist = (D.y * T.x - D.x * T.y) / mu;
    if(tmp_dist>=0 && (dist == -1. || tmp_dist<dist)) dist = tmp_dist;
  }
  D = (position + coins_rot[2]) - point;  
  mu = dir.y * N.x - dir.x * N.y;
  if(mu != 0.)
  {
    double tmp_dist = (D.y * N.x - D.x * N.y) / mu;
    if(tmp_dist>=0 && (dist == -1. || tmp_dist<dist)) dist = tmp_dist;
  }
  mu = dir.y * T.x - dir.x * T.y;
  if(mu != 0.)
  {
    double tmp_dist = (D.y * T.x - D.x * T.y) / mu;
    if(tmp_dist>=0 && (dist == -1. || tmp_dist<dist)) dist = tmp_dist;
  }
  return dist;
}
//---------------------------------------------------------------------------
vector_t ObjBox::get_normal(vector_t &point)
{
  vector_t n;
  vector_t v=point.rotate_spec(cosinus,-sinus);
  v+=G_init;
  if(v.y*dimX == dimY*v.x)
  {
    n.x=dimX;
    n.y=dimY;
    n=n/n.norme();
    if(v.x<0) n=-n;
  }
  else if(v.y*dimX == -dimY*v.x)
  {
    n.x=dimX;
    n.y=-dimY;
    n=n/n.norme();
    if(v.x<0) n=-n;
  }
  else
  {
    bool up_first_biss = v.y*dimX > dimY*v.x;
    bool up_second_biss = v.y*dimX > -dimY*v.x;
    if(!up_first_biss && up_second_biss) n.x=1.0;
    if(up_first_biss && !up_second_biss) n.x=-1.0;
    if(up_first_biss && up_second_biss) n.y=1.0;
    if(!up_first_biss && !up_second_biss) n.y=-1.0;
  }
  if(inverse) n=-n;
  return n.rotate_spec(cosinus,sinus);
}
//---------------------------------------------------------------------------

//----------------------------------------------------------------------------//
//                                                                            //
//                                     Cylindre                               //
//                                                                            //
//----------------------------------------------------------------------------//
ObjCylinder::~ObjCylinder()
{
}
//---------------------------------------------------------------------------
void ObjCylinder::init_params()
{
  G_init.x   = double_param_value("centre_gravite_x");  
  G_init.y   = double_param_value("centre_gravite_y");  
  masse      = double_param_value("masse");
  J          = double_param_value("moment");  
  matiere    = int_param_value("matiere");
  position.x = double_param_value("position_x");  
  position.y = double_param_value("position_y");
  speed.x    = double_param_value("vitesse_x");      
  speed.y    = double_param_value("vitesse_y");        
  angle      = double_param_value("angle")*M_PI/180.;    omega      = double_param_value("omega")*M_PI/180.;
  rayon      = double_param_value("rayon");
  hauteur    = double_param_value("hauteur");

  if(matiere>=NBR_MATIERES) 
  {
    matiere=0;
    printf("Erreur au chargement de l'objet: matière incorrecte, remplacée par du bois...\n");
  }
  last_position=position;
  last_angle=angle;   
  maj_const_vars();
}
//---------------------------------------------------------------------------
void ObjCylinder::draw()
{
  vector_t centre=position-G_rot;
  vector_t radius;
  radius.x=rayon;
  radius=radius.rotate_spec(cosinus,sinus);
  centre*=simul_info->scale;
  radius*=simul_info->scale;
  CercleSDL(((int)centre.x),((int)centre.y),((int)(rayon*simul_info->scale)),couleur);
  LigneSDL(((int)(centre.x+radius.x)),((int)(centre.y+radius.y)),((int)(centre.x-radius.x)),((int)(centre.y-radius.y)),couleur);
}
//---------------------------------------------------------------------------
void ObjCylinder::draw3D() 
{
  int n = 200. * rayon * M_PI;
  vector_t centre=position-G_rot;  
  GLfloat *x = (GLfloat*) malloc(sizeof(GLfloat)*n);
  GLfloat *y = (GLfloat*) malloc(sizeof(GLfloat)*n);
  GLfloat *nx = (GLfloat*) malloc(sizeof(GLfloat)*n);
  GLfloat *ny = (GLfloat*) malloc(sizeof(GLfloat)*n);  
  
  GLfloat B = ((GLfloat)(couleur & 255))/255.;
  GLfloat G = ((GLfloat)((couleur>>8) & 255))/255.;
  GLfloat R = ((GLfloat)((couleur>>16) & 255))/255.;  

  glColor3f(R,G,B); 

  GLfloat spec[] = {0.4,0.4,0.4,1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 40);  
  
  for(int i=0; i<n; i++)
  {
    nx[i] = cos(((double)i)*2.0*M_PI/((double)n)+angle);
    ny[i] = sin(((double)i)*2.0*M_PI/((double)n)+angle);    
    x[i] = centre.x + rayon * nx[i];
    y[i] = centre.y + rayon * ny[i];    
  }
  
  glBegin(GL_POLYGON);  
  glNormal3f(0., 0., 1.);
  for(int i=0; i<n; i++)
  {
    glVertex3f(x[i],y[i],-z+hauteur/2.);
  }
  glEnd();

  glBegin(GL_POLYGON);  
  glNormal3f(0., 0., -1.);
  for(int i=n-1; i>=0; i--)
    glVertex3f(x[i],y[i],-z-hauteur/2.);
  glEnd();
  
  glBegin(GL_QUADS);
  for(int i=0; i<n; i++)
  {
    glNormal3f(nx[i], ny[i], 0.);
    glVertex3f(x[i],y[i],-z+hauteur/2.);       
    glVertex3f(x[i],y[i],-z-hauteur/2.);
    glNormal3f(nx[(i+1)%n], ny[(i+1)%n], 0.);          
    glVertex3f(x[(i+1)%n],y[(i+1)%n],-z-hauteur/2.);  
    glVertex3f(x[(i+1)%n],y[(i+1)%n],-z+hauteur/2.);     
  }
  glEnd();
      
  free(x);
  free(y);
}
//---------------------------------------------------------------------------
bool ObjCylinder::is_point_in_obj(vector_t &point,object_t* &ss_obj)
{
  ss_obj=this;
  vector_t v=point+G_rot;
  if(inverse) return v.norme()>rayon; else return v.norme()<rayon;
}
//---------------------------------------------------------------------------
double ObjCylinder::dist_of_obj(vector_t &point, vector_t &u,object_t* &ss_obj)
{
  ss_obj=this;
  u=point+G_rot;
  double d=u.norme();
  if(!inverse)
  {
    if(d>rayon)
    {
      u=(-u)/u.norme();
      return d-rayon;
    }
    else
    {
      u.x=0.;
      u.y=0.;
      return 0.;
    }
  }
  else
  {
    if(d<rayon)
    {
      u=u/u.norme();
      return rayon-d;
    }
    else
    {
      u.x=0;
      u.y=0;
      return 0.;
    }
  }
}
//---------------------------------------------------------------------------
void ObjCylinder::get_overlapping_interval(vector_t &O, vector_t &Dir,double *min, double *max)
{
  double proj;
  proj=(position-G_rot-O)|Dir;
  *min=proj-rayon;
  *max=proj+rayon;  
}
//---------------------------------------------------------------------------
bool ObjCylinder::fast_collision_check(object_t *obj)
{
  vector_t p;
  double min,max;
  p=position-G_rot; 
  obj->get_overlapping_interval(p,N,&min,&max);
  if(min>=rayon || max<=-rayon)
    return obj->inverse||inverse;
  obj->get_overlapping_interval(p,T,&min,&max);
  if(min>=rayon || max<=-rayon)
    return obj->inverse||inverse;
  return true;  
}
//---------------------------------------------------------------------------
bool ObjCylinder::slow_collision_check(object_t *obj)
{
  object_t *ss_obj=this;
  vector_t p;
  vector_t v;
  vector_t n;
  v=position-G_rot-obj->position;
  double d=obj->dist_of_obj(v,n,ss_obj);
  if(ss_obj==NULL)
    return d>0.;
  else if(d<rayon)
  {
    v+=n*d;
    p=n*d-G_rot;
    n = ss_obj->get_normal(v);
    return calc_impulsion(n,p,ss_obj,v);
  }
  else
    return false;
}
//---------------------------------------------------------------------------
double ObjCylinder::distance_to(vector_t &point, vector_t &dir, double capt_z)
{
  // on vérifie rapidement que l'objet intersecte le rayon
  if(z + hauteur/2. < capt_z || z - hauteur/2. > capt_z)
    return -1.;  
  vector_t t = dir.rotate_spec(0.,1.);  // Rotation de PI sur 2
  double min,max;
  get_overlapping_interval(point,t,&min,&max);   
  if(min>0. || max<0.)
    return -1.;
  
  vector_t C = position - G_rot - point;
  double distC = C|dir;
  if(distC < 0) return -1;
  else
  {
    double _sinus = C ^ dir;
    return distC - sqrt(rayon*rayon - _sinus * _sinus);
  }
}
//---------------------------------------------------------------------------    
vector_t ObjCylinder::get_normal(vector_t &point)
{
  vector_t n=point+G_init;
  if(n.x==0. && n.y==0.)
  {
    if(inverse) return -N;
    else return N;
  }
  else
  {
    n=n/n.norme();
    if(inverse) return -n;    
    else return n;
  }
}
//---------------------------------------------------------------------------



//----------------------------------------------------------------------------//
//                                                                            //
//                                     Sphère                                 //
//                                                                            //
//----------------------------------------------------------------------------//
ObjSphere::~ObjSphere()
{
}
//---------------------------------------------------------------------------
void ObjSphere::init_params()
{
  G_init.x   = double_param_value("centre_gravite_x");  
  G_init.y   = double_param_value("centre_gravite_y");  
  masse      = double_param_value("masse");
  J          = double_param_value("moment");  
  matiere    = int_param_value("matiere");
  position.x = double_param_value("position_x");  
  position.y = double_param_value("position_y");
  speed.x    = double_param_value("vitesse_x");      
  speed.y    = double_param_value("vitesse_y");        
  angle      = double_param_value("angle")*M_PI/180.;    omega      = double_param_value("omega")*M_PI/180.;
  rayon      = double_param_value("rayon");
  hauteur    = double_param_value("rayon");

  if(matiere>=NBR_MATIERES) 
  {
    matiere=0;
    printf("Erreur au chargement de l'objet: matière incorrecte, remplacée par du bois...\n");
  }
  last_position=position;
  last_angle=angle;   
  maj_const_vars();
}
//---------------------------------------------------------------------------
void ObjSphere::draw()
{
  vector_t centre=position-G_rot;
  vector_t radius;
  radius.x=rayon;
  radius=radius.rotate_spec(cosinus,sinus);
  centre*=simul_info->scale;
  radius*=simul_info->scale;
  CercleSDL(((int)centre.x),((int)centre.y),((int)(rayon*simul_info->scale)),couleur);
  LigneSDL(((int)(centre.x+radius.x)),((int)(centre.y+radius.y)),((int)(centre.x-radius.x)),((int)(centre.y-radius.y)),couleur);
}
//---------------------------------------------------------------------------
void ObjSphere::draw3D() 
{
  vector_t centre=position-G_rot;

  GLfloat B = ((GLfloat)(couleur & 255))/255.;
  GLfloat G = ((GLfloat)((couleur>>8) & 255))/255.;
  GLfloat R = ((GLfloat)((couleur>>16) & 255))/255.;  
  glColor3f(R,G,B); 
  
  GLfloat spec[] = {0.5,0.5,0.5,1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 40);
  
  glPushMatrix(); 
  glTranslatef(centre.x,centre.y,-z); 
  glutSolidSphere(rayon,50,50);
  glPopMatrix();   
}
//---------------------------------------------------------------------------
bool ObjSphere::is_point_in_obj(vector_t &point,object_t* &ss_obj)
{
  ss_obj=this;
  vector_t v=point+G_rot;
  if(inverse) return v.norme()>rayon; else return v.norme()<rayon;
}
//---------------------------------------------------------------------------
double ObjSphere::dist_of_obj(vector_t &point, vector_t &u,object_t* &ss_obj)
{
  ss_obj=this;
  u=point+G_rot;
  double d=u.norme();
  if(!inverse)
  {
    if(d>rayon)
    {
      u=(-u)/u.norme();
      return d-rayon;
    }
    else
    {
      u.x=0.;
      u.y=0.;
      return 0.;
    }
  }
  else
  {
    if(d<rayon)
    {
      u=u/u.norme();
      return rayon-d;
    }
    else
    {
      u.x=0;
      u.y=0;
      return 0.;
    }
  }
}
//---------------------------------------------------------------------------
void ObjSphere::get_overlapping_interval(vector_t &O, vector_t &Dir,double *min, double *max)
{
  double proj;
  proj=(position-G_rot-O)|Dir;
  *min=proj-rayon;
  *max=proj+rayon;  
}
//---------------------------------------------------------------------------
bool ObjSphere::fast_collision_check(object_t *obj)
{
  vector_t p;
  double min,max;
  p=position-G_rot; 
  obj->get_overlapping_interval(p,N,&min,&max);
  if(min>=rayon || max<=-rayon)
    return obj->inverse||inverse;
  obj->get_overlapping_interval(p,T,&min,&max);
  if(min>=rayon || max<=-rayon)
    return obj->inverse||inverse;
  return true;  
}
//---------------------------------------------------------------------------
bool ObjSphere::slow_collision_check(object_t *obj)
{
  object_t *ss_obj=this;
  vector_t p;
  vector_t v;
  vector_t n;
  v=position-G_rot-obj->position;
  double d=obj->dist_of_obj(v,n,ss_obj);
  if(ss_obj==NULL)
    return d>0.;
  else if(d<rayon)
  {
    v+=n*d;
    p=n*d-G_rot;
    n = ss_obj->get_normal(v);
    return calc_impulsion(n,p,ss_obj,v);
  }
  else
    return false;
}
//---------------------------------------------------------------------------
double ObjSphere::distance_to(vector_t &point, vector_t &dir, double capt_z)
{
  // on vérifie rapidement que l'objet intersecte le rayon
  if(z + hauteur/2. < capt_z || z - hauteur/2. > capt_z)
    return -1.;  
  vector_t t = dir.rotate_spec(0.,1.);  // Rotation de PI sur 2
  double min,max;
  get_overlapping_interval(point,t,&min,&max);   
  if(min>0. || max<0.)
    return -1.;
  
  vector_t C = position - G_rot - point;
  double distC = C|dir;
  if(distC < 0) return -1;
  else
  {
    double _sinus = C ^ dir;
    return distC - sqrt(rayon*rayon - _sinus * _sinus);
  }
}
//---------------------------------------------------------------------------    
vector_t ObjSphere::get_normal(vector_t &point)
{
  vector_t n=point+G_init;
  if(n.x==0. && n.y==0.)
  {
    if(inverse) return -N;
    else return N;
  }
  else
  {
    n=n/n.norme();
    if(inverse) return -n;    
    else return n;
  }
}
//---------------------------------------------------------------------------


//----------------------------------------------------------------------------//
//                                                                            //
//                              Union de primitives                           //
//                                                                            //
//----------------------------------------------------------------------------//
union_obj_t::~union_obj_t()
{
}
//---------------------------------------------------------------------------
void union_obj_t::add_object(object_t *obj)
{
  obj->fixe=false;
  obj->linked_obj=this;  
  obj->linked=true;
  obj->my_id=-1;
  objects.resize(objects.size()+1);
  objects[objects.size()-1]=obj;
  objects_pos.resize(objects_pos.size()+1);
}
//---------------------------------------------------------------------------
void union_obj_t::attach_object(object_t *obj, vector_t pos)
{    
  objects_last_pos.resize(objects_last_pos.size()+1);  
  objects_last_pos[objects_last_pos.size()-1] = pos;
 
  objects_pos.resize(objects_pos.size()+1);
  objects_pos[objects_pos.size()-1] = pos; 
  
  objects.resize(objects.size()+1);
  objects[objects.size()-1]=obj;  
}
//---------------------------------------------------------------------------
void union_obj_t::detach_object(object_t *obj)
{
  unsigned int i;
  for(i=0; i<objects.size(); i++)
    if(objects[i] == obj)
      break;
  for(; i<objects.size()-1; i++)
  {
    objects[i] = objects[i+1];
    objects_pos[i] = objects_pos[i+1];
  }

  objects.resize(objects.size()-1);
  objects_pos.resize(objects_pos.size()-1);

  obj->linked=false;
}
//---------------------------------------------------------------------------
void union_obj_t::reset_last_pos(bool link_ignore=false)
{
  if(linked && !link_ignore)
    linked_obj->reset_last_pos();
  else
  {
    position=last_position;
    angle=last_angle;  
  }
  objects_pos=objects_last_pos;  
  for(unsigned int i=0; i<objects.size(); i++)
  {
    objects[i]->reset_last_pos(true);       
    objects[i]->position=position;
    objects[i]->angle=angle; 
    objects[i]->G_init=G_init-objects_pos[i]; 
    objects[i]->maj_const_vars();       
  } 
}
//---------------------------------------------------------------------------
void union_obj_t::set_last_pos(bool link_ignore=false)
{
  if(linked && !link_ignore)
    linked_obj->set_last_pos();
  else
  {
    last_position=position;
    last_angle=angle;
  }
  objects_last_pos=objects_pos;   
  for(unsigned int i=0; i<objects.size(); i++)
  {
    objects[i]->set_last_pos(true);  
    objects[i]->last_position=last_position;
    objects[i]->last_angle=last_angle;       
  }    
}
//---------------------------------------------------------------------------
void union_obj_t::init_params()
{  
  maj_const_vars();
}
//---------------------------------------------------------------------------
void union_obj_t::maj_const_vars()
{
  last_position=position;
  last_angle=angle;
  objects_last_pos=objects_pos;
    
  for(unsigned int i=0; i<objects.size(); i++)
  {
    objects[i]->simul_info=simul_info;
    objects[i]->masse=masse;
    objects[i]->J=J;    
    objects[i]->G_init=G_init-objects_pos[i]; 
    objects[i]->matiere=matiere;    
    objects[i]->maj_const_vars();   
  }
  maj_dyn_vars();
}
//---------------------------------------------------------------------------
void union_obj_t::maj_dyn_vars()
{
  cosinus=cos(angle);
  sinus=sin(angle);
  N.x=cosinus;
  N.y=sinus;
  T.x=-sinus;
  T.y=cosinus;
  G_rot=G_init.rotate_spec(cosinus,sinus);
  for(unsigned int i=0; i<objects.size(); i++)
  {   
    objects[i]->omega=omega;
    objects[i]->position=position;        
    objects[i]->angle=angle;            
    objects[i]->maj_dyn_vars();    
  }
}
//---------------------------------------------------------------------------
bool union_obj_t::fast_collision_check(object_t *obj)
{
  for(unsigned int i=0; i<objects.size(); i++)
    if(objects[i]->fast_collision_check(obj)) return true;
  return false;    
}
//---------------------------------------------------------------------------
bool union_obj_t::slow_collision_check(object_t *obj)
{
  bool is_coll=false;
  for(unsigned int i=0; i<objects.size(); i++)
    if(objects[i]->slow_collision_check(obj)) is_coll=true;
  return is_coll;
}
//---------------------------------------------------------------------------
void union_obj_t::get_overlapping_interval(vector_t &O, vector_t &Dir,double *min, double *max)
{
  double m,M;
  bool first=true;
  for(unsigned int i=0; i<objects.size(); i++)
  {
    objects[i]->get_overlapping_interval(O,Dir,&m,&M);  
    if(first)
    {
      *min=m;
      *max=M;
      first=false;
    }
    else
    {
      if(m<*min) *min=m;
      if(M>*max) *max=M;
    }
  }
}
//---------------------------------------------------------------------------
bool union_obj_t::is_point_in_obj(vector_t &point,object_t* &ss_obj)
{
  for(unsigned int i=0; i<objects.size(); i++)
    if(objects[i]->is_point_in_obj(point,ss_obj))
      return true;    
  return false;  
}
//---------------------------------------------------------------------------
double union_obj_t::dist_of_obj(vector_t&,vector_t &,object_t* &ss_obj)
{
  bool is_coll=false;
  for(unsigned int i=0; i<objects.size(); i++)
    if(ss_obj->slow_collision_check(objects[i])) is_coll=true;
  ss_obj=NULL;
  return is_coll?1.:0.;
}
//---------------------------------------------------------------------------
double union_obj_t::distance_to(vector_t &point, vector_t &dir, double capt_z)
{
  double dist = -1.;
  double tmp;
  for(unsigned int i=0; i<objects.size(); i++)
  {
    tmp = objects[i]->distance_to(point,dir,capt_z);
    if(tmp != -1. && (dist == -1. || tmp<dist))
      dist = tmp;
  }
  return dist;
}
//---------------------------------------------------------------------------
vector_t union_obj_t::get_normal(vector_t &point)
{
  object_t *fake;
  for(unsigned int i=0; i<objects.size(); i++)
    if(objects[i]->is_point_in_obj(point,fake))
      return objects[i]->get_normal(point);
  vector_t O;
  return O;
}
//---------------------------------------------------------------------------
void union_obj_t::draw()
{
  for(unsigned int i=0; i<objects.size(); i++)
  {
    objects[i]->position=position;
    objects[i]->draw();
  }
}
//---------------------------------------------------------------------------
void union_obj_t::draw3D() 
{
  for(unsigned int i=0; i<objects.size(); i++)
  {
    objects[i]->position=position;
    objects[i]->draw3D();
  }
}
//---------------------------------------------------------------------------

