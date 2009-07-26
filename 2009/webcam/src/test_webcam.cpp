#include <sstream>
#include "webcam.hpp"
#include "utils.hpp"
#include "processing.hpp"
#include "filter.hpp"

bool filter_pseudored(image_t::pixel_type_t *pix)
{
    const image_t::pixel_type_t Y=pix[0];
    const image_t::pixel_type_t U=pix[1];
    const image_t::pixel_type_t V=pix[2];
    // detect red
    if((U<150 && V>150 && Y<256) || (V>(U+50)))
    {
        pix[0]=255;
        pix[1]=0;
        pix[2]=0;
        return true;
    }
    else
        return false;
}

bool filter_pseudogreen(image_t::pixel_type_t *pix)
{
    image_t::pixel_type_t pix2[3];
    for(int i=0;i<3;i++)
        pix2[i]=pix[i];
    convert_yuv_to_rgb(pix2);
    
    return pix2[1]>=70 && pix2[1]>=(pix2[0]+20) && pix2[1]>=(pix2[2]+20);
    /*
    const image_t::pixel_type_t Y=pix[0];
    const image_t::pixel_type_t U=pix[1];
    const image_t::pixel_type_t V=pix[2];
    
    // detect green
    if(U>=70 && U<=120 && V>=70 && V<=120 && Y>=50 && Y<=200)
    {
        pix[0]=0;
        pix[1]=255;
        pix[2]=0;
        return true;
    }
    else
        return false;
    */
}

int main(int argc,char **argv)
{
    std::string dev(argv[1]);
    
    init_utils();
    
    webcam_t webcam(dev,640,480);
    webcam.start();
    
    for(int i=0;i<10;i++)
    {
        image_t img(image_t::yuv_format,640,480);
        image_t img2(image_t::yuv_format,640,480);
        webcam.capture(img);
        //convert_yuv_to_rgb(img);
        //filter_image(img,img2,despeckle_filter);
        //process_image(img,&filter_pseudogreen,rgb_color_t(255,0,0));
        //img2=img;
        
        unproject_parameters_t uparam;
        uparam.screen_w=640;
        uparam.object_radius=3.5;
        uparam.half_fov=0.46;
        
        if(true)
        {
            std::vector<rectangle_t> res=detect_rectangles(img,&filter_pseudogreen);
            printf("[main] %d rectangle(s) detected !\n",res.size());
            
            for(size_t i=0;i<res.size();i++)
            {
                for(size_t dx=0;dx<=res[i].w;dx++)
                    for(size_t dy=0;dy<=res[i].h;dy++)
                        for(size_t jj=0;jj<3;jj++)
                            img.pixel(res[i].x+dx,res[i].y+dy)[jj]=255;
                
                unproject_result_t ures=unproject(res[i],uparam);
                
                printf("[main] rectangle %d: x=%lf y=%lf\n",i,ures.x,ures.y);
            }
        }
        
        std::ostringstream oss;
        oss<<"out"<<i<<".jpg";
        save_image(img,oss.str());
    }
    
    webcam.stop();
    
    deinit_utils();
    
    return 0;
}
