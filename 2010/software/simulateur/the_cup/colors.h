#ifndef COLORS_H
#define COLORS_H

#define COLOR_SDL(R,G,B) (256*R + G)*256 + B

#define COLOR_GL(R,G,B)  (double(R)/255.),(double(G)/255.),(double(B)/255.)

#define clWhite COLOR_SDL(255,255,255)
#define glWhite COLOR_GL(255,255,255)
#define clBlack COLOR_SDL(0,0,0)
#define glBlack COLOR_GL(0,0,0)

// Playground
#define clTeam1        COLOR_SDL(0,67,137)
#define glTeam1        COLOR_GL(0,67,137)
#define clTeam2        COLOR_SDL(242,191,0)
#define glTeam2        COLOR_GL(242,191,0)
#define clDeposeTeam1  COLOR_SDL(37,107,85)
#define glDeposeTeam1  COLOR_GL(37,107,85)
#define clDeposeTeam2  COLOR_SDL(125,154,33)
#define glDeposeTeam2  COLOR_GL(125,154,33)
#define clGround       COLOR_SDL(74,165,66)
#define glGround       COLOR_GL(74,165,66)
#define clWall         COLOR_SDL(55,54,59)
#define glWall         COLOR_GL(55,54,59)
#define clHill         COLOR_SDL(197,200,207)
#define glHill         COLOR_GL(197,200,207)

// Eléments de jeu
#define clTomate       COLOR_SDL(249,0,0)
#define glTomate       COLOR_GL(249,0,0)
#define clOrange       COLOR_SDL(245,143,17)
#define glOrange       COLOR_GL(245,143,17)
#define clMais         COLOR_SDL(229,223,204)
#define glMais         COLOR_GL(229,223,204)
#define clFake         COLOR_SDL(51,54,52)
#define glFake         COLOR_GL(51,54,52)


// Robot
#define clRobot COLOR_SDL(200,200,200)
#define glRobot COLOR_GL (200,200,200)


#endif
