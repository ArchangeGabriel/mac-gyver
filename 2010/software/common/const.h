#ifndef CONSTH
#define CONSTH


#include "../common/simul.h"

#ifdef SIMULATION
#ifndef PC_INCLUDE
#define _USE_SIMUL_CONST
#endif
#endif


// Initial positions
/*#define _POS_INIT_X_BLUE   0.195
#define _POS_INIT_Y_BLUE   0.185
#define _POS_INIT_A_BLUE   90.

#define _POS_INIT_X_YELLOW 2.70
#define _POS_INIT_Y_YELLOW 0.3
#define _POS_INIT_A_YELLOW 120*/

#define _POS_INIT_X_BLUE   0.35
#define _POS_INIT_Y_BLUE   0.14
#define _POS_INIT_A_BLUE   90.

#define _POS_INIT_X_YELLOW 2.65
#define _POS_INIT_Y_YELLOW 0.14
#define _POS_INIT_A_YELLOW 90

// Robot
#define _LONGUEUR_ROBOT 0.27
#define _LARGEUR_ROBOT  0.29

// Webcam
// Webcam front
#define WC1_resX     640        // Résolution (largeur) de l'image
#define WC1_resY     480        // Résolution (hauteur) de l'image
#define WC1_offsetX  0.085      // en mètres: Décalage entre le centre de la webcam et le centre de l'objet porteur (abcisse)
#define WC1_offsetY  -0.135     // en mètres: Décalage entre le centre de la webcam et le centre de l'objet porteur (ordonnée)
#define WC1_offsetZ  0.07       // en mètres: Décalage entre le centre de la webcam et le sol
#define WC1_dirX     1.         // Vecteur de direction: orientation de la webcam dans le référentiel de l'objet porteur (abscisse)
#define WC1_dirY     -0.916     // Vecteur de direction: orientation de la webcam dans le référentiel de l'objet porteur (ordonnée)
#define WC1_dirZ     0.         // Vecteur de direction: orientation de la webcam dans le référentiel de l'objet porteur (cote)
#define WC1_Hfocal   18.8       // Angle de demi-focale

// Webcam top
#define WC2_resX     640        // Résolution (largeur) de l'image
#define WC2_resY     480        // Résolution (hauteur) de l'image
#define WC2_offsetX  0.         // en mètres: Décalage entre le centre de la webcam et le centre de l'objet porteur (abcisse)
#define WC2_offsetY  0.         // en mètres: Décalage entre le centre de la webcam et le centre de l'objet porteur (ordonnée)
#define WC2_offsetZ  0.40       // en mètres: Décalage entre le centre de la webcam et le sol
#define WC2_dirX     1.         // Vecteur de direction: orientation de la webcam dans le référentiel de l'objet porteur (abscisse)
#define WC2_dirY     0.         // Vecteur de direction: orientation de la webcam dans le référentiel de l'objet porteur (ordonnée)
#define WC2_dirZ     -0.333     // Vecteur de direction: orientation de la webcam dans le référentiel de l'objet porteur (cote)
#define WC2_Hfocal   18.8       // Angle de demi-focale





// Other constants
#ifdef _USE_SIMUL_CONST

#define _LONGUEUR_TER   Simul->info.longueur_ter
#define _LARGEUR_TER    Simul->info.largeur_ter
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

#define _LONGUEUR_TER    3.
#define _LARGEUR_TER     2.1
#define _RAYON_ROUE     0.025
#define _MOTOR_K        19.
#define _FREQ_CODER     ((500./(2.*M_PI))*_MOTOR_K/_RAYON_ROUE ) //(500./(2.*M_PI))
#define _ROUE_X         -0.103
#define _ROUE_Y         0.24
#define _MASSE          10.
#define _CONST_G        9.8
#define _ADHERE_ROUE    0.65
#define _CONST_TORQUE   0.0133
#define _CONST_SPEED    (695.8 * 2. * M_PI / 60.)
#define _RAP_REDUC      19.
#define _RENDEMNT       0.7
//#define _TENSION        14.4

#endif

#endif
