#include "motor.h"

//---------------------------------------------------------------------------
motor_t::motor_t()
{
  C=0;
  I=0;
  w=0;
  a=0;
}
//---------------------------------------------------------------------------  
void motor_t::init_params()
{
  printf("  --> Loading motor's params... ");
  fflush(stdout); 
  L  = double_param_value("inductance")/1000.;
  R  = double_param_value("resistance");    
  Kc = double_param_value("const_couple")/1000.;
  Kv = double_param_value("const_vitesse")*2.*M_PI/60.;
  d  = double_param_value("pente_couple_vitesse")*2.*M_PI/60.*1000.; 
  k  = double_param_value("rapport_reduction");
  n  = double_param_value("rendement");
  J  = double_param_value("inertie")*0.0000001 + J;
  
  Kc_k_n_0_98_2=Kc*k*n*0.98*2.;   
  r_sur_k=r/k;
  J_sur_dt=J/dt;
  m_sur_J_plus_m_r_carre=m/(J+m*r*r);
  m_g_e=m*g*e/1.5;   // m est déjà divisé par 2 mais on redivise car il y a 4 appuis
  dt_k_sur_J=dt*k/J;
  
  printf("ok\n");
  fflush(stdout);  
}
//---------------------------------------------------------------------------  
double motor_t::calc_force(double U, double V)
{
  double F;

  //w = V * k / r;
/*  I=(U - w/Kv) / R;
  C=I*Kc_k_n_0_98_2;   
  F=((w*r_sur_k-V)*J_sur_dt+C*r)*m_sur_J_plus_m_r_carre; // force de frottement 
      
  if(F>m_g_e)
  {
    derape=true;
    F=m_g_e;
  }
  else
  if(F<-m_g_e)
  {
    derape=true;
    F=-m_g_e;  
  }
  else
    derape=false;
    
  M=C-r*F;     // moment sur la roue
  w+=M*dt_k_sur_J;
  a+=w*dt;
  return F;*/
  
  I=(U - w/Kv) / R;
  C=I*Kc_k_n_0_98_2;   
  F=(w*r_sur_k-V)*m/dt; // force de frottement 
      
  if(F>m_g_e)
  {
    derape=true;
    F=m_g_e;
  }
  else
  if(F<-m_g_e)
  {
    derape=true;
    F=-m_g_e;  
  }
  else
    derape=false;
  
  w+=(C-r*F) * 2. * k * dt/ (m * r * r);  
  a+=w*dt;
  return F;  
}
//---------------------------------------------------------------------------  
