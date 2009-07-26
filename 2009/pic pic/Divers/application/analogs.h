// analogs.h //

#ifndef _ANALOGS_H
#define _ANALOGS_H

#define NB_ANALOGS 4 // AN0...3
#define TRISA_CONF 0xCF // Input pour RA0...3
//#define TRISE_CONF 0x07


#define NB_DIG (16-NB_ANALOGS) // Pour registres (voir analogs.c)

void init_adconversion(void);
void maj_result(void);

#endif // _ANALOGS_H
