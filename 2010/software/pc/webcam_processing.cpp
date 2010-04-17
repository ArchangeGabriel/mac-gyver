#include "webcam_processing.hpp"

bool filter_red(const image_t::pixel_type_t *pix)
{
    const image_t::pixel_type_t Y=pix[0];
    const image_t::pixel_type_t U=pix[1];
    const image_t::pixel_type_t V=pix[2];
    // detect red
    if((U<120 && V>170 && Y<256) || (V>(U+90)))        return true;
    else
        return false;
}

bool filter_green(const image_t::pixel_type_t *pix)
{
    image_t::pixel_type_t pix2[3];
    for(int i=0;i<3;i++)
        pix2[i]=pix[i];
    convert_yuv_to_rgb(pix2);
    
    return pix2[1]>=70 && pix2[1]>=(pix2[0]+20) && pix2[1]>=(pix2[2]+20);
}

bool convert_rgb_to_yuv(image_t::pixel_type_t *pix)
{
    int R=pix[0];
    int G=pix[1];
    int B=pix[2];
    pix[0] = clamp( 0, 255, ( (  66 * R + 129 * G +  25 * B + 128) >> 8) +  16 );
    pix[1] = clamp( 0, 255, ( ( -38 * R -  74 * G + 112 * B + 128) >> 8) + 128 );
    pix[2] = clamp( 0, 255, ( ( 112 * R -  94 * G -  18 * B + 128) >> 8) + 128 );
    
    return true;
}

bool convert_rgb_to_yuv(image_t& img)
{
    assert(img.fmt==image_t::rgb_format);

    for(size_t y=0;y<img.h;y++)
    {
        for(size_t x=0;x<img.w;x++)
        {
            convert_rgb_to_yuv(img.pixel(x,y));
        }
    }
    img.fmt=image_t::yuv_format;
    return true;
}

bool convert_yuv_to_rgb(image_t::pixel_type_t *pix)
{
    uint8_t Y=16+pix[0]/255.0*(235.0-16.0);
    uint8_t U=16+pix[1]/255.0*(240.0-16.0);
    uint8_t V=16+pix[2]/255.0*(240.0-16.0);
    #define r_from_yuv(c,d,e) clamp(0,255,((298*c+409*e+128)>>8)+16-16)
    #define g_from_yuv(c,d,e) clamp(0,255,((298*c-100*d-208*e+128)>>8)+128-128)
    #define b_from_yuv(c,d,e) clamp(0,255,((298*c+516*d+128)>>8)+128-128)
    int c=int(Y)-16;
    int d=int(U)-128;
    int e=int(V)-128;
    pix[0]=r_from_yuv(c,d,e);
    pix[1]=g_from_yuv(c,d,e);
    pix[2]=b_from_yuv(c,d,e);
    
    return true;
}

bool convert_yuv_to_rgb(image_t& img)
{
    assert(img.fmt==image_t::yuv_format);

    for(size_t y=0;y<img.h;y++)
    {
        for(size_t x=0;x<img.w;x++)
        {
            convert_yuv_to_rgb(img.pixel(x,y));
        }
    }
    img.fmt=image_t::rgb_format;
    return true;
}

bool process_image(image_t& img,filter_func_t fn,const rgb_color_t& res)
{
    assert(img.fmt==image_t::yuv_format);

    for(size_t y=0;y<img.h;y++)
    {
        for(size_t x=0;x<img.w;x++)
        {
            if(fn(img.pixel(x,y)))
            {
                img.pixel(x,y)[0]=res.r;
                img.pixel(x,y)[1]=res.g;
                img.pixel(x,y)[2]=res.b;
            }
            else
            {
                img.pixel(x,y)[0]=0;
                img.pixel(x,y)[1]=0;
                img.pixel(x,y)[2]=0;
            }
        }
    }
    img.fmt=image_t::rgb_format;

    return true;
}

template<typename T>
inline bool equal(const rgb_color_t& col,const T* ptr)
{
    return col.r==ptr[0] && col.g==ptr[1] && col.b==ptr[2];
}

#if 1
bool do_h_line_decomposition(const image_t& img,size_t y,const rgb_color_t& col,std::vector<seg_decomp_t>& res)
{
    res.clear();
    bool in_seg=false;
    seg_decomp_t cur_seg;

    for(size_t x=0;x<img.w;x++)
    {
        if(equal(col,img.pixel(x,y)))
        {
            if(!in_seg)
            {
                in_seg=true;
                cur_seg.x=x;
            }
        }
        else
        {
            if(in_seg)
            {
                in_seg=false;
                cur_seg.w=x-cur_seg.x;
                res.push_back(cur_seg);
            }
        }
    }

    return true;
}
#else
bool do_h_line_decomposition(image_t& img,size_t y,const rgb_color_t& col,std::vector<seg_decomp_t>& res)
{
    const double min_density=0.7;
    
    res.clear();
    bool in_seg=false;
    seg_decomp_t cur_seg;
    size_t cur_seg_nb_ok=0;

    for(size_t x=0;x<img.w;x++)
    {
        if(equal(col,img.pixel(x,y)))
        {
            if(!in_seg)
            {
                in_seg=true;
                cur_seg.x=x;
                cur_seg_nb_ok=1;
            }
        }
        else
        {
            if(in_seg)
            {
                double new_density=cur_seg_nb_ok/(x-cur_seg.x+1);
                if(new_density<min_density)
                {
                    in_seg=false;
                    cur_seg.w=x-cur_seg.x;
                    res.push_back(cur_seg);
                }
                else
                {
                    //*
                    img.pixel(x,y)[0]=col.r;
                    img.pixel(x,y)[1]=col.g;
                    img.pixel(x,y)[2]=col.b;
                    //*/
                }
            }
        }
    }
    
    if(in_seg)
    {
        cur_seg.w=img.w-cur_seg.x;
        res.push_back(cur_seg);
    }

    return true;
}
#endif

template<typename T>
void fast_vector_erase(std::vector<T>& v,size_t pos)
{
    if(pos!=(v.size()-1))
        std::swap(v[pos],v[v.size()-1]);
    v.pop_back();
}

bool seg_comp(const seg_decomp_t& s1,const seg_decomp_t& s2)
{
    return s1.w>s2.w;
}

bool do_h_decomposition(image_t& img,h_decomp_t& hld,const rgb_color_t& col)
{
    // maintain a list of finished blocks and opened blocks
    hld.blocks.clear();// this is the closed list
    std::vector<v_block_decomp_t>& close_list=hld.blocks;
    std::vector<v_block_decomp_t> open_list;
    std::vector<seg_decomp_t> line_dec;// use for the decomposition of each line
    std::vector<bool> line_dec_used;

    // do it line by line
    for(size_t y=0;y<img.h;y++)
    {
        // decompose line
        if(!do_h_line_decomposition(img,y,col,line_dec))
            return false;
        line_dec_used.clear();
        line_dec_used.resize(line_dec.size(),false);
        // sort by size
        std::sort(line_dec.begin(),line_dec.end(),seg_comp);
        // for each block try to find a consecutive segment(note: a segment can belong to several blocks)
        for(int i=0;i<int(open_list.size());i++)
        {
            bool find=false;
            v_block_decomp_t& block=open_list[i];
            for(size_t j=0;j<line_dec.size();j++)
            {
                if(block.seg[block.seg.size()-1].intersect(line_dec[j]))
                {
                    find=true;
                    line_dec_used[j]=true;
                    block.seg.push_back(line_dec[j]);
                    break;
                }
            }
            // if not found, close it
            if(!find)
            {
                close_list.push_back(block);
                fast_vector_erase(open_list,i);
                i--;
                continue;
            }
        }
        // search for unmatched segments
        for(size_t i=0;i<line_dec.size();i++)
        {
            if(line_dec_used[i])
                continue;
            // begin a new block
            v_block_decomp_t block;
            block.y=y;
            block.seg.push_back(line_dec[i]);
            open_list.push_back(block);
        }
    }
    // close opended blocks
    for(size_t i=0;i<open_list.size();i++)
        close_list.push_back(open_list[i]);

    return true;
}

void update_me(size_t& lmix,size_t& lmax,size_t& rmix,size_t& rmax,size_t new_x,size_t new_w)
{
    lmix=std::min(lmix,new_x);
    lmax=std::max(lmax,new_x);
    rmix=std::min(rmix,new_x+new_w);
    rmax=std::max(rmax,new_x+new_w);
}

bool fit_in_rules(size_t lmix,size_t lmax,size_t rmix,size_t rmax,size_t new_x,size_t new_w,size_t delta_x)
{
    update_me(lmix,lmax,rmix,rmax,new_x,new_w);
    if((lmax-lmix)>delta_x || (rmax-rmix)>delta_x)
        return false;
    else
        return true;
}

bool extract_rectangles(const v_block_decomp_t& block,std::vector<rectangle_t>& r,size_t delta_x,
    rectangle_filter_t rf,rectangle_comp_t rc)
{
    // brute force
    for(size_t y=0;y<block.seg.size();y++)
    {
        size_t left_min_x=block.seg[y].x;
        size_t left_max_x=block.seg[y].x;
        size_t right_min_x=block.seg[y].x+block.seg[y].w;
        size_t right_max_x=block.seg[y].x+block.seg[y].w;

        size_t oy=y+1;
        while(oy<block.seg.size() &&
            fit_in_rules(left_min_x,left_max_x,right_min_x,right_max_x,
                        block.seg[oy].x,block.seg[oy].w,delta_x))
        {
            update_me(left_min_x,left_max_x,right_min_x,right_max_x,
                        block.seg[oy].x,block.seg[oy].w);
            oy++;
        }
        // we now have a beautiful rectangle:
        size_t avmix=(left_min_x+left_max_x)/2;
        size_t avmax=(right_min_x+right_max_x)/2;
        rectangle_t temp;
        temp.x=avmix;
        temp.y=y+block.y;
        temp.w=avmax-avmix;
        temp.h=oy-y;

        // filter it
        if(!rf(temp))
            continue;
        // compare it to others
        bool keep=true;
        for(int i=0;i<int(r.size());i++)
        {
            bool k1,k2;
            rc(r[i],k1,temp,k2);
            if(!k2)
            {
                keep=false;
                break;
            }
            if(!k1)
            {
                fast_vector_erase(r,i);
                i--;
            }
        }
        if(keep)
            r.push_back(temp);
    }

    return true;
}

struct border_analysis_t
{
    __pos_t anchor,current;
    size_t min_x,max_x;
    size_t x_tolerance;
    std::vector<__pos_t> path;

    void begin(__pos_t pos,size_t xt)
    {
        x_tolerance=xt;
        min_x=max_x=pos.x;
        anchor=pos;
        current=pos;
    }

    void put(__pos_t p)
    {
        assert(p.y==(current.y+1));
        size_t new_min_x=std::min(min_x,p.x);
        size_t new_max_x=std::max(max_x,p.x);
        if((new_max_x-new_min_x)<=x_tolerance)
        {
            min_x=new_min_x;
            max_x=new_max_x;
            current=p;
        }
        else
        {
            size_t avg_x=(max_x+min_x)/2;
            anchor.x=avg_x;
            path.push_back(anchor);
            anchor.x=avg_x;
            anchor.y=p.y;
            path.push_back(anchor);

            begin(p,x_tolerance);
        }
    }

    void end()
    {
        size_t avg_x=(max_x+min_x)/2;
        anchor.x=avg_x;
        current.x=avg_x;
        path.push_back(anchor);
        if(anchor.y!=current.y)
            path.push_back(current);
    }

    std::vector<hv_segment_t> compute_edges()
    {
        __pos_t cur=path[0];
        std::vector<hv_segment_t> res;

        for(size_t i=1;i<path.size();i++)
        {
            __pos_t end=path[i];
            if(cur.x==end.x)
            {
                hv_segment_t s;
                s.orig=cur;
                s.end=end;
                s.dir=hv_segment_t::down;
                s.len=end.y-cur.y+1;
                res.push_back(s);

                cur=end;
            }
            else if(cur.y==end.y)
            {
                hv_segment_t s;
                s.orig=cur;
                s.end=end;

                if(end.x<cur.x)
                {
                    s.dir=hv_segment_t::left;
                    s.len=(cur.x-end.x)+1;
                }
                else
                {
                    s.dir=hv_segment_t::right;
                    s.len=(end.x-cur.x)+1;
                }
                res.push_back(s);

                cur=end;
            }
            else
            {
                assert(false);
                throw new std::runtime_error("internal error in compute_edges");
            }
        }

        return res;
    }
};

edge_detection_t do_approx_edge_detection(const v_block_decomp_t& block,size_t h_tolerance,size_t min_w)
{
    edge_detection_t ed;
    border_analysis_t left,right;
    //
    size_t i=0;
    while(i<block.seg.size() && i<block.seg.size() && block.seg[i].w<=min_w)
        i++;
    if(i==block.seg.size())
        return ed;
    
    left.begin(__pos_t(block.seg[i].x,block.y+i),h_tolerance);
    right.begin(__pos_t(block.seg[i].x+block.seg[i].w,block.y+i),h_tolerance);
    i++;
    for(;i<block.seg.size();i++)
    {
        left.put(__pos_t(block.seg[i].x,block.y+i));
        right.put(__pos_t(block.seg[i].x+block.seg[i].w,block.y+i));
    }

    left.end();
    right.end();

    ed.left=left.compute_edges();
    ed.right=right.compute_edges();
    return ed;
}

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

std::vector<rectangle_t> detect_rectangles(image_t& img,filter_func_t fn,size_t tolerance)
{
    h_decomp_t hd;
    std::vector<rectangle_t> rects;
    rgb_color_t col(255,0,0);
    
    if(!process_image(img,fn,col))
        return rects;
    
    if(!do_h_decomposition(img,hd,col))
        return rects;

    std::vector<edge_detection_t> edges;

    //printf("[processing] apply approximate H/V edge detection with tolerance %u\n",tolerance);
    for(size_t i=0;i<hd.blocks.size();i++)
    {
        const size_t edge_detection_min_w=50;
        const v_block_decomp_t& b=hd.blocks[i];
        edge_detection_t ed=do_approx_edge_detection(b,tolerance,edge_detection_min_w);
        //printf("[processing] block %u: filtering artecfacts...",i);
        filter_edge_detection(ed.left);
        filter_edge_detection(ed.right);
        //printf("[processing] %u segment left/%u segment right\n",ed.left.size(),ed.right.size());

        if(ed.left.size()==1 && ed.right.size()==1)
        {
            edges.push_back(ed);
            continue;
        }
        //printf("[processing block %u: complex decomposition: splitting...",i);
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
        //printf("[processing] got %u pieces\n",nb_pieces);
    }

    for(size_t i=0;i<edges.size();i++)
    {
        const edge_detection_t ed=edges[i];

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
            //printf("[processing] perfect_div=%f delta=%f\n",perfect_division,delta);
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
            //printf("[processing] got non-standard edge decomposition: %u left, %u right. Give up...\n",ed.left.size(),ed.right.size());
        }
    }
    
    return rects;
}

unproject_result_t unproject(const rectangle_t& rect,const unproject_parameters_t& param)
{
    int rel_x_beta=rect.x;
    rel_x_beta-=param.screen_w/2;
    int rel_x_alpha=rect.x+rect.w;
    rel_x_alpha-=param.screen_w/2;
    rel_x_alpha=-rel_x_alpha;// axis inversion
    rel_x_beta=-rel_x_beta;// ditto
    
    float tan_f=tan(param.half_fov);
    float factor=2.0*tan_f/param.screen_w;
    float alpha=atan(rel_x_alpha*factor);
    float beta=atan(rel_x_beta*factor);
    
    printf("alpha=%f beta=%f\n",alpha,beta);
    
    unproject_result_t res;
    res.x=-param.object_radius*(sin(alpha)+sin(beta))/sin(beta-alpha);
    res.y=param.object_radius*(cos(alpha)+cos(beta))/sin(beta-alpha);
    
    return res;
}

bool is_rectangle_of_color(const image_t& img,const rectangle_t& rect,filter_func_t fn,int percent)
{
    int nr=0;
    
    for(size_t dy=0;dy<rect.h;dy++)
        for(size_t dx=0;dx<rect.w;dx++)
            if(fn(img.pixel(rect.x+dx,rect.y+dy)))
                nr++;
                
    int p=(nr*100)/(rect.w*rect.h);
    
    return p>=percent;
}

std::vector<rectangle_t> detect_rectangles2(image_t& img,filter_func_t fn,float min_density)
{
    h_decomp_t hd;
    std::vector<rectangle_t> rects;
    rgb_color_t col(255,0,0);
    std::vector< std::vector<int> > nb_ok_pixels;
    
    if(!process_image(img,fn,col))
        return rects;
    
    {
        nb_ok_pixels.resize(img.h);
        for(size_t y=0;y<img.h;y++)
        {
            for(size_t x=0;x<img.w;x++)
            {
                int nb_pixels=0;
                
                if(x>0)
                    nb_pixels+=nb_ok_pixels[y][x-1];
                if(y>0)
                    nb_pixels+=nb_ok_pixels[y-1][x];
                if(x>0 && y>0)
                    nb_pixels-=nb_ok_pixels[y-1][x-1];
                
                if(equal(col,img.pixel(x,y)))
                    nb_pixels++;
                nb_ok_pixels[y].push_back(nb_pixels);
            }
        }
    }
    
    
    if(!do_h_decomposition(img,hd,col))
        return rects;
    
    for(size_t block=0;block<hd.blocks.size();block++)
    {
        __pos_t min,max;
        min.x=img.w;
        min.y=img.h;
        max.x=0;
        max.y=0;
        
        for(size_t dy=0;dy<hd.blocks[block].seg.size();dy++)
        {
            seg_decomp_t seg=hd.blocks[block].seg[dy];
            __pos_t seg_min(seg.x,hd.blocks[block].y+dy);
            __pos_t seg_max(seg.x+seg.w,hd.blocks[block].y+dy);
            
            min.x=std::min(min.x,seg_min.x);
            min.y=std::min(min.y,seg_min.y);
            
            max.x=std::max(max.x,seg_max.x);
            max.y=std::max(max.y,seg_max.y);
        }
        
        int nb_pixels=nb_ok_pixels[max.y][max.x]+nb_ok_pixels[min.y-1][min.x-1]-nb_ok_pixels[min.y-1][max.x]-nb_ok_pixels[max.y][min.x-1];
        int area=(max.x-min.x+1)*(max.y-min.y+1);
        
        //printf("[processing] new bbox: (%d,%d)->(%d,%d) nb_pixels=%d area=%d\n",min.x,min.y,max.x,max.y,nb_pixels,area);
        
        /*
        for(size_t x=min.x;x<=max.x;x++)
        {
            for(size_t jj=0;jj<3;jj++)
            {
                img.pixel(x,min.y)[jj]=255;
                img.pixel(x,max.y)[jj]=255;
            }
        }
        
        for(size_t y=min.y;y<=max.y;y++)
        {
            for(size_t jj=0;jj<3;jj++)
            {
                img.pixel(min.x,y)[jj]=255;
                img.pixel(max.x,y)[jj]=255;
            }
        }
        //*/
        
        if(nb_pixels<200)
            continue;
        
        float density=float(nb_pixels)/float(area);
        
        //printf("[processing] density=%f\n",density);
        
        if(density>=min_density)
        {
            rectangle_t r;
            r.x=min.x;
            r.y=min.y;
            r.w=max.x-min.x+1;
            r.h=max.y-min.y+1;
            rects.push_back(r);
        }
    }
    
    return rects;
}


