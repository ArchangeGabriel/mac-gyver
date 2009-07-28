#ifndef __video_capture__
#define __video_capture__

#include "common.hpp"

// NOTE: uses internal global variables to maintain the current state of the video capture process
int open_video_device(const std::string& dev_name);
void close_video_device(int fd);
bool init_video_device(int fd);
bool start_capturing(int fd);
void uninit_video_device(int fd);
void stop_capturing(int fd);
bool video_capture(int fd);
bool process_capture_to_image(image_t& img);// image must laready have the good size
size_t get_video_width();
size_t get_video_height();
void set_video_width(size_t w);// call before ini_video_device
void set_video_height(size_t h);// call before ini_video_device

/*
bool video_set_integer_control(int fd,__u32 cid,int v);
bool video_set_boolean_control(int fd,__u32 cid,bool b);
bool video_get_integer_control(int fd,__u32 cid,int& v);
bool video_get_boolean_control(int fd,__u32 cid,bool& b);
*/

#endif // __video_capture__
