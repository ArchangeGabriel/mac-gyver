#include <stdint.h>
#include <stdio.h>

#include "bitmap.h"

void save_bitmap_header(FILE *F, int width, int height)
{
  int sizeline = 3 * width;
  sizeline += sizeline%4;
  int n;
  fwrite("BM",2,1,F);
  n = 14 + 40 + sizeline*height; fwrite(&n,4,1,F);              
  n = 0;   fwrite(&n,4,1,F);              
  n = 14;  fwrite(&n,4,1,F);
                
  n = 40;  fwrite(&n,4,1,F);        
  n = width;  fwrite(&n,4,1,F);        
  n = height; fwrite(&n,4,1,F);   
  n = 1;   fwrite(&n,2,1,F);
  n = 24;  fwrite(&n,2,1,F);                  
  n = 0;   fwrite(&n,4,1,F);   
  n = height; fwrite(&n,4,1,F);
  n = 1000; fwrite(&n,4,1,F);
  n = 1000; fwrite(&n,4,1,F);
  n = 0;   fwrite(&n,4,1,F);  
  n = 0;   fwrite(&n,4,1,F); 
}

void save_buff_to_bitmap(const char *file, unsigned int w, unsigned int h, uint16_t *data)
{
  FILE *F= fopen(file,"w+");
  save_bitmap_header(F, w, h);
  
  for(unsigned int y=0;y<h;y++)
  {
    for(unsigned int x=0;x<w;x++)
    {
      int ipix = 3*(y*w+x);
      fwrite(&data[ipix+2],1,1,F);   // 2
      fwrite(&data[ipix+1],1,1,F);   // 1
      fwrite(&data[ipix+0],1,1,F);   // 0
    }
    unsigned int x = 3 * w;
    int n = 0;
    while(x%4)
    {
      fwrite(&n,1,1,F);
      x++;
    }
  }              
  fclose(F); 
}

