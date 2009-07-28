#include "common.hpp"
#include "processing.hpp"
#include "video_capture.hpp"
#include "utils.hpp"

/*
float blur_kernel[3][3]=
{
    {1.0f/16.0f,2.0f/16.0f,1.0f/16.0f},
    {2.0f/16.0f,4.0f/16.0f,2.0f/16.0f},
    {1.0f/16.0f,2.0f/16.0f,1.0f/16.0f}
};

float blur2_kernel[3][3]=
{
    {1/9.,1/9.,1/9.},
    {1/9.,1/9.,1/9.},
    {1/9.,1/9.,1/9.}
};

float v_prewitt_kernel[3][3]=
{
    {-1.0f/16.0f,0.0f,1.0f/16.0f},
    {-1.0f/16.0f,0.0f,1.0f/16.0f},
    {-1.0f/16.0f,0.0f,1.0f/16.0f}
};

float h_prewitt_kernel[3][3]=
{
    {-1.0f/16.0f,-1.0f/16.0f,-1.0f/16.0f},
    {0.0f,0.0f,0.0f},
    {1.0f/16.0,1.0f/16.0,1.0f/16.0f}
};

float p_roberts_kernel[3][3]=
{
    {0,0,0},
    {0,0,1},
    {0,-1,0}
};

float m_roberts_kernel[3][3]=
{
    {0,0,0},
    {0,-1,0},
    {0,0,1}
};

float v_sobel_kernel[3][3]=
{
    {-1.0f/4.0f,0.0f,1.0f/4.0f},
    {-2.0f/4.0f,0.0f,2.0f/4.0f},
    {-1.0f/4.0f,0.0f,1.0f/4.0f}
};

float h_sobel_kernel[3][3]=
{
    {-1.0f/4.0f,-2.0f/4.0f,-1.0f/4.0f},
    {0.0f,0.0f,0.0f},
    {1.0f/4.0,2.0f/4.0,1.0f/4.0f}
};

float v_kirsch_kernel[3][3]=
{
    {-3.0f/15.0f,-3.0f/15.0f,5.0f/15.0f},
    {-3.0f/15.0f,0,5.0f/15.0f},
    {-3.0f/15.0f,-3.0f/15.0f,5.0f/15.0f},
};

float h_kirsch_kernel[3][3]=
{
    {-3.0f/15.0f,-3.0f/15.0f,-3.0f/15.0f},
    {-3.0f/15.0f,0,-3.0f/15.0f},
    {5.0f/15.0f,5.0f/15.0f,5.0f/15.0f},
};

float v_mdif_kernel[5][5]=
{
    {0,-1/12.,0,1/12.,0},
    {-1/12.,-2/12.,0,2/12.,1/12.},
    {-1/12.,-3/12.,0,3/12.,1/12.},
    {-1/12.,-2/12.,0,2/12.,1/12.},
    {0,-1/12.,0,1/12.,0}
};

float h_mdif_kernel[5][5]=
{
    {0,-1/12.,-1/12.,-1/12.,0},
    {-1/12.,-2/12.,-3/12.,-2/12.,-1/12.},
    {0,0,0,0,0},
    {1/12.,2/12.,3/12.,2/12.,1/12.},
    {0,1/12.,1/12.,1/12.,0}
};

float dog_kernel[7][7]=
{
    {0,0,1/28.,1/28.,1/28.,0,0},
    {0,1/28.,1/28.,1/28.,1/28.,1/28.,0},
    {1/28.,1/28.,-1/28.,-4/28.,-1/28.,1/28.,1/28.},
    {1/28.,1/28.,-4/28.,-8/28.,-4/28.,1/28.,1/28.},
    {1/28.,1/28.,-1/28.,-4/28.,-1/28.,1/28.,1/28.},
    {0,1/28.,1/28.,1/28.,1/28.,1/28.,0},
    {0,0,1/28.,1/28.,1/28.,0,0}
};

float log_kernel[5][5]=
{
    {0,0,-1/16.,0,0},
    {0,-1/16.-2/16.,-1/16.,0},
    {-1/16.,-2/16.,16/16.,-2/16.,-1/16.},
    {0,-1/16.-2/16.,-1/16.,0},
    {0,0,-1/16.,0,0}
};

typedef void (*do_convolution_ex_op_t)(image_t& res,size_t x,size_t y,size_t i,image_t::pixel_type_t val);

void equal_op(image_t& res,size_t x,size_t y,size_t i,image_t::pixel_type_t val)
{
    res.pixel(x,y)[i]=val;
}

void add_equal_op(image_t& res,size_t x,size_t y,size_t i,image_t::pixel_type_t val)
{
    res.pixel(x,y)[i]+=val;
}

template<size_t T>
void do_convolution_ex(const image_t& img,image_t& res,float kernel[T][T],do_convolution_ex_op_t op)
{
    size_t shift=(T-1)/2;
    for(size_t y=shift;y<img.h-shift;y++)
    {
        for(size_t x=shift;x<img.w-shift;x++)
        {
            for(size_t i=0;i<3;i++)
            {
                float fres=0.0f;
                for(size_t dy=0;dy<T;dy++)
                    for(size_t dx=0;dx<T;dx++)
                        fres+=img.pixel(x+dx-shift,y+dy-shift)[i]*kernel[dy][dx];
                //res.pixel(x,y)[i]=fres;
                op(res,x,y,i,fres);
            }
        }
    }
}

template<size_t T>
void do_convolution(const image_t& img,image_t& res,float kernel[T][T])
{
    do_convolution_ex(img,res,kernel,equal_op);
}

void do_normalization(image_t& img)
{
    for(size_t y=0;y<img.h;y++)
        for(size_t x=0;x<img.w;x++)
            for(size_t i=0;i<3;i++)
                img.pixel(x,y)[i]=clamp(img.pixel(x,y)[i],0,255);
}

void do_addition(image_t& img,const image_t& o)
{
    for(size_t y=0;y<o.h;y++)
        for(size_t x=0;x<o.w;x++)
            for(size_t i=0;i<3;i++)
                img.pixel(x,y)[i]=img.pixel(x,y)[i]+o.pixel(x,y)[i];
}

void do_subtraction(image_t& img,const image_t& o)
{
    for(size_t y=0;y<o.h;y++)
        for(size_t x=0;x<o.w;x++)
            for(size_t i=0;i<3;i++)
                img.pixel(x,y)[i]=img.pixel(x,y)[i]-o.pixel(x,y)[i];
}

typedef void (*do_op_t)(image_t::pixel_type_t *px);

void do_op(image_t& img,do_op_t op)
{
    for(size_t y=0;y<img.h;y++)
        for(size_t x=0;x<img.w;x++)
            op(img.pixel(x,y));
}

void white_threshold_op(image_t::pixel_type_t *px)
{
    if(px[0]<150 || px[1]<150 || px[2]<150)
        px[0]=px[1]=px[2]=0;
}

void to_wb_op(image_t::pixel_type_t *px)
{
    image_t::pixel_type_t val=0.3*px[0]+0.59*px[1]+0.11*px[2];
    px[0]=px[1]=px[2]=val;
}

inline float square(float a)
{
    return a*a;
}

void do_despeckle(const image_t& img,image_t& res,float delta,size_t size)
{
    size_t shift=(size-1)/2;
    for(size_t III=0;III<3;III++)
    {
        for(size_t y=shift;y<img.h-shift;y++)
        {
            for(size_t x=shift;x<img.w-shift;x++)
            {
                float avg=0;
                for(size_t i=1;i<shift;i++)
                    for(size_t j=1;j<shift;j++)
                        for(size_t i=0;i<3;i++)
                            avg+=img.pixel(x+i,y+j)[III]+img.pixel(x-i,y-j)[III];
                avg/=(size*size-1);
                float etype=0;
                for(size_t i=1;i<shift;i++)
                    for(size_t j=1;j<shift;j++)
                        etype+=square(img.pixel(x+i,y+j)[III]-avg)+square(img.pixel(x-i,y-j)[III]-avg);
                etype/=(size*size-1);
                float delta2=fabs(img.pixel(x,y)[III]-avg)-delta*sqrt(etype);
                if(delta2>=0.0)
                    res.pixel(x,y)[III]=avg;
                else
                    res.pixel(x,y)[III]=img.pixel(x,y)[III];
            }
        }
    }
}

bool process_image(image_t& img)
{
    image_t res(img.w,img.h);
    // optional pre filtering
    #if 1
    do_convolution(img,res,blur_kernel);
    img=res;
    #endif

    // optional to black&white transformation
    #if 1
    do_op(img,to_wb_op);
    #endif

    // optional pre filtering
    #if 1
    do_convolution(img,res,blur_kernel);
    img=res;
    #endif

    // blur&subtract
    #if 0
    do_convolution(img,res,blur_kernel);
    do_subtraction(img,res);
    #endif

    // blur&subtract
    #if 1
    do_convolution(img,res,blur2_kernel);
    do_subtraction(img,res);
    #endif

    // preswitt kernels
    #if 0
    do_convolution(img,res,v_prewitt_kernel);
    do_convolution_ex(img,res,h_prewitt_kernel,add_equal_op);
    img=res;
    #endif

    // roberts kernels
    #if 0
    do_convolution(img,res,p_roberts_kernel);
    do_convolution_ex(img,res,m_roberts_kernel,add_equal_op);
    img=res;
    #endif

    // sobel kernels
    #if 0
    do_convolution(img,res,v_sobel_kernel);
    do_convolution_ex(img,res,h_sobel_kernel,add_equal_op);
    img=res;
    #endif

    // kirsch kernels
    #if 0
    do_convolution(img,res,v_kirsch_kernel);
    do_convolution_ex(img,res,h_kirsch_kernel,add_equal_op);
    img=res;
    #endif

    // MDIF kernel
    #if 0
    do_convolution(img,res,v_mdif_kernel);
    do_convolution_ex(img,res,h_mdif_kernel,add_equal_op);
    img=res;
    #endif

    // DOG(Difference of gaussian) kernel
    #if 0
    do_convolution(img,res,dog_kernel);
    img=res;
    #endif

    // LOG(Laplacian of gaussian) kernel
    #if 0
    do_convolution(img,res,log_kernel);
    img=res;
    #endif

    // despeckle
    #if 0
    do_despeckle(img,res,0.5,5);
    img=res;
    #endif

    // thresold(white)
    #if 0
    do_op(img,white_threshold_op);
    #endif

    // optional post filtering
    #if 0
    do_convolution(img,res,blur_kernel);
    img=res;
    #endif
    do_normalization(img);
    return true;
}
*/

void next_color(rgb_color_t& c)
{
    const size_t delta=100;
    if(c.r==255 || (c.r+delta)>255)
    {
        c.r=255;
        if(c.g==255 || (c.g+delta)>255)
        {
            c.g=255;
            if(c.b==255 || (c.b+delta)>255)
            {
               c.r=delta;
               c.g=0;
               c.b=0;
            }
            else
                c.b+=delta;
        }
        else
            c.g+=delta;
    }
    else
        c.r+=delta;
}
/*
bool rect_filter(const rectangle_t& r)
{
    const size_t min_area_x=300;
    const float min_ratio=2.3-0.5;
    const float max_ratio=2.3+0.5;
    // filter too small ones
    if(r.area()<min_area_x)
        return false;
    // filter rectangle without the good shape
    float ratio=r.w;
    ratio/=r.h;
    if(ratio<min_ratio || ratio>max_ratio)
        return false;
    //
    return true;
}

void rect_comp(const rectangle_t& r1,bool& k1,const rectangle_t& r2,bool& k2)
{
    rectangle_t i=r1.intersect(r2);

    if(i.area()/float(r1.area()) > 0.9)
        k1=false;
    else
        k1=true;

    if(i.area()/float(r2.area()) > 0.9)
        k2=false;
    else
        k2=true;

    // avoid to delete both !
    if(!k1 && !k2)
    {
        if(r1.area()>r2.area())
            k1=true;
        else
            k2=true;
    }
}*/

void filter_edge_detection(std::vector<hv_segment_t>& s)
{
    for(int j=0;j<int(s.size());j++)
    {
        bool discard=false;
        if(s[j].dir!=hv_segment_t::down || s[j].len<10)
            discard=true;
        if(discard)
        {
            s.erase(s.begin()+j);
            j--;
        }
    }
}

void add_rectangle(std::vector<rectangle_t>& rects,rectangle_t rect)
{
    // filter overlapping
    for(int i=0;i<int(rects.size());i++)
    {
        rectangle_t inter=rect.intersect(rects[i]);
        if(inter.area()==0)
            continue;
        float percentage1=float(inter.area())/rect.area();
        float percentage2=float(inter.area())/rects[i].area();
        if(percentage1>0.4)
            return;
        if(percentage2>0.4)
        {
            rects.erase(rects.begin()+i);
            i--;
        }
    }
    rects.push_back(rect);
}

bool detect_image(image_t& img)
{
    h_decomp_t hd;
    if(!do_h_decomposition(img,hd,rgb_color_t(255,0,0)))
        return false;
    // clear image
    for(size_t y=0;y<img.h;y++)
        for(size_t x=0;x<img.w;x++)
            for(size_t i=0;i<3;i++)
                img.pixel(x,y)[3]=0;
    // print blocks
    rgb_color_t col(255,255,255);

    printf("[main] %d blocks detected\n",hd.blocks.size());
    #if 0
    for(size_t i=0;i<hd.blocks.size();i++)
    {
        const v_block_decomp_t& b=hd.blocks[i];
        for(size_t dy=0;dy<b.seg.size();dy++)
            for(size_t dx=0;dx<b.seg[dy].w;dx++)
            {
                img.pixel(b.seg[dy].x+dx,b.y+dy)[0]=col.r;
                img.pixel(b.seg[dy].x+dx,b.y+dy)[1]=col.g;
                img.pixel(b.seg[dy].x+dx,b.y+dy)[2]=col.b;
            }

        next_color(col);
    }
    #endif

    #if 1
    const size_t tolerance=4;
    std::vector<rectangle_t> rects;
    std::vector<edge_detection_t> edges;

    printf("[main] apply approximate H/V edge detection with tolerance %u\n",tolerance);
    for(size_t i=0;i<hd.blocks.size();i++)
    {
        const size_t edge_detection_min_w=50;
        const v_block_decomp_t& b=hd.blocks[i];
        edge_detection_t ed=do_approx_edge_detection(b,tolerance,edge_detection_min_w);
        printf("[main] block %u: filtering artecfacts...",i);
        filter_edge_detection(ed.left);
        filter_edge_detection(ed.right);
        printf("%u segment left/%u segment right\n",ed.left.size(),ed.right.size());

        if(ed.left.size()==1 && ed.right.size()==1)
        {
            edges.push_back(ed);
            continue;
        }
        printf("[main] block %u: complex decomposition: splitting...",i);
        size_t left=0,right=0;
        size_t nb_pieces=0;

        while(left!=ed.left.size() && right!=ed.right.size())
        {
            // update left if necessary
            while(left<ed.left.size() && ed.left[left].end.y<=ed.right[right].orig.y)
                left++;
            if(left==ed.left.size())
                break;
            // update right
            while(right<ed.right.size() && ed.right[right].end.y<=ed.left[left].orig.y)
                right++;
            if(right==ed.right.size())
                break;
            // add to list
            edge_detection_t new_ed;
            new_ed.left.push_back(ed.left[left]);
            new_ed.right.push_back(ed.right[right]);
            // normalize
            size_t uy=std::max(new_ed.left[0].orig.y,new_ed.right[0].orig.y);
            size_t dy=std::min(new_ed.left[0].end.y,new_ed.right[0].end.y);
            new_ed.left[0].orig.y=new_ed.right[0].orig.y=uy;
            new_ed.left[0].end.y=new_ed.right[0].end.y=dy;
            new_ed.left[0].len=new_ed.right[0].len=dy-uy+1;

            edges.push_back(new_ed);
            nb_pieces++;
            // find first end
            if(ed.left[left].end.y<ed.right[right].end.y)
                left++;
            else
                right++;
        }
        printf("got %u pieces\n",nb_pieces);
    }

    for(size_t i=0;i<edges.size();i++)
    {
        const edge_detection_t ed=edges[i];
        /*
        std::vector<hv_segment_t> s[2];

        s[0]=ed.left;
        s[1]=ed.right;

        for(size_t k=0;k<2;k++)
        {
            //printf("(main] edge %u: got %u segments\n",i,s[i].size());

            for(size_t j=0;j<s[k].size();j++)
            {
                fflush(stdout);
                const hv_segment_t& seg=s[k][j];
                __pos_t p=seg.orig;
                //printf("[main] seg: (%u,%u)->(%u,%u) [len=%u]\n",seg.orig.x,seg.orig.y,seg.end.x,seg.end.y,seg.len);

                for(size_t walk=0;walk<seg.len;walk++)
                {
                    img.pixel(p.x,p.y)[0]=col.r*0;
                    img.pixel(p.x,p.y)[1]=col.g*0+255;
                    img.pixel(p.x,p.y)[2]=col.b*0;

                    if(seg.dir==hv_segment_t::left)
                        p.x--;
                    else if(seg.dir==hv_segment_t::right)
                        p.x++;
                    else if(seg.dir==hv_segment_t::down)
                        p.y++;
                }
            }
            next_color(col);
        }
        //*/

        if(ed.left.size()==1 && ed.right.size()==1)
        {
            size_t up_y=std::max(ed.left[0].orig.y,ed.right[0].orig.y);
            size_t down_y=std::min(ed.left[0].end.y,ed.right[0].end.y);
            rectangle_t r;
            r.x=ed.left[0].orig.x;
            r.y=up_y;
            r.h=down_y-up_y+1;
            r.w=ed.right[0].orig.x-ed.left[0].orig.x+1;
            // doing an approximation: the really diameter is not the visible diameter
            const size_t min_w=30;
            if(r.w<=min_w)
                continue;

            // doing an approximation: the really diameter is not the visible diameter
            float ratio=float(r.w)/float(r.h);
            const float expected_ratio=2.3;
            const float max_ratio_delta=0.7;

            float perfect_division=expected_ratio/ratio;
            long nearest=lround(perfect_division);
            float delta=fabs(perfect_division-nearest);
            printf("[main] perfect_div=%f delta=%f\n",perfect_division,delta);
            // doing some calculus gives us the following formula:
            // for each rectangle to be within range, one must have:
            // delta<H/W*max_ratio_delta
            if(delta<(r.h*max_ratio_delta/r.w))
            {
                r.h/=nearest;
                //printf("[main] ratio=%f\n",float(r.w)/r.h);
                for(long j=0;j<nearest;j++)
                {
                    r.y=up_y+j*r.h;
                    add_rectangle(rects,r);
                }
            }
            else
            {
                long H=r.h;
                long Y=r.y;
                r.h=r.w/expected_ratio;
                nearest=floor(H/r.h);
                for(long i=0;i<nearest;i++)
                {
                    r.y=Y+H-i*r.h;
                    add_rectangle(rects,r);
                }
            }
        }
        else
        {
            printf("[main] got non-standard edge decomposition: %u left, %u right. Give up...\n",ed.left.size(),ed.right.size());
        }

    }
    #endif

    #if 0
    printf("[main] detecting rectangles...\n");
    // now detect rectangles
    std::vector<rectangle_t> rects;
    const size_t delta_x=3;

    for(size_t i=0;i<hd.blocks.size();i++)
    {
        // call with rects and not a new vector to allow filtering against already found rectangles
        if(!extract_rectangles(hd.blocks[i],rects,delta_x,&rect_filter,&rect_comp))
            continue;
    }
    for(size_t i=0;i<rects.size();i++)
        printf("[main] rect: x=%d y=%d w=%d h=%d\n",rects[i].x,rects[i].y,rects[i].w,rects[i].h);
    #endif

    printf("[main] %d rectangle(s) detected\n",rects.size());
    for(size_t i=0;i<rects.size();i++)
    {
        printf("[main] rect: x=%d y=%d w=%d h=%d\n",rects[i].x,rects[i].y,rects[i].w,rects[i].h);
        int rel_x_beta=rects[i].x;
        rel_x_beta-=img.w/2;
        int rel_x_alpha=rects[i].x+rects[i].w;
        rel_x_alpha-=img.w/2;
        rel_x_alpha=-rel_x_alpha;// axis inversion
        rel_x_beta=-rel_x_beta;// ditto

        float half_fov=(53.333/2)/180.0*M_PI;
        float factor=img.w/2.0/tan(half_fov);
        float alpha=atan(rel_x_alpha/factor);
        float beta=atan(rel_x_beta/factor);
        printf("[main] alpha=%f beta=%f\n",alpha,beta);

        float radius=0.035;
        float cx=-radius*(sin(alpha)+sin(beta))/sin(beta-alpha);
        float cy=radius*(cos(alpha)+cos(beta))/sin(beta-alpha);
        printf("[main] (camera_angle_related) cx=%f cy=%f cy/cx=%f\n",cx,cy,cy/cx);
        // compute position is angle absolute view
        float cam_angle=45.0/180*M_PI;
        float new_cx=cx*cos(cam_angle)+cy*sin(cam_angle);
        float new_cy=-cx*sin(cam_angle)+cy*cos(cam_angle);
        printf("[main] (camera_angle_neutral) cx=%f cy=%f cy/cx=%f\n",new_cx,new_cy,new_cy/new_cx);
    }

    #if 1
    for(size_t i=0;i<rects.size();i++)
    {
        for(size_t dy=0;dy<rects[i].h;dy++)
            for(size_t dx=0;dx<rects[i].w;dx++)
            {
                img.pixel(rects[i].x+dx,rects[i].y+dy)[0]=col.r;
                img.pixel(rects[i].x+dx,rects[i].y+dy)[1]=col.g;
                img.pixel(rects[i].x+dx,rects[i].y+dy)[2]=col.b;
            }

        next_color(col);
    }
    #endif

    return true;
}

#if 0
int main(int argc,char **argv)
{
    if(!init_utils())
    {
        printf("[main] Cannot init utility library\n");
        return 0;
    }

    char dev[64]={0};
    strcpy(dev,"/dev/video0");

    if(argc>=2)
    {
        const char *prefix="-dev=";
        if(strlen(argv[1])>=strlen(prefix) && strncmp(argv[1],prefix,strlen(prefix))==0)
        {
            memcpy(dev,argv[1]+strlen(prefix),std::min(size_t(63),1+strlen(argv[1])-strlen(prefix)));
        }
    }

    set_video_width(640);
    set_video_height(480);

    printf("[main] Using device %s\n",dev);

    int video_device=open_video_device(dev);
    if(video_device==-1)
    {
        printf("[main] Error: unable to open video device %s\n",dev);
        return 0;
    }
    if(!init_video_device(video_device))
    {
        close_video_device(video_device);
        return 0;
    }
    if(!start_capturing(video_device))
    {
        printf("[main] Error: couldn't start capturing\n");
    }
    else
    {
        for(size_t i=0;i<4;i++)
        {
            sleep(1);
            image_t img(image_t::yuv_format,get_video_width(),get_video_height());
            if(video_capture(video_device))
            {
                if(process_capture_to_image(img) && process_image(img))
                {
                    char str[32];
                    sprintf(str,"capture%u.bmp",i);
                    if(!save_image(img,str))
                    {
                        printf("[main] Error: image saving failed\n");
                    }
                }
                else
                {
                    printf("[main] Error: capture/image processing failed\n");
                }
            }
            else
            {
                printf("[main] Error: video capture failed\n");
            }
        }

        stop_capturing(video_device);
    }

    uninit_video_device(video_device);
    close_video_device(video_device);
    deinit_utils();

    return 0;
}
#else
int main(int argc,char **argv)
{
    if(!init_utils())
    {
        printf("[main] Cannot init utility library\n");
        return 0;
    }

    if(argc!=2)
    {
        printf("[main] Usage: %s img_filename\n",argv[0]);
        goto Lend;
    }

    {
        image_t img(image_t::rgb_format,0,0);
        if(!load_image(img,argv[1]))
        {
            printf("[main] Error: cannot load image '%s'\n",argv[1]);
            goto Lend;
        }

        convert_rgb_to_yuv(img);
        process_image(img);
        detect_image(img);
        if(!save_image(img,"img.bmp"))
        {
            printf("[main] Error: image saving failed\n");
            goto Lend;
        }
    }
Lend:
    deinit_utils();

    return 0;
}
#endif
