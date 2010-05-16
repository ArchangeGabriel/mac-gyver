#ifndef __webcam_common__
#define __webcam_common__

#include <stdint.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <cassert>
#include <vector>
#include <algorithm>
#include <cmath>

static inline int clamp(int lower,int upper,int val)
{
    if(val<lower)
        return lower;
    if(val>upper)
        return upper;
    return val;
}

// RGB32/YUV format
struct image_t
{
    typedef uint16_t pixel_type_t;

    enum image_format_t
    {
      rgb_format,
      yuv_format
    };

    image_t(image_format_t fmt,size_t w,size_t h)
        :fmt(fmt),w(w),h(h)
    {
      data=new pixel_type_t[w*h*3];
    }
    image_t(const image_t& o);
    image_t& operator=(const image_t& o);

    ~image_t()
    {
      delete[] data;
    }

    image_format_t fmt;
    size_t w,h;
    pixel_type_t *data;

    inline pixel_type_t *pixel(size_t x,size_t y)
    {
        return &data[3*(y*w+x)];
    }
    
    inline pixel_type_t *my_pixel(size_t x,size_t y)
    {
        return &data[3*((h-1-y)*w+x)];
    }

    inline const pixel_type_t *pixel(size_t x,size_t y) const
    {
        return &data[3*(y*w+x)];
    }
};

#endif // __common__
