#include "filter.hpp"

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
                            avg+=img.pixel(x+i,y+j)[III]+img.pixel(x-i,y-j)[III]+img.pixel(x-i,y+j)[III]+img.pixel(x+i,y-j)[III];
                avg/=(size*size-1);
                float etype=0;
                for(size_t i=1;i<shift;i++)
                    for(size_t j=1;j<shift;j++)
                        etype+=square(img.pixel(x+i,y+j)[III]-avg)+square(img.pixel(x-i,y-j)[III]-avg)+square(img.pixel(x-i,y+j)[III]-avg)+
                            square(img.pixel(x+i,y-j)[III]-avg);
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

/*
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

void filter_image(const image_t& img,image_t& res,filter_t filter)
{
    switch(filter)
    {
        case blur_filter:
            do_convolution(img,res,blur_kernel);
            break;
        case blur2_filter:
            do_convolution(img,res,blur2_kernel);
            break;
        case preswitt_filter:
            do_convolution(img,res,v_prewitt_kernel);
            do_convolution_ex(img,res,h_prewitt_kernel,add_equal_op);
            break;
        case roberts_filter:
            do_convolution(img,res,p_roberts_kernel);
            do_convolution_ex(img,res,m_roberts_kernel,add_equal_op);
            break;
        case sobel_filter:
            do_convolution(img,res,v_sobel_kernel);
            do_convolution_ex(img,res,h_sobel_kernel,add_equal_op);
            break;
        case kirsch_filter:
            do_convolution(img,res,v_kirsch_kernel);
            do_convolution_ex(img,res,h_kirsch_kernel,add_equal_op);
            break;
        case mdif_filter:
            do_convolution(img,res,v_mdif_kernel);
            do_convolution_ex(img,res,h_mdif_kernel,add_equal_op);
            break;
        case despeckle_filter:
            do_despeckle(img,res,0.5,5);
            break;
        default:
            break;
    }
}

