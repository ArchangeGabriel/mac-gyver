#ifndef SDLH
#define SDLH
#include <SDL.h>

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

typedef struct point_t
{
  int x;
  int y;
} point_t;

void initSDL();
bool is_SDL_ready();
void Load_SDL_Colors();
void Load_SDL_Background();
void Free_SDL_Background();
void Draw_SDL_Background();

Uint32 makeColorSDL(char *color);
Uint32 makeColorSDL(int R, int G, int B);
void setPixelVerif(int x, int y, Uint32 coul);
void LigneSDL(int x1, int y1, int x2, int y2, Uint32 coul);
void PolylineSDL(point_t *Points, int nbr, Uint32 coul);
void RefreshSDL(void);
void LigneHorizontaleSDL(int x, int y, int w, Uint32 coul);
void LigneVerticaleSDL(int x, int y, int w, Uint32 coul);
void FillRectSDL(int x, int y, int w, int h, Uint32 coul);
void CercleSDL(int cx, int cy, int rayon, Uint32 coul);
void DisqueSDL(int cx, int cy, int rayon, Uint32 coul);
#endif
