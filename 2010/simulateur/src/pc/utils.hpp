#ifndef __utils__
#define __utils__

#include "common.hpp"

bool init_utils();
void deinit_utils();
bool save_image(const image_t& img,const std::string& name);
bool load_image(image_t& img,const std::string& name);

#endif // __utils__
