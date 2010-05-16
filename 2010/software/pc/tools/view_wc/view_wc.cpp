#include "../../webcam.hpp"
#include "../../webcam_common.hpp"
#include "../../webcam_processing.hpp"
#include "../../sdl.h"
#include "../../../common/const.h"
#include "../../../common/bitmap.h"


#define WEBCAM1 "/dev/video0"
#define WEBCAM2 "/dev/video1"

void copy_img_to_buff(Uint32 *dest, image_t::pixel_type_t *src, int w, int h)
{
  for(int x=0; x<w; x++)
    for(int y=0; y<h; y++)
    {
      int i_dest = (h-y-1)*w+x;
      int i_src = 3*i_dest;
      dest[i_dest] = (((src[i_src+0] << 8) + src[i_src+1]) << 8) + src[i_src+2];
    }
}


int main(int argc, char **argv)
{
  if(argc < 2)
  {
    printf("Usage is: ./view_wc_tool id [filename]\n");
    return 1;
  }
  
  char *id;
  int w,h;
  if(argv[1][0] == '0')
  {
    id = WEBCAM1;
    w = WC1_resX;
    h = WC1_resY;    
  }
  else
  {
    id = WEBCAM2;
    w = WC2_resX;
    h = WC2_resY;    
  }
    
  image_t img(image_t::yuv_format,0,0);
  webcam_t WC("test", id, w, h);
  sdlWindow visu(w, h, "Webcam");  
  
  WC.start();
  
  if(argc >= 3)
    save_buff_to_bitmap(argv[2], WC.get_width(), WC.get_height(), img.data);  
  else
    while(true)
    {
      WC.capture(img);
      convert_yuv_to_rgb(img);
      copy_img_to_buff(visu.back_pixels, img.data, w, h);
      visu.Load_SDL_Background(true);
      visu.Draw_SDL_Background();
      visu.RefreshSDL();
      usleep(10000);
    }
}
