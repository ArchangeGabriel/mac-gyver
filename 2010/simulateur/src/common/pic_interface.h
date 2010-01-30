// comm.h //

#ifndef _COMM_H
#define _COMM_H

void set_speed(unsigned char vitesse1, unsigned char vitesse2); // change la vitesse des moteurs en ces valeurs (0 ne change rien, 1 arriere toute, 128 arret, 255 avant toute)
unsigned char get_codeuses(int *codeuse1, unsigned char *sens1, int *codeuse2, unsigned char *sens2); // met les valeurs courantes des codeuses dans codeuse1 et codeuse2, et les sens dans sens1 et sens2 / Retourne l'indice de l'echange usb (il s'incremente tous les pas de temps, au cas ou on a rate une trame usb)

unsigned char get_digital_in(void); // retourne les 7 entrees numeriques (le bit de poids fort sera nul)
void get_analog_in(unsigned short *result, unsigned char length); // rempli result des length (1-7) premieres entrees analogiques

void set_DC_motor(unsigned char number, char sens); // met le moteur number (1-4) dans le sens sens (-1, 0 arret, 1)
void set_servo(unsigned char number, unsigned char position); //met le servo number (1-4) en position position (intervalle a determiner)

#endif // _COMM_H
