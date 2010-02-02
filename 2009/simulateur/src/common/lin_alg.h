#ifndef lin_algH
#define lin_algH

#include <math.h>

class vector_t
{
  public:
  float x,y;

  vector_t() {x=0.; y=0.;}
  vector_t(float _x,float _y) {x=_x; y=_y;}

  vector_t operator + (vector_t const &S) const
  {
    vector_t R;
    R.x=x+S.x;
    R.y=y+S.y;
    return R;
  }
  inline void operator += (vector_t const &S)
  {
    x+=S.x;
    y+=S.y;
  }
  inline void operator += (vector_t &S)
  {
    x+=S.x;
    y+=S.y;
  }  
  vector_t operator - (vector_t const &S) const
  {
    vector_t R;
    R.x=x-S.x;
    R.y=y-S.y;
    return R;
  } 
  inline void operator -= (vector_t const &S)
  {
    x-=S.x;
    y-=S.y;
  }
  vector_t operator * (float scalar) const
  {
    vector_t R;
    R.x=x*scalar;
    R.y=y*scalar;
    return R;
  }
  inline void operator *= (float const scalar)
  {
    x*=scalar;
    y*=scalar;
  }
  vector_t operator / (float const scalar) const
  {
    vector_t R;
    R.x=x/scalar;
    R.y=y/scalar;
    return R;
  }
  inline void operator /= (float const scalar)
  {
    x/=scalar;
    y/=scalar;
  }

  inline float operator ^ (vector_t const &S) const
  {
    return x*S.y-y*S.x;
  }
  inline float operator | (vector_t const &S) const
  {
    return x*S.x+y*S.y;
  }

  vector_t rotate(float const &alpha) const
  {
    vector_t R;
    float C = cos(alpha);
    float S = sin(alpha);
    R.x=C*x-S*y;
    R.y=S*x+C*y;
    return R;
  }
  inline vector_t rotate_spec(float const &cosinus,float const &sinus) const
  {
    vector_t R;
    R.x=cosinus*x-sinus*y;
    R.y=sinus*x+cosinus*y;
    return R;
  }
  inline float norme() const
  {
    return sqrtl(x*x+y*y);
  }
  inline float norme2() const
  {
    return x*x+y*y;
  }  
  vector_t operator - () const
  {
    vector_t R;
    R.x=-x;
    R.y=-y;
    return R;
  }
  float to_angle()
  {
    float a;                     
    if(x == 0.) a = (y>0)?M_PI_2:-M_PI_2;
    else
    {
      a = atanf(y/x);
      if(x<0) a += M_PI;
      while(a>M_PI) a -= 2.*M_PI;
    }
    return a;
  }
};


#endif

