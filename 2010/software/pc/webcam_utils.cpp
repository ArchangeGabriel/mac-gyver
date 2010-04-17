#include "webcam_utils.hpp"
#include <IL/il.h>
#include <IL/ilu.h>

bool init_utils()
{
    if(ilGetInteger(IL_VERSION_NUM)<IL_VERSION || iluGetInteger(ILU_VERSION_NUM)<ILU_VERSION)
    {
        printf("[utils] Error: Your DevIL library is too old\n");
        return false;
    }
    ilInit();
    iluInit();
    ilEnable(IL_ORIGIN_SET);
    // note: opengl sets origin on lower left so actually lower left for opengl means upper left for image standard
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    return true;
}

void deinit_utils()
{
}

bool load_image(image_t& img,const std::string& name)
{
    ILuint img_id;
    ilGenImages(1,&img_id);
    ilBindImage(img_id);
    ILboolean res=ilLoadImage((char *)name.c_str());
    if(!res)
    {
        ilDeleteImages(1,&img_id);
        return false;
    }

    int w=ilGetInteger(IL_IMAGE_WIDTH);
    int h=ilGetInteger(IL_IMAGE_HEIGHT);
    img=image_t(image_t::rgb_format,w,h);
    uint8_t *data=new uint8_t[img.w*img.h*3];
    ilCopyPixels(0,0,0,w,h,1,IL_RGB,IL_UNSIGNED_BYTE,data);
    ilDeleteImages(1,&img_id);

    for(size_t y=0;y<img.h;y++)
        for(size_t x=0;x<img.w;x++)
            for(size_t i=0;i<3;i++)
                img.pixel(x,y)[i]=data[3*(y*img.w+x)+i];
    delete[] data;

    return res;
}

bool save_image(const image_t& img,const std::string& name)
{
    ILuint img_id;
    ilGenImages(1,&img_id);
    ilBindImage(img_id);
    uint8_t *data=new uint8_t[img.w*img.h*3];
    for(size_t y=0;y<img.h;y++)
        for(size_t x=0;x<img.w;x++)
            for(size_t i=0;i<3;i++)
                data[3*(y*img.w+x)+i]=img.pixel(x,y)[i];
    switch(img.fmt)
    {
        case image_t::rgb_format:
            ilTexImage(img.w,img.h,1,3,IL_RGB,IL_UNSIGNED_BYTE,data);
            break;
        case image_t::yuv_format:
            ilTexImage(img.w,img.h,1,3,IL_LUMINANCE,IL_UNSIGNED_BYTE,data);
            break;
        default:
            printf("[utils] Error: internal error\n");
            break;
    }
    delete[] data;
    ilEnable(IL_FILE_OVERWRITE);
    ILboolean res=ilSaveImage((char *)name.c_str());
    ilDeleteImages(1,&img_id);
    return res;
}
