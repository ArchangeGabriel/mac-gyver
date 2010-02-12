#include "webcam.hpp"

webcam_t::webcam_t(const std::string& dev,size_t w,size_t h)
    :m_width(w),m_height(h),m_image(image_t::yuv_format,0,0)
{
    open(dev);
    init();
}

webcam_t::~webcam_t()
{
    deinit();
}

void webcam_t::start()
{
    start_capturing();
    
    m_image=image_t(image_t::yuv_format,m_width,m_height);
}

void webcam_t::stop()
{
    stop_capturing();
}

size_t webcam_t::get_width()
{
    return m_width;
}

size_t webcam_t::get_height()
{
    return m_height;
}

void webcam_t::capture(image_t& img)
{
    do_capture();
    get_image(img);
}

void webcam_t::do_capture()
{
    video_capture();
    process_capture_to_image(m_image);
}

void webcam_t::get_image(image_t& img)
{
    img=m_image;
}

const image_t& webcam_t::get_image2() const
{
    return m_image;
}

int webcam_t::xioctl(int request,void *arg)
{
    int r;

    do
        r=ioctl(m_fd,request,arg);
    while(-1==r && EINTR==errno);

    return r;
}

void webcam_t::open(const std::string& dev)
{
    // open device
    m_fd=::open(dev.c_str(),O_RDWR|O_NONBLOCK,0);
    if(m_fd==-1)
    {
        printf("[video] Cannot open device '%s'\n",dev.c_str());
        return;
    }
}

bool webcam_t::init()
{
    struct v4l2_capability cap;
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    struct v4l2_format fmt;
    struct v4l2_input input;
    struct v4l2_frmsizeenum frmsize;
    struct v4l2_standard std;


    // check the device is a video device, capable of read/write operations
    if (xioctl(VIDIOC_QUERYCAP,&cap)==-1)
    {
        printf("[video] Error: device is not a valid V4L2 device or the request failed\n");
        return false;
    }
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        printf("[video] Error: device has not video capture capability\n");
        return false;
    }

    printf("[video] Available video inputs:\n");
    memset(&input,0,sizeof(input));
    while(ioctl(m_fd,VIDIOC_ENUMINPUT,&input)!=-1)
    {
        printf("[video] * %s[type=%s]\n",input.name,input.type==V4L2_INPUT_TYPE_CAMERA?"camera":"tuner");
        input.index++;
    }

    printf("[video] Available frame sizes[experimental API call]:\n");
    memset(&frmsize,0,sizeof(frmsize));
    while(ioctl(m_fd,VIDIOC_ENUM_FRAMESIZES,&frmsize)!=-1)
    {
        if(frmsize.type==V4L2_FRMSIZE_TYPE_DISCRETE)
            printf("[video] * %dx%d\n",frmsize.discrete.width,frmsize.discrete.height);
        frmsize.index++;
    }

    printf("[video] Supported video standards:\n");
    memset(&std,0,sizeof(std));
    while(ioctl(m_fd,VIDIOC_ENUMSTD,&std)!=-1)
    {
        printf("[video] * %s[lines_per_frame=%d]\n",std.name,std.framelines);
        std.index++;
    }

    bool read_write_supported=(cap.capabilities & V4L2_CAP_READWRITE);
    m_read_write_supported=read_write_supported;
    bool streaming_supported=(cap.capabilities & V4L2_CAP_STREAMING);
    m_streaming_supported=streaming_supported;
    bool asyncio_supported=(cap.capabilities & V4L2_CAP_ASYNCIO);

    printf("[video] Device supports the following input mode:\n");
    printf("[video] * read/write: %s\n",read_write_supported?"yes":"no");
    printf("[video] * streaming: %s\n",streaming_supported?"yes":"no");
    printf("[video] * asynchronous io: %s\n",asyncio_supported?"yes":"no");

    if(!m_read_write_supported && !m_streaming_supported)
    {
        printf("[video] Error: device does not supported read !\n");
        return false;
    }

    // note: we use a conservative choice: not interleave YUV but it would be better to do the best possible chocie using the enumeration
    memset(&fmt,0,sizeof(fmt));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = m_width;
    fmt.fmt.pix.height      = m_height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_NONE;
    fmt.fmt.pix.colorspace  = V4L2_COLORSPACE_SMPTE170M;

    if(xioctl(VIDIOC_S_FMT,&fmt)==-1)
    {
        printf("[video] Error: input format selection failed\n");
        return false;
    }

    // if read write is not supported, setup mmap or user pointer
    printf("[video] Trying read/write method...\n");

    // try read/write method
    {
        if(read_write_supported)
        {
            m_video_buffer=new uint8_t[m_video_image_size];
            if(m_video_buffer!=0)
            {
                printf("[video] Using read/write method\n");
                m_capture_method=VIDEO_READ_WRITE;
                goto Ltry_end;
            }
            else
                printf("[video] Error: could not allocate video buffer\n");
        }
    }

    // try user pointer method
    {
        struct v4l2_requestbuffers reqbuf;
        printf("[video] Trying user pointer method...\n");

        memset(&reqbuf,0,sizeof(reqbuf));
        reqbuf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        reqbuf.memory=V4L2_MEMORY_USERPTR;
        if(ioctl(m_fd,VIDIOC_REQBUFS,&reqbuf)!=-1)
        {
            m_video_buffer=new uint8_t[m_video_image_size];
            if(m_video_buffer!=0)
            {
                printf("[video] Using user pointer method\n");
                m_capture_method=VIDEO_USERP;
                goto Ltry_end;
            }
            else
                printf("[video] Error: could not allocate video buffer\n");
        }
    }

    // try memory mapping method
    {
        struct v4l2_requestbuffers reqbuf;
        printf("[video] Trying memory map method...\n");

        memset(&reqbuf,0,sizeof(reqbuf));
        reqbuf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        reqbuf.memory=V4L2_MEMORY_MMAP;
        reqbuf.count=1;

        if(ioctl(m_fd,VIDIOC_REQBUFS,&reqbuf)!=-1)
        {
            struct v4l2_buffer buffer;

            memset(&buffer,0,sizeof (buffer));
            buffer.type=reqbuf.type;
            buffer.memory=V4L2_MEMORY_MMAP;
            buffer.index=0;
            if(ioctl(m_fd,VIDIOC_QUERYBUF,&buffer)!=-1)
            {
                m_mmap_buffer_length=buffer.length;
                m_video_buffer=static_cast<uint8_t *>(
                    mmap(NULL,buffer.length,
                        PROT_READ|PROT_WRITE,
                        MAP_SHARED,
                        m_fd,buffer.m.offset));
                if(m_video_buffer!=MAP_FAILED)
                {
                    printf("[video] Using memory mapping method\n");
                    m_capture_method=VIDEO_MMAP;
                    goto Ltry_end;
                }
                else
                    printf("[video] Error: mmap failed\n");
            }
            else
                printf("[video] Error: could not query mmap buffer\n");
        }
    }

    // no method worked
    printf("[video] Error: no useable read method found\n");
    return false;

    Ltry_end:

    // setup cropping: reset if supported
    memset(&cropcap,0,sizeof(cropcap));
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(xioctl(VIDIOC_CROPCAP,&cropcap)==0)
    {
        crop.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c=cropcap.defrect; // reset to default
        if(xioctl(VIDIOC_S_CROP,&crop)==-1)
        {
            printf("[video] Warning: cropping unsupported or cropping reseting failed, assume everything is well setuped\n");
        }
    }
    else
    {
        printf("[video] Warning: cropping unsupported, assume everything is well setuped\n");
    }

    // select video input format
    {
        printf("[video] Supported formats:\n");
        // enumerate formats
        struct v4l2_fmtdesc fmt;
        fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.index=0;
        while(xioctl(VIDIOC_ENUM_FMT,&fmt)!=-1)
        {
            printf("[video] * %s\n",fmt.description);
            fmt.index++;
            xioctl(VIDIOC_ENUM_FMT,&fmt);
        }
    }
    /*
    // note: we use a conservative choice: not interleave YUV but it would be better to do the best possible chocie using the enumeration
    memset(&fmt,0,sizeof(fmt));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = m_width;
    fmt.fmt.pix.height      = m_height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_NONE;
    fmt.fmt.pix.colorspace  = V4L2_COLORSPACE_SMPTE170M;

    if(xioctl(VIDIOC_S_FMT,&fmt)==-1)
    {
        printf("[video] Error: input format selection failed\n");
        return false;
    }
    */
    
    printf("[video] Using input format: not interlaced, YUYV(I hope !) video capturing at %ux%u[bpl=%u]\n",
        fmt.fmt.pix.width,fmt.fmt.pix.height,fmt.fmt.pix.bytesperline);
    printf("[video] Driver set pixel format to %u(colorspace %u) and field to %u\n",
        fmt.fmt.pix.pixelformat,fmt.fmt.pix.colorspace,fmt.fmt.pix.field);
    m_width=fmt.fmt.pix.width;
    m_height=fmt.fmt.pix.height;
    m_video_bytes_per_line=fmt.fmt.pix.bytesperline;
    m_video_pixel_format=fmt.fmt.pix.pixelformat;
    m_video_colorspace=fmt.fmt.pix.colorspace;

    return true;
}

bool webcam_t::enqueue_buffer()
{
    struct v4l2_buffer buf;
    memset(&buf,0,sizeof(buf));

    buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory=V4L2_MEMORY_MMAP;
    buf.index=0;

    if(xioctl(VIDIOC_QBUF,&buf)==-1)
    {
        printf("[video] Error: could not enqueue video buffer\n");
        return false;
    }
    return true;
}

bool webcam_t::dequeue_buffer()
{
    struct v4l2_buffer buf;
    memset(&buf,0,sizeof(buf));

    buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory=V4L2_MEMORY_MMAP;

    int res;
    do
    {
        usleep(50);
        res=xioctl(VIDIOC_DQBUF,&buf);
    }
    while(res==-1 && errno==EAGAIN);

    if(res==-1)
    {
        printf("[video] Error: buffer dequeueing failed\n");
        return false;
    }
    else
        return true;
}

bool webcam_t::start_capturing()
{
    switch(m_capture_method)
    {
        case VIDEO_READ_WRITE:
            return true;
        case VIDEO_MMAP:
            {
                if(!enqueue_buffer())
                    return false;

                enum v4l2_buf_type type;
                type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if(xioctl(VIDIOC_STREAMON,&type)==-1)
                {
                    printf("[video] Error: could not start streaming\n");
                    return false;
                }

                if(!dequeue_buffer())
                    return false;

                return true;
            }
        case VIDEO_USERP:
            printf("[video] Error: start streaming for user pointer method unimplemented\n");
            return false;
        default:
            printf("[video] Error: no capture used[internal error]{stac}\n");
            return false;
    }
}

void webcam_t::stop_capturing()
{
    switch(m_capture_method)
    {
        case VIDEO_READ_WRITE:
            break;
        case VIDEO_MMAP:
        case VIDEO_USERP:
            {
                enum v4l2_buf_type type;
                type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
                if(xioctl(VIDIOC_STREAMOFF,&type)==-1)
                    printf("[video] Error: could not stop streaming\n");
            }
            break;
        default:
            printf("[video] Error: no capture used[internal error]{stoc}\n");
            break;
    }
}

bool webcam_t::video_get_integer_control(__u32 cid,int& v)
{
    struct v4l2_control control;
    memset(&control,0,sizeof(control));
    control.id=cid;

    if(ioctl(m_fd,VIDIOC_G_CTRL,&control)==-1)
        return false;
    else
    {
        v=control.value;
        return true;
    }
}

bool webcam_t::video_set_boolean_control(__u32 cid,bool b)
{
    struct v4l2_control control;
    memset (&control,0,sizeof(control));
    control.id=cid;
    control.value=b;

    if(ioctl(m_fd,VIDIOC_S_CTRL,&control)==-1)
        return false;
    else
        return true;
}

bool webcam_t::video_set_integer_control(__u32 cid,int v)
{
    struct v4l2_control control;
    memset (&control,0,sizeof(control));
    control.id=cid;
    control.value=v;

    if(ioctl(m_fd,VIDIOC_S_CTRL,&control)==-1)
        return false;
    else
        return true;
}

bool webcam_t::video_capture()
{
    int brightness,contrast,saturation,hue,gamma;
    if(!video_get_integer_control(V4L2_CID_BRIGHTNESS,brightness))
        printf("[video] Warning: could not get brighness\n");
    if(!video_get_integer_control(V4L2_CID_CONTRAST,contrast))
        printf("[video] Warning: could not get contrast\n");
    if(!video_get_integer_control(V4L2_CID_SATURATION,saturation))
        printf("[video] Warning: could not get saturation\n");
    if(!video_get_integer_control(V4L2_CID_HUE,hue))
        printf("[video] Warning: could not get hue\n");
    if(!video_get_integer_control(V4L2_CID_GAMMA,gamma))
        printf("[video] Warning: could not get gamma adjust\n");
    printf("[video] (capture) brightness=%d contrast=%d saturation=%d hue=%d gamma=%d\n",brightness,contrast,saturation,hue,gamma);

    switch(m_capture_method)
    {
        case VIDEO_READ_WRITE:
            printf("[video] Error: video capturing using read/write method unimplemented\n");
            return false;
        case VIDEO_USERP:
            printf("[video] Error: video capturing using user pointer method unimplemented\n");
            return false;
        case VIDEO_MMAP:
            {
                if(!enqueue_buffer())
                    return false;
                return dequeue_buffer();
            }
        default:
            printf("[video] Error: no capture used[internal error]{vc}\n");
            return false;
    }
}

void webcam_t::deinit()
{
    switch(m_capture_method)
    {
        case VIDEO_READ_WRITE:
        case VIDEO_USERP:
            delete[] m_video_buffer;
            break;
        case VIDEO_MMAP:
            munmap(m_video_buffer,m_mmap_buffer_length);
            break;
        default:
            printf("[video] Error: no capture used[internal error]{uvd}\n");
    }
}

bool webcam_t::process_capture_to_image(image_t& img)
{
    assert(img.fmt==image_t::yuv_format);
    // mainly expand yuyv to yuv
    // also take care of padding
    if(m_video_pixel_format==V4L2_PIX_FMT_YUYV)
    {
        for(size_t y=0;y<m_height;y++)
        {
            // four bytes -> two pixels
            for(size_t x=0;x<m_width;x+=2)
            {
                // equivalent of 2 bytes per pixel
                uint8_t *ptr=m_video_buffer+(m_height-1-y)*m_video_bytes_per_line+x*2;

                uint8_t Y1=ptr[0];
                uint8_t U=ptr[1];
                uint8_t V=ptr[3];
                uint8_t Y2=ptr[2];

                img.pixel(x,y)[0]=Y1;
                img.pixel(x,y)[1]=U;
                img.pixel(x,y)[2]=V;
                img.pixel(x+1,y)[0]=Y2;
                img.pixel(x+1,y)[1]=U;
                img.pixel(x+1,y)[2]=V;
            }
        }
        return true;
    }
    else
    {
        printf("[video] Error: video pixel format support unimplemented\n");
        return false;
    }
}

