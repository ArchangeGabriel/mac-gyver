// commandes.h //

#ifndef _COMMANDES_H
#define _COMMANDES_H

/////////////////////////////////////////////////////////////////////
// Format du 1° octet des paquets
// Analogs / Digitals / Moteurs / Servo / Warnings [3 bits] / Recu ou Erreur

#define ANALOGS 0x80           // 10000000
#define DIGITALS 0x40          // 01000000
#define MOTEURS 0x20           // 00100000
#define SERVO 0x10             // 00010000
#define WARNINGS 0x0e          // 00001110
#define ERREUR_CHECKSUM 0x01   // 00000001
#define RECU 0x00              // 00000000

// WARNINGS // nU, GOGO, Erreur Servo
#define GOGOGRINGO 0x04        // 00000100
#define ERREUR_SERVO 0x02      // 00000010

/////////////////////////////////////////////////////////////////////
// Format des 2 octets moteur
// Atteinte (0) ou visee (1) (mot 1) / Initialisation / Position moteur 1 (visee ou atteinte) [6 bits] 

#define POSITION_MOTEUR_1 0x3f // 00111111
#define ACTIV_MOTEUR_1 0x80    // 10000000
#define INITIALISATION_1 0x40  // 01000000

//  nU [4 bits] / Atteinte (0) ou visee (1) (mot 2) / Initialisation / Position moteur 2 (visee ou atteinte) [2 bits]

#define POSITION_MOTEUR_2 0x03 // 00000011
#define INITIALISATION_2 0x04  // 00000100
#define ACTIV_MOTEUR_2 0x08    // 00001000
#define MOTEUR_3 0x30          // 00110000
#define ACTIV_MOTEUR_3 0x40    // 01000000

#define MOT3_ARRET 0x00
#define MOT3_ENTRANT 0x10
#define MOT3_SORTANT 0x20

#endif // _COMMANDES_H
