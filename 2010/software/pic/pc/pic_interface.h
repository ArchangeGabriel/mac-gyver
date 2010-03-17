// comm.h //

#ifndef _COMM_H
#define _COMM_H

// Toutes ces fonctions retournent -1 en cas d'erreur, et un resultat >= 0 si tout s'est bien passe.
// get_codeuses, get_digital_in, init_analog_in, get_analog_in peuvent retourner des valeurs interessantes (genre leur resultat quoi !).

int set_speed(unsigned char vitesse1, unsigned char vitesse2); // change la vitesse des moteurs en ces valeurs (0 ne change rien, 1 arriere toute, 128 arret, 255 avant toute). Retourne 1 si ok, -1 sinon
int get_codeuses(int *codeuse1, unsigned char *sens1, int *codeuse2, unsigned char *sens2); // met les valeurs courantes des codeuses dans codeuse1 et codeuse2, et les sens dans sens1 et sens2 / Retourne l'indice de l'echange usb (il s'incremente tous les pas de temps, au cas ou on a rate une trame usb) si ok, -1 sinon
// cette fonction est destinee a etre appelee en boucle dans un thread (cf testusb.cpp)

int get_digital_in(void); // retourne de 0 à 127 (ie les 7 entrees numeriques) si ok, -1 sinon
int init_analog_in(unsigned char number); // defini le nombre NB_ANALOG d'entrees analogiques. Retourne min(number, NB max d'analogiques de la carte) si ok, -1 sinon. Doit etre appelee avant le premier get_analog_in.
int get_analog_in(unsigned short *result, unsigned char length); // rempli result des length (ou NB_ANALOG si length trop grand) premieres entrees analogiques. Retourne ne nombre de valeurs effectivement ecrites (ie min(length, NB_ANALOG)) si ok, -1 sinon

int set_DC_motor(unsigned char number, char sens); // met le moteur number (1-4) dans le sens sens (-1, 0 arret, 1). Retourne 1 si ok, -1 sinon.
int set_servo(unsigned char number, unsigned char position); //met le servo number (1-4) en position position (1-255) (position = 0 rend le servo libre). Retourne 1 si ok, -1 sinon

int setup_usb_connexions(); // Initialise les connexions usb avec la carte. Retourne 1 si tout fonctionne, un resultat negatif sinon.
void shut_usb(); // Ferme proprement les connexions usb.
int repare_usb(); // Tente de reparer les connexions usb cassees. Retourne 1 si tout fonctionne, un resultat negatif sinon.

#endif // _COMM_H
