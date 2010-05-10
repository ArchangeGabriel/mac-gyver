#ifndef SDLH
#define SDLH
#include <SDL.h>
#include <pthread.h>

#define _SCALE_SDL 200

#define COLOR_SDL(R,G,B) (256*R + G)*256 + B


#define clWhite COLOR_SDL(255,255,255)
#define clBlack COLOR_SDL(0,0,0)

// Playground
#define clTeam1        COLOR_SDL(0,67,137)
#define clTeam2        COLOR_SDL(242,191,0)
#define clDeposeTeam1  COLOR_SDL(37,107,85)
#define clDeposeTeam2  COLOR_SDL(125,154,33)
#define clGround       COLOR_SDL(74,165,66)
#define clWall         COLOR_SDL(55,54,59)
#define clHill         COLOR_SDL(197,200,207)

// Eléments de jeu
#define clTomate       COLOR_SDL(249,0,0)
#define clOrange       COLOR_SDL(245,143,17)
#define clMais         COLOR_SDL(229,223,204)
#define clFake         COLOR_SDL(51,54,52)


// Robot
#define clRobot COLOR_SDL(255,255,255)

class sdlWindow
{
  static bool init_done;
  SDL_Surface* affichage;
  SDL_Surface* background;
  Uint32 *back_pixels;

  pthread_mutex_t using_background;  
  
  inline void setPixel(int x, int y, Uint32 coul)
  { 
    *((Uint32*)(affichage->pixels) + x + y * affichage->w) = coul; 
  }

  inline void setPixelVerif(int x, int y, Uint32 coul) 
  {
    if (x >= 0 && x < affichage->w &&
        y >= 0 && y < affichage->h)
      setPixel(x, y, coul);
  }

  inline void swapInts(int* x, int* y)
  {
    int t = *x;
    *x = *y;
    *y = t;
  }  
  
 
  public:
  sdlWindow(int width, int height, const char *title);
  void RefreshSDL(void);
  
  inline Uint32 makeColorSDL(char *color);
  inline Uint32 makeColorSDL(int R, int G, int B) { return SDL_MapRGB(affichage->format, R, G, B); }  

  void LigneSDL(int x1, int y1, int x2, int y2, Uint32 coul);
  void LigneHorizontaleSDL(int x, int y, int w, Uint32 coul);
  void LigneVerticaleSDL(int x, int y, int w, Uint32 coul);
  void PolylineSDL(point_t *Points, int nbr, Uint32 coul);
  void FillRectSDL(int x, int y, int w, int h, Uint32 coul);
  void CercleSDL(int cx, int cy, int rayon, Uint32 coul);
  void DisqueSDL(int cx, int cy, int rayon, Uint32 coul);
  
  
  void Load_SDL_Background();
  void Draw_SDL_Background();
};
#endif
