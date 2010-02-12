#ifndef __processing__
#define __processing__

#include "common.hpp"
#include <stdexcept>

struct seg_decomp_t
{
    size_t x;
    size_t w;

    inline bool intersect(const seg_decomp_t& o) const
    {
        if(x<=o.x)
            return o.x<(x+w);
        else
            return x<(o.x+o.w);
    }
};

struct v_block_decomp_t
{
    size_t y;// first y
    std::vector<seg_decomp_t> seg;// list of starting x's and width', one for each y
};

struct h_decomp_t
{
    std::vector<v_block_decomp_t> blocks;
};

struct rgb_color_t
{
    uint8_t r,g,b;
    rgb_color_t():r(0),g(0),b(0){}
    rgb_color_t(uint8_t r,uint8_t g,uint8_t b):r(r),g(g),b(b){}
};

struct rectangle_t
{
    size_t x,y,w,h;

    size_t area() const
    {
        return w*h;
    }

    rectangle_t intersect(const rectangle_t& o) const
    {
        rectangle_t res;
        res.x=std::max(x,o.x);
        res.y=std::max(y,o.y);
        res.w=std::max(int(std::min(x+w,o.x+o.w))-int(res.x),0);
        res.h=std::max(int(std::min(y+h,o.y+o.h))-int(res.y),0);
        return res;
    }
};

struct __pos_t
{
    __pos_t():x(0),y(0){}
    __pos_t(size_t x,size_t y):x(x),y(y){}

    size_t x,y;
};

struct hv_segment_t
{
    __pos_t orig;
    __pos_t end;

    enum dir_t
    {
        up,down,left,right
    };

    dir_t dir;
    size_t len;
};

struct edge_detection_t
{
    std::vector<hv_segment_t> left;
    std::vector<hv_segment_t> right;
};

struct unproject_parameters_t
{
    size_t screen_w;
    double half_fov;
    double object_radius;
};

struct unproject_result_t
{
    double x,y;
};

typedef bool (*filter_func_t)(const image_t::pixel_type_t *pix);

bool filter_red(const image_t::pixel_type_t *pix);
bool filter_green(const image_t::pixel_type_t *pix);

// do a rgb to yuv conversion
bool convert_rgb_to_yuv(image_t& img);
bool convert_rgb_to_yuv(image_t::pixel_type_t *pix);
// the inverse
bool convert_yuv_to_rgb(image_t& img);
bool convert_yuv_to_rgb(image_t::pixel_type_t *pix);
// do standard processing on a yuv image and gives the result as a rgb image
bool process_image(image_t& img,filter_func_t fn,const rgb_color_t& res);
// do an horizontal lines decomposition(also eliminates some artecfacts to keeps only relevant lines)
bool do_h_decomposition(image_t& img,h_decomp_t& hld,const rgb_color_t& col);
// take a block and extract a rectangle(if possible) from it
// delta_h is the maximum difference of x's in a border [assume 0 difference in y because of vertical decomposition]
// DONT CLEAR r ON CALL, this allow to filter against already found rectangles
typedef bool (*rectangle_filter_t)(const rectangle_t& r);
typedef void (*rectangle_comp_t)(const rectangle_t& r1,bool& keep1,const rectangle_t& r2,bool& keep2);
bool extract_rectangles(const v_block_decomp_t& block,std::vector<rectangle_t>& r,size_t delta_x,
    rectangle_filter_t rf,rectangle_comp_t rc);

edge_detection_t do_approx_edge_detection(const v_block_decomp_t& block,size_t h_tolerance,size_t min_w);

// (low-level) do an horizontal line decomposition
bool do_h_line_decomposition(const image_t& img,size_t y,const rgb_color_t& col,std::vector<seg_decomp_t>& res);

// (high-level)
std::vector<rectangle_t> detect_rectangles(image_t& img,filter_func_t fn,size_t tolerance=10);
std::vector<rectangle_t> detect_rectangles2(image_t& img,filter_func_t fn,float min_density=0.75);

unproject_result_t unproject(const rectangle_t& rect,const unproject_parameters_t& param);

// determine whether a rectangle region a density of pixel that satisfied <fn> that is greater than <percent>%
bool is_rectangle_of_color(const image_t& img,const rectangle_t& rect,filter_func_t fn,int percent);


#endif // __processing__
