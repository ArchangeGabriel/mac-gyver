#include <unistd.h>

#define PC_INCLUDE
#include "../common/const.h"
#include "cinematik.h"
#include "path_tracker.h"
#include "anticol.h"
#include "sdl.h"
#include "visualizer.h"

pp_path path_planned;
sdlWindow visu_window(_LONGUEUR_TER * _SCALE_SDL, _LARGEUR_TER * _SCALE_SDL, "Feed the world");

//------------------------------------------------------------------------------
void visu_draw_background(int config_terrain)
{
  // sol
  visu_window.FillRectSDL(0,0,int(_SCALE_SDL*_LONGUEUR_TER),int(_SCALE_SDL*_LARGEUR_TER),clGround);  
  // départ 1
  visu_window.FillRectSDL(0,0,int(_SCALE_SDL*0.5),int(_SCALE_SDL*0.5),clTeam1);
  visu_window.FillRectSDL(int(_SCALE_SDL*(_LONGUEUR_TER-0.5)),int(_SCALE_SDL*(_LARGEUR_TER-0.522)),int(_SCALE_SDL*0.5),int(_SCALE_SDL*0.522),clDeposeTeam1);  
  // départ 2
  visu_window.FillRectSDL(int(_SCALE_SDL*(_LONGUEUR_TER-0.5)),0,int(_SCALE_SDL*0.5),int(_SCALE_SDL*0.5),clTeam2);
  visu_window.FillRectSDL(0.,int(_SCALE_SDL*(_LARGEUR_TER-0.522)),int(_SCALE_SDL*0.5),int(_SCALE_SDL*0.522),clDeposeTeam2);  
  // Hors Terrain
  visu_window.FillRectSDL(int(_SCALE_SDL*0.5),int(_SCALE_SDL*2.122),int(_SCALE_SDL*2.),int(_SCALE_SDL*0.5),clBlack);
  // Mur Bas
  visu_window.FillRectSDL(int(_SCALE_SDL*0.5),int(_SCALE_SDL*2.1),int(_SCALE_SDL*2.),int(_SCALE_SDL*0.022),clWall);    
  visu_window.FillRectSDL(int(_SCALE_SDL*0.5),int(_SCALE_SDL*2.122),int(_SCALE_SDL*0.022),int(_SCALE_SDL*0.5),clWall);         
  visu_window.FillRectSDL(int(_SCALE_SDL*(_LONGUEUR_TER-0.522)),int(_SCALE_SDL*2.122),int(_SCALE_SDL*0.022),int(_SCALE_SDL*0.5),clWall);                         
  // Colline
  visu_window.FillRectSDL(int(_SCALE_SDL*0.67),0,int(_SCALE_SDL*1.66),int(_SCALE_SDL*0.5),clHill);
  // Mur colline
  visu_window.FillRectSDL(int(_SCALE_SDL*0.74),int(_SCALE_SDL*0.5),int(_SCALE_SDL*1.52),int(_SCALE_SDL*0.022),clWall); 
  visu_window.LigneVerticaleSDL(int(_SCALE_SDL*0.74), 0, int(_SCALE_SDL*0.5), clBlack);               
  visu_window.LigneVerticaleSDL(int(_SCALE_SDL*(_LONGUEUR_TER-0.74)), 0, int(_SCALE_SDL*0.5), clBlack);                 
  visu_window.LigneVerticaleSDL(int(_SCALE_SDL*1.24), 0, int(_SCALE_SDL*0.5), clBlack);                                    
  visu_window.LigneVerticaleSDL(int(_SCALE_SDL*(_LONGUEUR_TER-1.24)), 0, int(_SCALE_SDL*0.5), clBlack); 
  
  int configE = (config_terrain%100)/10;
  int configI = config_terrain%10;
  
  if(configE != 0 && configI !=0)
  {
    bool FakeCornE[3][3];
    bool FakeCornI[2];
    FakeCornE[0][0] = (configE==2 || configE==5 || configE==8);
    FakeCornE[1][0] = (configE==4 || configE==5 || configE==6);  
    FakeCornE[2][0] = (configE==7 || configE==8 || configE==9);    
    FakeCornE[0][1] = (configE==1 || configE==2 || configE==7);
    FakeCornE[1][1] = (configE==1 || configE==2 || configE==3);  
    FakeCornE[2][1] = (configI==2 || configI==4);
    FakeCornE[0][2] = (configE==3 || configE==4 || configE==9);
    FakeCornE[1][2] = (configI==1 || configI==3);
    FakeCornE[2][2] = false;      
    FakeCornI[0] = (configI==1 || configI==2);
    FakeCornI[1] = (configI==3 || configI==4);
    
    for(int i=0;i<3;++i)  
      for(int j=0;j<3;++j)    
        if(i != 2 || j != 2)
        {
          if(FakeCornE[i][j])
          {
            double x,y;
            x = 0.15 + 0.45*i;
            y = 2.1 - 1.378 + 0.5*j + 0.25*i;            
            visu_window.DisqueSDL(int(_SCALE_SDL*x), int(_SCALE_SDL*y), (_SCALE_SDL*0.025), clBlack);
            
            x = _LONGUEUR_TER - 0.15 - 0.45*i;
            y = 2.1 - 1.378 + 0.5*j + 0.25*i;
            visu_window.DisqueSDL(int(_SCALE_SDL*x), int(_SCALE_SDL*y), (_SCALE_SDL*0.025), clBlack);
          }
        }
        
    for(int i=0;i<2;++i) 
    {
      if(FakeCornI[i])
      {
        double x = _LONGUEUR_TER/2.;
        double y = 2.1 - 0.628 + 0.5*i; 
        visu_window.DisqueSDL(int(_SCALE_SDL*x), int(_SCALE_SDL*y), (_SCALE_SDL*0.025), clBlack);        
      }
    }
  }      
  visu_window.Load_SDL_Background();          
}
//------------------------------------------------------------------------------
void* visu_draw_robot(void*)
{
  vector_t N,T,u,v;
  point_t point[4];

  position_t pos,dest;
  
  visu_draw_background(0);
  
  while(true)
  {
    visu_window.Draw_SDL_Background();  
    
    // Draw path
    for(unsigned int i=1; i<path_planned.size(); i++)
      visu_window.LigneSDL((int)(_SCALE_SDL*path_planned[i-1].x), (int)(_SCALE_SDL*path_planned[i-1].y),
               (int)(_SCALE_SDL*path_planned[i].x), (int)(_SCALE_SDL*path_planned[i].y), clBlack);//makeColorSDL(255,0,0));

    // Draw
    pos = cine_get_position();
    dest = pt_get_dest();

    // Robot supposé
    N.x=cos(pos.a);
    N.y=sin(pos.a);
    T.x=-sin(pos.a);
    T.y=cos(pos.a);
    point[0].x=((int)((pos.x+N.x*_LONGUEUR_ROBOT/2.+T.x*_LARGEUR_ROBOT/2.)*_SCALE_SDL));     
    point[0].y=((int)((pos.y+N.y*_LONGUEUR_ROBOT/2.+T.y*_LARGEUR_ROBOT/2.)*_SCALE_SDL));
    point[1].x=((int)((pos.x+N.x*_LONGUEUR_ROBOT/2.-T.x*_LARGEUR_ROBOT/2.)*_SCALE_SDL));
    point[1].y=((int)((pos.y+N.y*_LONGUEUR_ROBOT/2.-T.y*_LARGEUR_ROBOT/2.)*_SCALE_SDL));
    point[2].x=((int)((pos.x-N.x*_LONGUEUR_ROBOT/2.-T.x*_LARGEUR_ROBOT/2.)*_SCALE_SDL));
    point[2].y=((int)((pos.y-N.y*_LONGUEUR_ROBOT/2.-T.y*_LARGEUR_ROBOT/2.)*_SCALE_SDL));
    point[3].x=((int)((pos.x-N.x*_LONGUEUR_ROBOT/2.+T.x*_LARGEUR_ROBOT/2.)*_SCALE_SDL));
    point[3].y=((int)((pos.y-N.y*_LONGUEUR_ROBOT/2.+T.y*_LARGEUR_ROBOT/2.)*_SCALE_SDL)); 
    visu_window.PolylineSDL(point, 4, clRobot);
    visu_window.LigneSDL(point[2].x, point[2].y, ((int)(pos.x*_SCALE_SDL)), ((int)(pos.y*_SCALE_SDL)), clRobot); 
    visu_window.LigneSDL(point[3].x, point[3].y, ((int)(pos.x*_SCALE_SDL)), ((int)(pos.y*_SCALE_SDL)), clRobot);

    // Destination
    N.x=cos(dest.a);
    N.y=sin(dest.a);
    T.x=-sin(dest.a)*0.03;
    T.y=cos(dest.a)*0.03; 
    visu_window.LigneSDL(((int)(_SCALE_SDL*dest.x)), ((int)(_SCALE_SDL*dest.y)), 
             ((int)(_SCALE_SDL*(dest.x+N.x*0.1))), ((int)(_SCALE_SDL*(dest.y+N.y*0.1))), clBlack);
    visu_window.LigneSDL(((int)(_SCALE_SDL*(dest.x+N.x*0.1))), ((int)(_SCALE_SDL*(dest.y+N.y*0.1))), 
             ((int)(_SCALE_SDL*(dest.x+N.x*0.07+T.x))), ((int)(_SCALE_SDL*(dest.y+N.y*0.07+T.y))), clBlack);
    visu_window.LigneSDL(((int)(_SCALE_SDL*(dest.x+N.x*0.1))), ((int)(_SCALE_SDL*(dest.y+N.y*0.1))), 
             ((int)(_SCALE_SDL*(dest.x+N.x*0.07-T.x))), ((int)(_SCALE_SDL*(dest.y+N.y*0.07-T.y))), clBlack);
    
    // Captors
    /*for(int i=0;i<4;i++)
    {
      vector_t pos = captor_get_position(i);
      Uint32 color;
      switch(captor_get_status(i))
      {
        case 0: color = clBlack; break;
        case 1: color = makeColorSDL(255, 0, 180); break;
        case 2: color = makeColorSDL(255, 0, 0); break;
        case 3: color = makeColorSDL(60, 60, 60); break;
      }
      DisqueSDL(((int)(_SCALE_SDL*pos.x)),
                ((int)(_SCALE_SDL*pos.y)),
                ((int)(_SCALE_SDL*0.01)),color);        
    } */ 
    visu_window.RefreshSDL();             
    usleep(40000);
  }
}
//------------------------------------------------------------------------------
void visu_draw_path(pp_path &p)
{
  path_planned.resize(p.size());
  for(unsigned int i=0; i<p.size(); i++)
    path_planned[i] = p[i];    
}
//------------------------------------------------------------------------------
