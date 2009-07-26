/*-----------------------------------------------------------------
    strmusart.c - usart stream putchar

    Modified for pic16 port, by Vangelis Rokas, 2004 (vrokas@otenet.gr)
    
    Written By - Sandeep Dutta . sandeep.dutta@usa.net (1999)

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by the
    Free Software Foundation; either version 2, or (at your option) any
    later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this program; if not, write to the Free Software
    Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

    In other words, you are welcome to use, share and improve this program.
    You are forbidden to forbid anyone else to use, share and improve
    what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

/*
** $Id: strmgpsim.c 3714 2005-04-02 13:13:53Z vrokas $
*/

extern WREG;

/* note that USART should already been initialized */
void __stream_gpsim_putchar(unsigned char c) __wparam __naked
{
  c;
  __asm
    MOVFF	_WREG, 0xf7f
    RETURN
  __endasm;
}
