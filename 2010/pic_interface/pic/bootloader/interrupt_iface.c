/*-------------------------------------------------------------------------
  interrupt_iface.c - Interruptions interface
-------------------------------------------------------------------------*/


/* TODO Interrupt entry points should be defines in config.h */

/* Interrupt vectors */
#pragma code high_priority_isr 0x2030
void high_priority_isr(void) interrupt 1
{
}

#pragma code low_priority_isr 0x4000
void low_priority_isr(void) interrupt 2
{
}

