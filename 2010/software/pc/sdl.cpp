#include <unistd.h>

#define PC_INCLUDE
#include "../common/const.h"
#include "cinematik.h"
#include "path_planning.h"
#include "anticol.h"
#include "sdl.h"

#define _SCALE_SDL 200

SDL_Surface* affichage=NULL;
SDL_Surface* background=NULL;
Uint32 *back_pixels;

//------------------------------------------------------------------------------
void initSDL()
{
  printf("SDL initialization...           ");
  fflush(stdout);  
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Error during SDL initialization : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  int Width = _LONGUEUR_TER * _SCALE_SDL;
  int Height = _LARGEUR_TER * _SCALE_SDL;  
  affichage = SDL_SetVideoMode(Width,Height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF/*|SDL_FULLSCREEN*/);
  back_pixels = new Uint32[4*affichage->w*affichage->h];
  if (affichage == NULL || back_pixels == NULL) {
    fprintf(stderr, "Cannot activate the graphic mode : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  printf("ok\nSetting title...                ");
  fflush(stdout); 
  SDL_WM_SetCaption("Temples of Atlantis", NULL);
  printf("ok\n");
  fflush(stdout);  
  Load_SDL_Colors(); 
}
//------------------------------------------------------------------------------
void Load_SDL_Colors()
{  
 
}
//------------------------------------------------------------------------------
bool is_SDL_ready()
{
  return affichage!=NULL;  
}
//------------------------------------------------------------------------------
void Load_SDL_Background()
{
  memcpy(back_pixels,affichage->pixels,4*affichage->w*affichage->h);
  background=SDL_CreateRGBSurfaceFrom(back_pixels,affichage->w,affichage->h,32,4*affichage->w,0,0,0,0);
  if (background == NULL) {
    fprintf(stderr, "Cannot create background : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }  
}
//------------------------------------------------------------------------------
void Free_SDL_Background()
{
  SDL_FreeSurface(background);
}
//------------------------------------------------------------------------------
void Draw_SDL_Background()
{
  SDL_BlitSurface(background,NULL,affichage,NULL);
}
//------------------------------------------------------------------------------
inline Uint32 makeColorSDL(int R, int G, int B)
{
  return SDL_MapRGB(affichage->format, R, G, B);
}
//------------------------------------------------------------------------------
Uint32 makeColorSDL(char *color)
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
inline void setPixel(int x, int y, Uint32 coul)
{
  *((Uint32*)(affichage->pixels) + x + y * affichage->w) = coul;
}
//------------------------------------------------------------------------------
inline void setPixelVerif(int x, int y, Uint32 coul)
{
  if (x >= 0 && x < affichage->w &&
      y >= 0 && y < affichage->h)
    setPixel(x, y, coul);
}
//------------------------------------------------------------------------------
inline void echangerEntiers(int* x, int* y)
{
  int t = *x;
  *x = *y;
  *y = t;
}
//------------------------------------------------------------------------------
void RefreshSDL(void)
{
  SDL_Flip(affichage);
//  SDL_PumpEvents(); 
}
//------------------------------------------------------------------------------
void LigneHorizontaleSDL(int x, int y, int w, Uint32 coul)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = w;
  r.h = 1;

  SDL_FillRect(affichage, &r, coul);
}
//------------------------------------------------------------------------------
void LigneVerticaleSDL(int x, int y, int h, Uint32 coul)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = 1;
  r.h = h;

  SDL_FillRect(affichage, &r, coul);
}
//------------------------------------------------------------------------------
void LigneSDL(int x1, int y1, int x2, int y2, Uint32 coul)
{
  int d, dx, dy, aincr, bincr, xincr, yincr, x, y;
  
  if (abs(x2 - x1) < abs(y2 - y1))
  {
    /* parcours par l'axe vertical */

    if (y1 > y2)
    {
      echangerEntiers(&x1, &x2);
      echangerEntiers(&y1, &y2);
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
      echangerEntiers(&x1, &x2);
      echangerEntiers(&y1, &y2);
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
void PolylineSDL(point_t *Points, int nbr, Uint32 coul)
{
  int i;
  for(i=1;i<nbr;i++)
    LigneSDL(Points[i-1].x, Points[i-1].y, Points[i].x, Points[i].y, coul);
  LigneSDL(Points[nbr-1].x, Points[nbr-1].y, Points[0].x, Points[0].y, coul);
}
//------------------------------------------------------------------------------
void FillRectSDL(int x, int y, int w, int h, Uint32 coul)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;

  SDL_FillRect(affichage, &r, coul);
}
//------------------------------------------------------------------------------
void CercleSDL(int cx, int cy, int rayon, Uint32 coul)
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
void DisqueSDL(int cx, int cy, int rayon, Uint32 coul)
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
void SDL_First_Background()
{
  printf("Creating background...          ");
  fflush(stdout);
  // sol
  FillRectSDL(0,0,int(_SCALE_SDL*_LONGUEUR_TER),int(_SCALE_SDL*_LARGEUR_TER),clGround);  
  // départ 1
  FillRectSDL(0,0,int(_SCALE_SDL*0.5),int(_SCALE_SDL*0.5),clTeam1);
  FillRectSDL(int(_SCALE_SDL*(_LONGUEUR_TER-0.5)),int(_SCALE_SDL*(_LARGEUR_TER-0.522)),int(_SCALE_SDL*0.5),int(_SCALE_SDL*0.522),clDeposeTeam1);  
  // départ 2
  FillRectSDL(int(_SCALE_SDL*(_LONGUEUR_TER-0.5)),0,int(_SCALE_SDL*0.5),int(_SCALE_SDL*0.5),clTeam2);
  FillRectSDL(0.,int(_SCALE_SDL*(_LARGEUR_TER-0.522)),int(_SCALE_SDL*0.5),int(_SCALE_SDL*0.522),clDeposeTeam2);  
  // Hors Terrain
  FillRectSDL(int(_SCALE_SDL*0.5),int(_SCALE_SDL*2.122),int(_SCALE_SDL*2.),int(_SCALE_SDL*0.5),clBlack);
  // Mur Bas
  FillRectSDL(int(_SCALE_SDL*0.5),int(_SCALE_SDL*2.1),int(_SCALE_SDL*2.),int(_SCALE_SDL*0.022),clWall);    
  FillRectSDL(int(_SCALE_SDL*0.5),int(_SCALE_SDL*2.122),int(_SCALE_SDL*0.022),int(_SCALE_SDL*0.5),clWall);         
  FillRectSDL(int(_SCALE_SDL*(_LONGUEUR_TER-0.522)),int(_SCALE_SDL*2.122),int(_SCALE_SDL*0.022),int(_SCALE_SDL*0.5),clWall);                         
  // Colline
  FillRectSDL(int(_SCALE_SDL*0.67),0,int(_SCALE_SDL*1.66),int(_SCALE_SDL*0.5),clHill);
  // Mur colline
  FillRectSDL(int(_SCALE_SDL*0.74),int(_SCALE_SDL*0.5),int(_SCALE_SDL*1.52),int(_SCALE_SDL*0.022),clWall); 
  LigneVerticaleSDL(int(_SCALE_SDL*0.74), 0, int(_SCALE_SDL*0.5), clBlack);               
  LigneVerticaleSDL(int(_SCALE_SDL*(_LONGUEUR_TER-0.74)), 0, int(_SCALE_SDL*0.5), clBlack);                 
  LigneVerticaleSDL(int(_SCALE_SDL*1.24), 0, int(_SCALE_SDL*0.5), clBlack);                                    
  LigneVerticaleSDL(int(_SCALE_SDL*(_LONGUEUR_TER-1.24)), 0, int(_SCALE_SDL*0.5), clBlack); 
  
  printf("ok\n");
  fflush(stdout);                 
}
//------------------------------------------------------------------------------
void SDL_Draw_Robot()
{
  vector_t N,T,u,v;
  point_t point[4];

  position_t pos,dest;
  
  while(true)
  {
    Draw_SDL_Background();  
    // Draw
    pos = cine_get_position();
    dest = pp_get_dest();

    // Robot supposé
    N.x=cos(pos.a);
    N.y=sin(pos.a);
    T.x=-sin(pos.a);
    T.y=cos(pos.a);
    v=vector_t(_ROUE_X,0.).rotate(pos.a);
    point[0].x=((int)((pos.x+N.x*_LONGUEUR_ROBOT/2.+T.x*_LARGEUR_ROBOT/2.-v.x)*_SCALE_SDL));     
    point[0].y=((int)((pos.y+N.y*_LONGUEUR_ROBOT/2.+T.y*_LARGEUR_ROBOT/2.-v.y)*_SCALE_SDL));
    point[1].x=((int)((pos.x+N.x*_LONGUEUR_ROBOT/2.-T.x*_LARGEUR_ROBOT/2.-v.x)*_SCALE_SDL));
    point[1].y=((int)((pos.y+N.y*_LONGUEUR_ROBOT/2.-T.y*_LARGEUR_ROBOT/2.-v.y)*_SCALE_SDL));
    point[2].x=((int)((pos.x-N.x*_LONGUEUR_ROBOT/2.-T.x*_LARGEUR_ROBOT/2.-v.x)*_SCALE_SDL));
    point[2].y=((int)((pos.y-N.y*_LONGUEUR_ROBOT/2.-T.y*_LARGEUR_ROBOT/2.-v.y)*_SCALE_SDL));
    point[3].x=((int)((pos.x-N.x*_LONGUEUR_ROBOT/2.+T.x*_LARGEUR_ROBOT/2.-v.x)*_SCALE_SDL));
    point[3].y=((int)((pos.y-N.y*_LONGUEUR_ROBOT/2.+T.y*_LARGEUR_ROBOT/2.-v.y)*_SCALE_SDL)); 
    PolylineSDL(point, 4, clRobot);
    LigneSDL(point[2].x, point[2].y, ((int)(pos.x*_SCALE_SDL)), ((int)(pos.y*_SCALE_SDL)), clRobot); 
    LigneSDL(point[3].x, point[3].y, ((int)(pos.x*_SCALE_SDL)), ((int)(pos.y*_SCALE_SDL)), clRobot);

    // Destination
    N.x=cos(dest.a);
    N.y=sin(dest.a);
    T.x=-sin(dest.a)*0.03;
    T.y=cos(dest.a)*0.03; 
    LigneSDL(((int)(_SCALE_SDL*dest.x)), ((int)(_SCALE_SDL*dest.y)), 
             ((int)(_SCALE_SDL*(dest.x+N.x*0.1))), ((int)(_SCALE_SDL*(dest.y+N.y*0.1))), clBlack);
    LigneSDL(((int)(_SCALE_SDL*(dest.x+N.x*0.1))), ((int)(_SCALE_SDL*(dest.y+N.y*0.1))), 
             ((int)(_SCALE_SDL*(dest.x+N.x*0.07+T.x))), ((int)(_SCALE_SDL*(dest.y+N.y*0.07+T.y))), clBlack);
    LigneSDL(((int)(_SCALE_SDL*(dest.x+N.x*0.1))), ((int)(_SCALE_SDL*(dest.y+N.y*0.1))), 
             ((int)(_SCALE_SDL*(dest.x+N.x*0.07-T.x))), ((int)(_SCALE_SDL*(dest.y+N.y*0.07-T.y))), clBlack);
    
    // Captors
    for(int i=0;i<4;i++)
    {
      vector_t pos = captor_get_position(i);
      Uint32 color;
      switch(captor_get_status(i))
      {
        case 0: color = clBlack; break;
        case 1: color = SDL_MapRGB(affichage->format, 255, 0, 180);  break;
        case 2: color = SDL_MapRGB(affichage->format, 255, 0, 0);  break;
        case 3: color = SDL_MapRGB(affichage->format, 60, 60, 60);  break;
      }
      DisqueSDL(((int)(_SCALE_SDL*pos.x)),
                ((int)(_SCALE_SDL*pos.y)),
                ((int)(_SCALE_SDL*0.01)),color);        
    }  
    RefreshSDL();             
    usleep(40000);
  }
}

