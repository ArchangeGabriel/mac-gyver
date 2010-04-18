#include <pthread.h>

// Defined in pic_interface.cpp
pthread_mutex_t* picWebcam(int id, int *W, int *H, uint16_t *data = NULL);

//------------------------------------------------------------------------------

webcam_t::webcam_t(const std::string& dev,size_t,size_t)
    :m_image(image_t::yuv_format,0,0)
{
  m_fd = atoi(dev.c_str());
  int w,h; 
  pthread_mutex_t *mutex = picWebcam(m_fd, &w, &h);
  
  if(mutex == NULL) return;
    
  pthread_mutex_lock(mutex);    
  pthread_mutex_unlock(mutex);      
  pthread_mutex_destroy(mutex);      
  delete mutex;
 
  m_width = w;
  m_height = h;
}

webcam_t::~webcam_t()
{
}
//------------------------------------------------------------------------------
void webcam_t::start()
{
    m_image=image_t(image_t::yuv_format,m_width,m_height);
}
//------------------------------------------------------------------------------
void webcam_t::stop()
{
}
//------------------------------------------------------------------------------
size_t webcam_t::get_width()
{
    return m_width;
}
//------------------------------------------------------------------------------
size_t webcam_t::get_height()
{
    return m_height;
}
//------------------------------------------------------------------------------
void webcam_t::capture(image_t& img)
{
    do_capture();
    get_image(img);
}
//------------------------------------------------------------------------------
void webcam_t::do_capture()
{
  int w,h; 
  pthread_mutex_t *mutex = picWebcam(m_fd, &w, &h, m_image.data);
  
  if(mutex == NULL) return;
    
  pthread_mutex_lock(mutex);    
  pthread_mutex_unlock(mutex);      
  pthread_mutex_destroy(mutex);      
  delete mutex;  
}
//------------------------------------------------------------------------------
void webcam_t::get_image(image_t& img)
{
    img=m_image;
}
//------------------------------------------------------------------------------
const image_t& webcam_t::get_image2() const
{
    return m_image;
}
//------------------------------------------------------------------------------
