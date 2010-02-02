// digitals.c //

#include <pic18fregs.h>
#include "ep2.h"
#include "proto.h"
#include "pining.h"


unsigned char dig_data[2];

void send_digitals_in()
{
    dig_data[0] = DIGITS;
    dig_data[1] = PORTB;
    ep2_source_data = dig_data;
    ep2_num_bytes_to_send = 2;
    my_prepare_ep2_in();
}

void init_digitals_in(void)
{
    TRISB = 0xff;
    INTCON2bits.RBPU = 1; // internal pull-up on port B
}

