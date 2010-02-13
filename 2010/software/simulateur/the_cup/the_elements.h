#ifndef THE_ELEMENTS_H
#define THE_ELEMENTS_H

#include "../object.h"

class EltTomato : public ObjSphere
{
  public:
  EltTomato();
  
  void calc_frottements();  
  void calc_forces();
};

class EltOrange : public ObjSphere
{
  public:
  EltOrange();
  
  void calc_frottements();  
  void calc_forces();
};

class EltCorn : public ObjCylinder
{
  public:
  EltCorn();
  
  void calc_frottements();  
  void calc_forces();
};

class EltFakeCorn : public ObjCylinder
{
  public:
  EltFakeCorn();
  
  void calc_frottements();  
  void calc_forces();
};



#endif
