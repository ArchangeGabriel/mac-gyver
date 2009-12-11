#include "the_elements.h"
#include "colors.h"


//----------------------------------------------------------------------------
EltTomato::EltTomato()
{
  fixe=false;
  rayon=0.05;
  hauteur=rayon*2;
  z=rayon;  
  masse=0.15;
  J=masse*rayon*rayon*2./5.;
  matiere=mBois;
  couleur=clTomate;
}
//----------------------------------------------------------------------------
void EltTomato::calc_forces()
{
  calc_frottements();
}
//----------------------------------------------------------------------------
void EltTomato::calc_frottements()
{
  // frottements
  vector_t C1,C2,F;
  double norme;
  norme=-1.*simul_info->coeff_frott[mBois][mBois]*masse;
  F = speed*norme;
  add_force(G_rot,F);    
  C1=N*rayon;
  C2=-C1;
  norme=(get_speed(C1)^N)/rayon*0.0008;
  F=T*norme;
  add_force(C1,F);
  F=-F;
  add_force(C2,F);
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
EltOrange::EltOrange()
{
  fixe=false;
  rayon=0.05;
  hauteur=rayon*2;
  z=rayon;  
  masse=0.3;
  J=masse*rayon*rayon*2./5.;
  matiere=mBois;
  couleur=clOrange;
}
//----------------------------------------------------------------------------
void EltOrange::calc_forces()
{
  calc_frottements();
}
//----------------------------------------------------------------------------
void EltOrange::calc_frottements()
{
  // frottements
  vector_t C1,C2,F;
  double norme;
  norme=-1.*simul_info->coeff_frott[mBois][mBois]*masse;
  F = speed*norme;
  add_force(G_rot,F);    
  C1=N*rayon;
  C2=-C1;
  norme=(get_speed(C1)^N)/rayon*0.0008;
  F=T*norme;
  add_force(C1,F);
  F=-F;
  add_force(C2,F);
}
//----------------------------------------------------------------------------






//----------------------------------------------------------------------------
EltCorn::EltCorn()
{
  fixe=false;
  rayon=0.025;
  masse=0.25;
  hauteur=0.15;
  z=hauteur/2;  
  J=masse*rayon*rayon/2.;
  matiere=mBois;
  couleur=clMais;
}
//----------------------------------------------------------------------------
void EltCorn::calc_forces()
{
  calc_frottements();
}
//----------------------------------------------------------------------------
void EltCorn::calc_frottements()
{
  // frottements
  vector_t C1,C2,F;
  double norme;
  norme=-10.*simul_info->coeff_frott[mBois][mBois]*masse;
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





//----------------------------------------------------------------------------
EltFakeCorn::EltFakeCorn()
{
  fixe=true;
  rayon=0.025;
  masse=0.25;
  hauteur=0.15;
  z=hauteur/2; 
  J=masse*rayon*rayon/2.;
  matiere=mBois;
  couleur=clFake;
}
//----------------------------------------------------------------------------
void EltFakeCorn::calc_forces()
{
  calc_frottements();
}
//----------------------------------------------------------------------------
void EltFakeCorn::calc_frottements()
{
  // frottements
  vector_t C1,C2,F;
  double norme;
  norme=-10.*simul_info->coeff_frott[mBois][mBois]*masse;
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
