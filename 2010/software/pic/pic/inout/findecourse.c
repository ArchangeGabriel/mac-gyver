// findecourse.c //

//#include "digitals.h"
#include <pic18fregs.h>
#include "moteurs.h"

#include "debugs.h"

#define NB_FDC 8

unsigned char fdc_on;
unsigned char fdc[NB_FDC][4]; // Etat moteurs, Masque, Etat capteurs, Effet
unsigned char motor_order;

void init_fdc()
{
  unsigned char i,j;
  fdc_on = 0;
  motor_order = 0;
  for(i=0;i<NB_FDC;i++)
    for(j=0;j<4;j++)
      fdc[i][j] = 0;
}

void set_fdc(unsigned char * donnees, unsigned char longueur)
{
  unsigned char i,j;
  for(i=0;i<longueur;i++)
  {
    for(j=0;j<4;j++)
    {
      fdc[i][j] = donnees[(i<<2) + j];
    }
  }
  for(j=0;j<4;j++) write_debug(j,fdc[0][j]);
  for(j=0;j<4;j++) write_debug(j+4,fdc[1][j]);
  fdc_on = longueur;
  INTCON2bits.INTEDG0 = 0;
  INTCON2bits.INTEDG1 = 0;
  INTCONbits.INT0IF = 1;
  INTCON3bits.INT1IF = 1;
  if(fdc_on)
  {
    INTCONbits.INT0IE = 1;
    INTCON3bits.INT1IE = 1;
  }
}

void verif_fdc(unsigned char o)
{
  unsigned char i,temp;
  temp = o;
//  write_debug(4,temp);
  for(i=0;i<fdc_on;i++)
  {
    if((temp & fdc[i][0]) && ((PORTB & fdc[i][1]) == fdc[i][2])) temp &= fdc[i][3];
  }
  motor_order = temp;
  set_motors(motor_order);
//  write_debug(5,motor_order);
}

void gestionfdc()
{
  verif_fdc(motor_order);
}
