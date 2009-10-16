#include <pthread.h>

#include "picAPI.h"
#include "webcamAPI.h"

void init_webcam_api()
{
}

//------------------------------------------------------------------------------
unsigned char* get_webcam_picture(int webcam_id)
{
  unsigned char * data;
  pthread_mutex_t *mutex = picWebcam(webcam_id, &data);
  pthread_mutex_lock(mutex);

  return data;
}
//------------------------------------------------------------------------------
int wc_reco_config()
{
  return 100;
}
//------------------------------------------------------------------------------

