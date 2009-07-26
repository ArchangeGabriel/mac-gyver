#include "common.hpp"
#include "processing.hpp"
#include "utils.hpp"
#include <cstdio>
#include <cstdlib>

struct opt_t
{
    bool has_help;
    bool has_outfile;
    const char *outfile;
    bool has_infile;
    const char *infile;
    bool has_dumpuvplane;
    int uvplane_y;
    bool has_filter;
    const char *filter_colors;
    bool has_extractplane;
    const char *extractplane;
    bool has_dumpfilter;
    const char *dumpfilter;

    bool error;
}g_opt;

void print_help()
{
    printf("usage: exec <switches>\n");
    printf("Switch:\n");
    printf("--help\tDisplay this help\n");
    printf("--dumpuvplane <Y>\tOutput a UV plane with a constant Y\n");
    printf("--filter <colors>\tFilter colors: <colors> is a list of colors\n");
    printf("\t\t\tColors: red,green,blue,white\n");
    printf("-o <out_file>\t\tSelect output file\n");
    printf("-i <in_file>\t\tSelect input file\n");
    printf("--extractplane <P>\tExtract plane: Y,U or V\n");
    printf("--dumpfilter <color>\tOutput all colors accepted by the filter\n");
}

bool read_clamped_integer(const char *str,int min,int max,int& dst)
{
    dst=0;
    bool neg=false;
    if(*str=='-')
    {
        neg=true;
        str++;
    }

    while(*str)
    {
        if((*str)>='0' && (*str)<='9')
            dst=dst*10+(*str++)-'0';
        else
            return false;
    }

    if(neg)
        dst=-dst;
    return dst>=min && dst<=max;
}

void dump_uvplane()
{
    if(!g_opt.has_outfile)
    {
        printf("[dump_uvplane] Error: you need to specify a destination file\n");
        return;
    }

    printf("[dump_uvplane] Dumping UV plane with Y=%d to '%s'\n",g_opt.uvplane_y,g_opt.outfile);

    image_t img(image_t::yuv_format,256,256);
    for(size_t u=0;u<256;u++)
        for(size_t v=0;v<256;v++)
        {
            img.pixel(u,v)[0]=g_opt.uvplane_y;
            img.pixel(u,v)[1]=u;
            img.pixel(u,v)[2]=v;
        }

    if(!convert_yuv_to_rgb(img))
    {
        printf("[dump_uvplane] Error converting from YUV to RGB\n");
        return ;
    }

    if(!save_image(img,std::string(g_opt.outfile)))
    {
        printf("[dump_uvplane] Cannot save output image\n");
        return;
    }
}

typedef bool (*filter_func_t)(image_t::pixel_type_t *pix);

bool filter_red(image_t::pixel_type_t *pix)
{
    const image_t::pixel_type_t Y=pix[0];
    const image_t::pixel_type_t U=pix[1];
    const image_t::pixel_type_t V=pix[2];
    // detect red
    if((U<120 && V>170 && Y<256) || (V>(U+90)))
    {
        pix[0]=255;
        pix[1]=0;
        pix[2]=0;
        return true;
    }
    else
        return false;
}

bool filter_white(image_t::pixel_type_t *pix)
{
    const image_t::pixel_type_t Y=pix[0];
    const image_t::pixel_type_t U=pix[1];
    const image_t::pixel_type_t V=pix[2];
    // detect white
    if(U>=100 && U<=150 && V>=100 && V<=150 && Y>=170)
    {
        pix[0]=255;
        pix[1]=255;
        pix[2]=255;
        return true;
    }
    else
        return false;
}

bool filter_blue(image_t::pixel_type_t *pix)
{
    const image_t::pixel_type_t Y=pix[0];
    const image_t::pixel_type_t U=pix[1];
    const image_t::pixel_type_t V=pix[2];
    // detect blue
    if(U>=150 && U<=200 && V>=80 && V<=120 && Y>=100 && Y<=150)
    {
        pix[0]=0;
        pix[1]=0;
        pix[2]=255;
        return true;
    }
    else
        return false;
}

bool filter_green(image_t::pixel_type_t *pix)
{
    const image_t::pixel_type_t Y=pix[0];
    const image_t::pixel_type_t U=pix[1];
    const image_t::pixel_type_t V=pix[2];
    // detect green
    if(U>=70 && U<=120 && V>=70 && V<=120 && Y>=100 && Y<=200)
    {
        pix[0]=0;
        pix[1]=255;
        pix[2]=0;
        return true;
    }
    else
        return false;
}

struct named_color_d
{
    std::string name;
    filter_func_t func;
    bool use;
}named_filters[]=
{
    {"red",&filter_red},
    {"white",&filter_white},
    {"blue",&filter_blue},
    {"green",&filter_green}
};

#define STATIC_SIZEOF(tab) (sizeof(tab)/sizeof((tab)[0]))

void filter_colors()
{
    if(!g_opt.has_outfile)
    {
        printf("[filter_colors] Error: you need to specify a destination file\n");
        return;
    }
    if(!g_opt.has_infile)
    {
        printf("[filter_colors] Error: you need to specify a source file\n");
        return;
    }
    // parse colors
    {
        std::string str=g_opt.filter_colors;
        size_t last=0;
        for(size_t cur=0;cur<=str.size();cur++)
        {
            if(cur==str.size() || str[cur]==',')
            {
                size_t len=cur-last;
                bool ok=false;

                for(size_t i=0;i<STATIC_SIZEOF(named_filters);i++)
                {
                    if(named_filters[i].name==str.substr(last,len))
                    {
                        if(named_filters[i].use)
                            printf("[filter_colors] Warning: redundant color '%s'\n",named_filters[i].name.c_str());
                        named_filters[i].use=true;
                        ok=true;
                        break;
                    }
                }
                if(!ok)
                    printf("[filter_colors] Warning: ignoring unknown named filter '%s'\n",str.substr(last,len).c_str());
                last=cur+1;
            }
        }
    }

    printf("Filtering color(s) ");
    bool first=true;
    for(size_t i=0;i<STATIC_SIZEOF(named_filters);i++)
    {
        if(!named_filters[i].use)
            continue;
        if(!first)
            printf(",");
        else
            first=false;
        printf("%s",named_filters[i].name.c_str());
    }
    printf(" of image '%s' to '%s'\n",g_opt.infile,g_opt.outfile);

    // read
    image_t img(image_t::rgb_format,0,0);
    if(!load_image(img,g_opt.infile))
    {
        printf("[filter_colors] Cannot load image\n");
        return;
    }
    if(!convert_rgb_to_yuv(img))
    {
        printf("[filter_colors] Cannot convert image to YUV\n");
        return;
    }
    // filter
    for(size_t y=0;y<img.h;y++)
        for(size_t x=0;x<img.w;x++)
        {
            bool filtered=false;
            for(size_t i=0;i<STATIC_SIZEOF(named_filters);i++)
            {
                if(named_filters[i].use && named_filters[i].func(img.pixel(x,y)))
                {
                    filtered=true;
                    break;
                }
            }
            if(!filtered)
                img.pixel(x,y)[0]=img.pixel(x,y)[1]=img.pixel(x,y)[2]=0;
        }
    img.fmt=image_t::rgb_format;
    // write
    if(!save_image(img,std::string(g_opt.outfile)))
    {
        printf("[filter_colors] Cannot save output image\n");
        return;
    }
}

void extract_plane()
{
    if(!g_opt.has_outfile)
    {
        printf("[extract_plane] Error: you need to specify a destination file\n");
        return;
    }
    if(!g_opt.has_infile)
    {
        printf("[extract_plane] Error: you need to specify a source file\n");
        return;
    }

    size_t plane_index=0;
    if(strcmp(g_opt.extractplane,"Y")==0)
        plane_index=0;
    else if(strcmp(g_opt.extractplane,"U")==0)
        plane_index=1;
    else if(strcmp(g_opt.extractplane,"V")==0)
        plane_index=2;
    else
    {
        printf("[extract_plane] Error: unknwon plane '%s'\n",g_opt.extractplane);
        return;
    }

    printf("[extract_plane] Extracting plane %d(%s) of image '%s' to '%s'\n",
        plane_index,g_opt.extractplane,g_opt.infile,g_opt.outfile);
    // load
    image_t img(image_t::rgb_format,0,0);
    if(!load_image(img,g_opt.infile))
    {
        printf("[extract_plane] Cannot load image\n");
        return;
    }
    if(!convert_rgb_to_yuv(img))
    {
        printf("[extract_plane] Cannot convert image to YUV\n");
        return;
    }
    // extract
    for(size_t y=0;y<img.h;y++)
        for(size_t x=0;x<img.w;x++)
        {
            image_t::pixel_type_t t=img.pixel(x,y)[plane_index];
            img.pixel(x,y)[0]=img.pixel(x,y)[1]=img.pixel(x,y)[2]=t;
        }

    img.fmt=image_t::rgb_format;
    // write
    if(!save_image(img,std::string(g_opt.outfile)))
    {
        printf("[filter_colors] Cannot save output image\n");
        return;
    }
}

void dump_filter()
{
    if(!g_opt.has_outfile)
    {
        printf("[extract_plane] Error: you need to specify a destination file\n");
        return;
    }
    //
    size_t filter_index=STATIC_SIZEOF(named_filters);
    for(size_t i=0;i<STATIC_SIZEOF(named_filters);i++)
        if(named_filters[i].name==g_opt.dumpfilter)
            filter_index=i;
    if(filter_index==STATIC_SIZEOF(named_filters))
    {
        printf("[dump_filter] Error: unknown named filter '%s'\n",g_opt.dumpfilter);
        return;
    }
    // first compute the number of values
    size_t count=0;
    image_t::pixel_type_t pix[3];
    for(size_t Y=0;Y<256;Y++)
        for(size_t U=0;U<256;U++)
            for(size_t V=0;V<256;V++)
            {
                pix[0]=Y;pix[1]=U;pix[2]=V;
                if(named_filters[filter_index].func(pix))
                    count++;
            }
    printf("[dump_filter] Dumping filter %d(%s) to '%s': %u colors accepted\n",
        filter_index,g_opt.dumpfilter,g_opt.outfile,count);
    size_t img_size=0;
    while((img_size*img_size)<count)
        img_size++;
    // fill
    image_t img(image_t::yuv_format,img_size,img_size);
    size_t pos=0;
    for(size_t Y=0;Y<256;Y++)
        for(size_t U=0;U<256;U++)
            for(size_t V=0;V<256;V++)
            {
                pix[0]=Y;pix[1]=U;pix[2]=V;
                if(named_filters[filter_index].func(pix))
                {
                    size_t x=pos%img.w;
                    size_t y=pos/img.w;
                    img.pixel(x,y)[0]=Y;
                    img.pixel(x,y)[1]=U;
                    img.pixel(x,y)[2]=V;
                    pos++;
                }
            }
    while(pos<(img_size*img_size))
    {
        size_t x=pos%img.w;
        size_t y=pos/img.w;
        img.pixel(x,y)[0]=img.pixel(x,y)[1]=img.pixel(x,y)[2]=0;
        pos++;
    }
    // conver
    if(!convert_yuv_to_rgb(img))
    {
        printf("[dump_filter] Error: cannot convert image to RGB\n");
        return ;
    }
    // write
    if(!save_image(img,std::string(g_opt.outfile)))
    {
        printf("[filter_colors] Cannot save output image\n");
        return;
    }
}

int main(int argc,char **argv)
{
    if(!init_utils())
    {
        printf("[main] Cannot initialize utils library\n");
        return 0;
    }
    atexit(deinit_utils);

    if(argc==1)
    {
        print_help();
        return 0;
    }

    for(int i=1;i<argc;i++)
    {
        if(strcmp(argv[i],"--help")==0)
        {
            if(g_opt.has_help)
                printf("[opt_parser] Warning: redundant --help switch\n");
            g_opt.has_help=true;
        }
        else if(strcmp(argv[i],"--dumpuvplane")==0)
        {
            if(i==(argc-1))
            {
                printf("[opt_parser] Error: missing <Y> for --dumpuvplane\n");
                g_opt.error=true;
            }
            else
            {
                i++;
                g_opt.has_dumpuvplane=true;
                if(!read_clamped_integer(argv[i],0,255,g_opt.uvplane_y))
                {
                    printf("[opt_parser] Error: invalid <Y> for --dumpuvplane\n");
                    g_opt.error=true;
                }
            }
        }
        else if(strcmp(argv[i],"-o")==0)
        {
            if(i==(argc-1))
            {
                printf("[opt_parser] Error: missing <out_file> for -o\n");
                g_opt.error=true;
            }
            else
            {
                i++;
                g_opt.has_outfile=true;
                g_opt.outfile=argv[i];
            }
        }
        else if(strcmp(argv[i],"-i")==0)
        {
            if(i==(argc-1))
            {
                printf("[opt_parser] Error: missing <in_file> for -i\n");
                g_opt.error=true;
            }
            else
            {
                i++;
                g_opt.has_infile=true;
                g_opt.infile=argv[i];
            }
        }
        else if(strcmp(argv[i],"--filter")==0)
        {
            if(i==(argc-1))
            {
                printf("[opt_parser] Error: missing <colors> for --filter\n");
                g_opt.error=true;
            }
            else
            {
                i++;
                g_opt.has_filter=true;
                g_opt.filter_colors=argv[i];
            }
        }
        else if(strcmp(argv[i],"--extractplane")==0)
        {
            if(i==(argc-1))
            {
                printf("[opt_parser] Error: missing <P> for --extractplane\n");
                g_opt.error=true;
            }
            else
            {
                i++;
                g_opt.has_extractplane=true;
                g_opt.extractplane=argv[i];
            }
        }
        else if(strcmp(argv[i],"--dumpfilter")==0)
        {
            if(i==(argc-1))
            {
                printf("[opt_parser] Error: missing <color> for --dumpfilter\n");
                g_opt.error=true;
            }
            else
            {
                i++;
                g_opt.has_dumpfilter=true;
                g_opt.dumpfilter=argv[i];
            }
        }
        else
        {
            printf("[opt_parser] Error: unknown switch '%s'\n",argv[i]);
            g_opt.error=true;
        }
    }

    if(g_opt.error)
    {
        printf("[opt_parser] Errors encountered during parsing: run the program with --help to check the syntax\n");
        return 0;
    }

    if(g_opt.has_help)
    {
        print_help();
        return 0;
    }

    if(g_opt.has_dumpuvplane)
    {
        dump_uvplane();
        return 0;
    }

    if(g_opt.has_filter)
    {
        filter_colors();
        return 0;
    }

    if(g_opt.has_extractplane)
    {
        extract_plane();
        return 0;
    }

    if(g_opt.has_dumpfilter)
    {
        dump_filter();
        return 0;
    }

    return 0;
}
