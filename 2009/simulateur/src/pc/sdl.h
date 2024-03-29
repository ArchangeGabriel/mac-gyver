#ifndef SDLH
#define SDLH
#include <SDL.h>

typedef struct point_t
{
  int x;
  int y;
} point_t;

// Couleurs
enum {clBlack, clDarkGray, clGray, clLightGray, clWhite, clGround, clRed, clGreen, clDarkRed, clDarkGreen, clDarkBrown, clBrown, NBR_COLORS};

void initSDL();
bool is_SDL_ready();
void Load_SDL_Colors();
void Load_SDL_Background();
void Free_SDL_Background();
void Draw_SDL_Background();
void SDL_First_Background();

Uint32 makeColorSDL(char *color);
Uint32 makeColorSDL(int R, int G, int B);
Uint32 getColorSDL(int color_id);
void setPixelVerif(int x, int y, Uint32 coul);
void LigneSDL(int x1, int y1, int x2, int y2, Uint32 coul);
void PolylineSDL(point_t *Points, int nbr, Uint32 coul);
void RefreshSDL(void);
void LigneHorizontaleSDL(int x, int y, int w, Uint32 coul);
void LigneVerticaleSDL(int x, int y, int w, Uint32 coul);
void FillRectSDL(int x, int y, int w, int h, Uint32 coul);
void CercleSDL(int cx, int cy, int rayon, Uint32 coul);
void DisqueSDL(int cx, int cy, int rayon, Uint32 coul);
void SDL_Draw_Robot();
#endif
