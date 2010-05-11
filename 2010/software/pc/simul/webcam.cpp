#include "../common.h"

// Defined in pic_interface.cpp
pthread_mutex_t* picWebcam(int id, unsigned int *W, unsigned int *H, uint16_t *data = NULL);

//------------------------------------------------------------------------------

webcam_t::webcam_t(const char *name,const std::string& dev, size_t w, size_t h)
    :m_name(name),m_image(image_t::yuv_format,w,h)
{
  m_fd = atoi(dev.c_str());
  m_width = w;
  m_height = h;
  pthread_mutex_init(&mutex, NULL);  
}
//------------------------------------------------------------------------------
webcam_t::~webcam_t()
{
  pthread_mutex_lock(&mutex);    
  pthread_mutex_unlock(&mutex);      
  pthread_mutex_destroy(&mutex);  
}
//------------------------------------------------------------------------------
void webcam_t::start()
{
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
const char* webcam_t::get_name()
{
  return m_name;
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
  unsigned int w,h; 
  pthread_mutex_lock(&mutex);  

  pthread_mutex_t *wait = picWebcam(m_fd, &w, &h, m_image.data);
     
  if(wait == NULL) 
  {
  	printf("CAPTURE FAILED !!!!\n");
    return;
  }
  wait_for_it(wait);    
  	
  if(w != m_width || h != m_height)
    printf("Webcam: dimension mismatch!\n");  
  pthread_mutex_unlock(&mutex);
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
