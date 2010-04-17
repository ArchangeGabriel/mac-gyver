#ifndef __webcam_filter__
#define __webcam_filter__

#include "webcam_common.hpp"

enum filter_t
{
    blur_filter,
    blur2_filter,
    preswitt_filter,
    roberts_filter,
    sobel_filter,
    kirsch_filter,
    mdif_filter,
    despeckle_filter
};

void filter_image(const image_t& from,image_t& to,filter_t filter);

#endif // __filter__
