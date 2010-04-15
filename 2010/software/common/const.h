#ifndef CONSTH
#define CONSTH


#include "../common/simul.h"

#ifdef SIMULATION
#ifndef PC_INCLUDE
#define _USE_SIMUL_CONST
#endif
#endif


#ifdef _USE_SIMUL_CONST
#define _LONGUEUR_TER   Simul->info.longueur_ter
#define _LARGEUR_TER    Simul->info.largeur_ter
#define _POS_INIT_X     Simul->info.pos_init.x
#define _POS_INIT_Y     Simul->info.pos_init.y
#define _POS_INIT_A     Simul->info.angle_init
#define _RAYON_ROUE     Simul->info.rayon_roue
#define _FREQ_CODER     Simul->info.periodes_codeuse
#define _MOTOR_K        Simul->info.moteur_k
#define _ROUE_X         Simul->info.roues_x
#define _ROUE_Y         Simul->info.roues_y
#define _MASSE          Simul->info.masse
#define _CONST_G        Simul->info.g
#define _ADHERE_ROUE    Simul->info.moteur_e
#define _CONST_TORQUE   Simul->info.moteur_Kc
#define _CONST_SPEED    Simul->info.moteur_Kv
#define _RAP_REDUC      Simul->info.moteur_k
#define _RENDEMNT       Simul->info.moteur_n
#define _TENSION        Simul->info.Umax
#else
#define _LONGUEUR_TER   3.
#define _LARGEUR_TER    2.1
/*
#define _POS_INIT_X     0.2
#define _POS_INIT_Y     0.2
#define _POS_INIT_A     45.
*/
#define _POS_INIT_X     0.5
#define _POS_INIT_Y     0.5
#define _POS_INIT_A     90.
#define _RAYON_ROUE     0.025
#define _FREQ_CODER     (500./(2.*M_PI))
#define _MOTOR_K        19.
#define _ROUE_X         -0.09
#define _ROUE_Y         0.27
#define _MASSE          10.
#define _CONST_G        9.8
#define _ADHERE_ROUE    0.65
#define _CONST_TORQUE   0.0133
#define _CONST_SPEED    (695.8 * 2. * M_PI / 60.)
#define _RAP_REDUC      19.
#define _RENDEMNT       0.7
#define _TENSION        14.4
#endif

#define _DEMI_FOCALE    26.35                 //     (en degrées)
#define _POS_CAM1_X     10.0
#define _POS_CAM1_Y     13.5
#define _POS_CAM1_A     0.0
#define _LONGUEUR_ROBOT 0.27
#define _LARGEUR_ROBOT  0.29

#endif
