/*-------------------------------------------------------------------------
  flash.c - Program memory flashing process
-------------------------------------------------------------------------*/

#include "config.h"
#include "common_types.h"
#include <pic18fregs.h>
#include "flash.h"

uint ep2_num_bytes_to_send;
uchar *ep2_source_data;

void write_block(__data uchar *src)
{
    static uchar i;

    __asm
    
    tblrd*-

    __endasm;
    
    for(i = 0; i < 32; i++)
    {
        TABLAT = *src;
        src++;
        __asm
        
        tblwt+*

        __endasm;
    } 
    // Beware : when WR is set, TBLPTR have to be inside the 32 byte range to be written
    // so it should be decremented...
    
    __asm
    
    bsf     _EECON1, 7 // EEPGD
    bcf     _EECON1, 6 // CFGS
    bsf     _EECON1, 2 // WREN
    bcf     _INTCON, 7 // GIE
    
    // required erase sequence
    movlw   0x55
    movwf   _EECON2
    movlw   0xaa
    movwf   _EECON2
    bsf     _EECON1, 1 // WR

    // stall here for 2ms

    __endasm;
}

void erase_block()
{
    // Erase a 64 bytes block
    __asm
    // address set by the caller 

    // Point to flash program memory
    bsf     _EECON1, 7 // EEPGD
    bcf     _EECON1, 6 // CFGS
    bsf     _EECON1, 2 // WREN
    bsf     _EECON1, 4 // FREE
    bcf     _INTCON, 7 // GIE
    
    // required erase sequence
    movlw   0x55
    movwf   _EECON2
    movlw   0xaa
    movwf   _EECON2
    bsf     _EECON1, 1 // WR
    
    // stall here for 2ms
    
    __endasm;
}
