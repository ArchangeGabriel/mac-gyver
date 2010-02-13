#include "webcamAPI.h"
#include "webcam.hpp"
#include "processing.hpp"
#include "utils.hpp"
#define PC_INCLUDE
#include "../common/const.h"
#include "../simulateur/the_cup/webcam_list.h"
#include "cinematik.h"

webcam_t video_front(WEBCAM1,640,480);
webcam_t video_top(WEBCAM2,640,480);

rectangle_t video_top_detect_rect;

unsigned char* get_webcam_picture(int webcam_id)
{
    return NULL;
}

void init_webcam_api()
{
    init_utils();
    
    video_front.start();
    video_top.stop();
    
    // TODO: à configurer
    video_top_detect_rect.x=170;
    video_top_detect_rect.y=170;
    video_top_detect_rect.w=230;
    video_top_detect_rect.h=200;
}

bool teste_palet_en_place(bool vert)
{
    video_top.do_capture();
    
    filter_func_t fn=(vert ? filter_green : filter_red);
    
    return is_rectangle_of_color(video_top.get_image2(),video_top_detect_rect,fn,50);
}

int wc_reco_config()
{
  return -1;
}

std::vector<palet_t> capture_et_detecte_palets_relatif_camera(bool vert)
{
    video_front.do_capture();
    image_t img(image_t::yuv_format,video_front.get_width(),video_front.get_height());
    
    video_front.get_image(img);
    
    std::vector<rectangle_t> rects;
    filter_func_t fn=(vert ? filter_green : filter_red);
    
    rects=detect_rectangles2(img,fn,0.75);
    
    unproject_parameters_t uparam;
    uparam.screen_w=video_front.get_width();
    uparam.object_radius=3.0;
    uparam.half_fov=/*0.46*/0.33;
    
    std::vector<palet_t> result;
    
    for(size_t i=0;i<rects.size();i++)
    {
        rectangle_t r=rects[i];
        // doing an approximation: the really diameter is not the visible diameter
        float ratio=float(r.w)/float(r.h);
        const float expected_ratio=2.3;
        const float max_ratio_delta=0.7;
        
        unproject_result_t ur=unproject(r,uparam);
        
        float surete=std::max(0.0,1.0-fabs(ratio-expected_ratio)/max_ratio_delta);
        
        palet_t p;
        p.pos.x=ur.x;
        p.pos.y=ur.y;
        p.surete=surete;
        
        result.push_back(p);
    }
    
    return result;
}

std::vector<palet_t> capture_et_detecte_palets(bool vert)
{
    std::vector<palet_t> res=capture_et_detecte_palets_relatif_camera(vert);
    
    for(size_t i=0;i<res.size();i++)
    {
        /*
        M=Xu_X+Y_Y
        u_X=cos(a)u_x+sin(a)u_y
        u_Y=-sin(a)u_x+cos(a)u_y
        ==>
        M=(Xcos(a)-Ysin(a))u_x+(Xsin(a)+Ycos(a))u_y
        */
        palet_t p;
        p.pos.x=cine_get_position().x + _POS_CAM1_X + res[i].pos.x*cos(_POS_CAM1_A) - res[i].pos.y*sin(_POS_CAM1_A);
        p.pos.y=cine_get_position().y + _POS_CAM1_Y + res[i].pos.x*sin(_POS_CAM1_A) + res[i].pos.y*cos(_POS_CAM1_A);
        p.surete=res[i].surete;
        res[i]=p;
    }
    
    return res;
}

