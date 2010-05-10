#include "webcam_common.hpp"

image_t& image_t::operator=(const image_t& o)
{
  if(&o!=this)
  {
    delete[] data;
    w=o.w;
    h=o.h;
    fmt=o.fmt;
    data=new pixel_type_t[w*h*3];
    memcpy(data,o.data,w*h*3*sizeof(pixel_type_t));
  }
  return *this;
}
image_t::image_t(const image_t& o)
{
  w=o.w;
  h=o.h;
  fmt=o.fmt;
  data=new pixel_type_t[w*h*3];
  memcpy(data,o.data,w*h*3*sizeof(pixel_type_t));
}
