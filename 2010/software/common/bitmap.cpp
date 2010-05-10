#include <stdint.h>
#include <stdio.h>

#include "bitmap.h"

void save_buff_to_bitmap(const char *file, unsigned int w, unsigned int h, uint16_t *data)
{
  FILE *F= fopen(file,"w+");
  int sizeline = 3 * w;
  sizeline += sizeline%4;
  int n;
  fwrite("BM",2,1,F);
  n = 14 + 40 + sizeline*h; fwrite(&n,4,1,F);              
  n = 0;   fwrite(&n,4,1,F);              
  n = 14;  fwrite(&n,4,1,F);
                
  n = 40;  fwrite(&n,4,1,F);        
  n = w;  fwrite(&n,4,1,F);        
  n = h; fwrite(&n,4,1,F);   
  n = 1;   fwrite(&n,2,1,F);
  n = 24;  fwrite(&n,2,1,F);                  
  n = 0;   fwrite(&n,4,1,F);   
  n = h; fwrite(&n,4,1,F);
  n = 1000; fwrite(&n,4,1,F);
  n = 1000; fwrite(&n,4,1,F);
  n = 0;   fwrite(&n,4,1,F);  
  n = 0;   fwrite(&n,4,1,F);  
  
  for(unsigned int y=0;y<h /* Non signé: MAX_INT == -1*/;y++)
  {
    for(unsigned int x=0;x<w;x++)
    {
      int ipix = 3*(y*w+x);
      fwrite(&data[ipix+2],1,1,F);   // 2
      fwrite(&data[ipix+1],1,1,F);   // 1
      fwrite(&data[ipix+0],1,1,F);   // 0
    }
    unsigned int x = 3 * w;
    n = 0;
    while(x%4)
    {
      fwrite(&n,1,1,F);
      x++;
    }
  }              
  fclose(F); 
}

