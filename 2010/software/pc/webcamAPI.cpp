#define PC_INCLUDE
#include "webcamAPI.h"
#include "webcam.hpp"
#include "webcam_processing.hpp"
#include "webcam_utils.hpp"
#include "../common/const.h"
#include "cinematik.h"
#include "sdl.h"

#include "../common/simul.h"

#ifdef SIMULATION
#define WEBCAM1 "0"
#define WEBCAM2 "1"
#else
#define WEBCAM1 "/dev/video1"
#define WEBCAM2 "/dev/video2"
#endif

webcam_t video_front(WEBCAM1,640,480);
webcam_t video_top(WEBCAM2,640,480);

#ifdef VISUALIZE
sdlWindow output_wc1(640, 480, "Webcam1");
sdlWindow output_wc2(640, 480, "Webcam2");
#endif

void webcam_init()
{
  #ifndef SIMULATION
  init_utils();
  #endif
    
  video_front.start();
  video_top.start();
}

int wc_reco_config()
{

/*
  WC.do_capture();
  image_t img = WC.get_image2();*/
 
//  save_buff_to_bitmap("img.bmp", 640, 480, img.data);


  return 14;
}

