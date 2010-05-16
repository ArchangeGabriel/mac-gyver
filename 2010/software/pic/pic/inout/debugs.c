/* debug.c */

#include "ep2.h"
#include "proto.h"
#include "app_usb.h"
unsigned char debug_data[9];
unsigned char debug[8];

void init_debug()
{
    unsigned char i;
    for(i=0;i<8;i++) debug[i] = 0;
    for(i=0;i<9;i++) debug_data[i] = 0;
    debug_data[0] = DEBUGS;
}

void send_debug()
{
    unsigned char i;
    while(EP_IN_BD(2).Stat.uc & BDS_USIE); // wait the Uown bit to be cleared
    ep2_num_bytes_to_send = 9;
    ep2_source_data = &debug_data[1];
    for(i=0;i<8;i++) ep2_source_data[i] = debug[i];
    ep2_source_data = debug_data;
    ep2_source_data[0] = DEBUGS;
    my_prepare_ep2_in();
}

void write_debug(unsigned char i, unsigned char d)
{
    debug[i] = d;
}
