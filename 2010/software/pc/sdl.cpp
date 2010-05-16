#include <unistd.h>

#define PC_INCLUDE
#include "../common/const.h"
#include "cinematik.h"
#include "path_tracker.h"
#include "anticol.h"
#include "sdl.h"

bool sdlWindow::init_done = false;

//------------------------------------------------------------------------------
sdlWindow::sdlWindow(int width, int height, const char *title)
{
  pthread_mutex_init(&using_background, NULL); 

  if(!init_done)
  {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      fprintf(stderr, "Error during SDL initialization : %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
    }
    init_done = true;
  }
 
  back_pixels = new Uint32[4*width*height];
  
  affichage = SDL_SetVideoMode(width,height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF/*|SDL_FULLSCREEN*/);
  background = NULL;

  if (affichage == NULL || back_pixels == NULL) {
    fprintf(stderr, "Cannot activate the graphic mode : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }  
  printf("ok\nSetting title...                ");
  fflush(stdout); 
  
  SDL_WM_SetCaption(title, NULL);
}
//------------------------------------------------------------------------------
Uint32 sdlWindow::makeColorSDL(char *color)
{
  int r=0,g=0,b=0;
  if('0'<=color[0] && color[0]<='9') r+=16*(color[0]-'0');
  if('A'<=color[0] && color[0]<='F') r+=16*(color[0]-'A'+10);  
  if('a'<=color[0] && color[0]<='f') r+=16*(color[0]-'a'+10);  
  if('0'<=color[1] && color[1]<='9') r+=(color[1]-'0');
  if('A'<=color[1] && color[1]<='F') r+=(color[1]-'A'+10);  
  if('a'<=color[1] && color[1]<='f') r+=(color[1]-'a'+10);  

  if('0'<=color[2] && color[2]<='9') g+=16*(color[2]-'0');
  if('A'<=color[2] && color[2]<='F') g+=16*(color[2]-'A'+10);  
  if('a'<=color[2] && color[2]<='f') g+=16*(color[2]-'a'+10);  
  if('0'<=color[3] && color[3]<='9') g+=(color[3]-'0');
  if('A'<=color[3] && color[3]<='F') g+=(color[3]-'A'+10);  
  if('a'<=color[3] && color[3]<='f') g+=(color[3]-'a'+10);  
  
  if('0'<=color[4] && color[4]<='9') b+=16*(color[4]-'0');
  if('A'<=color[4] && color[4]<='F') b+=16*(color[4]-'A'+10);  
  if('a'<=color[4] && color[4]<='f') b+=16*(color[4]-'a'+10);  
  if('0'<=color[5] && color[5]<='9') b+=(color[5]-'0');
  if('A'<=color[5] && color[5]<='F') b+=(color[5]-'A'+10);  
  if('a'<=color[5] && color[5]<='f') b+=(color[5]-'a'+10);            
  return makeColorSDL(r,g,b);
}
//------------------------------------------------------------------------------
void sdlWindow::RefreshSDL(void)
{
  SDL_Flip(affichage);
//  SDL_PumpEvents(); 
}
//------------------------------------------------------------------------------
void sdlWindow::LigneHorizontaleSDL(int x, int y, int w, Uint32 coul)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = w;
  r.h = 1;

  SDL_FillRect(affichage, &r, coul);
}
//------------------------------------------------------------------------------
void sdlWindow::LigneVerticaleSDL(int x, int y, int h, Uint32 coul)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = 1;
  r.h = h;

  SDL_FillRect(affichage, &r, coul);
}
//------------------------------------------------------------------------------
void sdlWindow::LigneSDL(int x1, int y1, int x2, int y2, Uint32 coul)
{
  int d, dx, dy, aincr, bincr, xincr, yincr, x, y;
  
  if (abs(x2 - x1) < abs(y2 - y1))
  {
    /* parcours par l'axe vertical */

    if (y1 > y2)
    {
      swapInts(&x1, &x2);
      swapInts(&y1, &y2);
    }

    xincr = x2 > x1 ? 1 : -1;
    dy = y2 - y1;
    dx = abs(x2 - x1);
    d = 2 * dx - dy;
    aincr = 2 * (dx - dy);
    bincr = 2 * dx;
    x = x1;
    y = y1;
    
    setPixelVerif(x, y, coul);

    for (y = y1+1; y <= y2; ++y)
    {
      if (d >= 0)
      {
	      x += xincr;
	      d += aincr;
      }
      else
	      d += bincr;

      setPixelVerif(x, y, coul);
    }
  }
  else 
  {
    /* parcours par l'axe horizontal */
    if (x1 > x2)
    {
      swapInts(&x1, &x2);
      swapInts(&y1, &y2);
    }

    yincr = y2 > y1 ? 1 : -1;
    dx = x2 - x1;
    dy = abs(y2 - y1);
    d = 2 * dy - dx;
    aincr = 2 * (dy - dx);
    bincr = 2 * dy;
    x = x1;
    y = y1;

    setPixelVerif(x, y, coul);

    for (x = x1+1; x <= x2; ++x)
    {
      if (d >= 0)
      {
       	y += yincr;
       	d += aincr;
      }
      else
      	d += bincr;

      setPixelVerif(x, y, coul);
    }
  }    
}
//------------------------------------------------------------------------------
void sdlWindow::PolylineSDL(point_t *Points, int nbr, Uint32 coul)
{
  int i;
  for(i=1;i<nbr;i++)
    LigneSDL(Points[i-1].x, Points[i-1].y, Points[i].x, Points[i].y, coul);
  LigneSDL(Points[nbr-1].x, Points[nbr-1].y, Points[0].x, Points[0].y, coul);
}
//------------------------------------------------------------------------------
void sdlWindow::FillRectSDL(int x, int y, int w, int h, Uint32 coul)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;

  SDL_FillRect(affichage, &r, coul);
}
//------------------------------------------------------------------------------
void sdlWindow::CercleSDL(int cx, int cy, int rayon, Uint32 coul)
{
  int d, y, x;

  d = 3 - (2 * rayon);
  x = 0;
  y = rayon;

  while (y >= x) {
    setPixelVerif(cx + x, cy + y, coul);
    setPixelVerif(cx + y, cy + x, coul);
    setPixelVerif(cx - x, cy + y, coul);
    setPixelVerif(cx - y, cy + x, coul);
    setPixelVerif(cx + x, cy - y, coul);
    setPixelVerif(cx + y, cy - x, coul);
    setPixelVerif(cx - x, cy - y, coul);
    setPixelVerif(cx - y, cy - x, coul);

    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }

    x++;
  }
}
//------------------------------------------------------------------------------
void sdlWindow::DisqueSDL(int cx, int cy, int rayon, Uint32 coul)
{
  int d, y, x;

  d = 3 - (2 * rayon);
  x = 0;
  y = rayon;

  while (y >= x) {
    LigneHorizontaleSDL(cx - x, cy - y, 2 * x + 1, coul);
    LigneHorizontaleSDL(cx - x, cy + y, 2 * x + 1, coul);
    LigneHorizontaleSDL(cx - y, cy - x, 2 * y + 1, coul);
    LigneHorizontaleSDL(cx - y, cy + x, 2 * y + 1, coul);

    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }

    x++;
  }
}
//------------------------------------------------------------------------------
void sdlWindow::Load_SDL_Background(bool from_back_pixel)
{
  pthread_mutex_lock(&using_background);
  if(!from_back_pixel)
    memcpy(back_pixels,affichage->pixels,4*affichage->w*affichage->h);
  if(background) SDL_FreeSurface(background);
  background=SDL_CreateRGBSurfaceFrom(back_pixels,affichage->w,affichage->h,32,4*affichage->w,0,0,0,0);
  if(!background) {
    fprintf(stderr, "Cannot create background : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }  
  pthread_mutex_unlock(&using_background);  
}
//------------------------------------------------------------------------------
void sdlWindow::Draw_SDL_Background()
{
  pthread_mutex_lock(&using_background);
  SDL_BlitSurface(background,NULL,affichage,NULL);
  pthread_mutex_unlock(&using_background);  
}
//------------------------------------------------------------------------------
