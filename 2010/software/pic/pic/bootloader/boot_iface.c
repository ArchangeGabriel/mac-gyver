/*---------------------------------------------------------------------------
  boot_iface.c - Boot interface for applications
---------------------------------------------------------------------------*/

#pragma code boot_goto_table 0x1c

// This dummy function is in fact a table of goto exported to
// the application

// TODO The boot version should be a define from config.h

void boot_goto_table(void) __naked
{
    __asm
    
    extern _dispatch_usb_event
    extern _reset_usb
    extern _usb_sleep
    extern _fill_in_buffer

    extern _ep0_init
    extern _ep0_in
    extern _ep0_setup

    extern _ep1_init
    extern _ep1_out

    extern _ep2_init
    extern _ep2_in
    
    extern __mullong
    extern __mulint
    extern __gptrget1
    extern __gptrget2
    extern __gptrget3
    extern __gptrget4
    extern __gptrput1
    extern __gptrput2
    extern __gptrput3
    extern _delay10ktcy
    extern _delay1ktcy
    extern _delay10tcy

//    extern _getbuffersaddresses

;--------------------------------
    
    DW 0x0010 ; Boot Version in BCD format

    goto _dispatch_usb_event
    goto _reset_usb
    goto _usb_sleep
    goto _fill_in_buffer

    goto _ep0_init
    goto _ep0_in
    goto _ep0_setup

    goto _ep1_init
    goto _ep1_out

    goto _ep2_init
    goto _ep2_in

    goto __mullong
    goto __mulint
    goto __gptrget1
    goto __gptrget2
    goto __gptrget3
    goto __gptrget4
    goto __gptrput1
    goto __gptrput2
    goto __gptrput3
    goto _delay10ktcy
    goto _delay1ktcy
    goto _delay10tcy
    
  //  goto _getbuffersaddresses

    __endasm;
}
    
