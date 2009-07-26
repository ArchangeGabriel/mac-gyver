/*-------------------------------------------------------------------------
  config.c - PIC configuration words

             (c) 2006 Pierre Gaufillet <pierre.gaufillet@magic.fr> 

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
-------------------------------------------------------------------------*/

/* $Id: config.c,v 1.1 2006/04/17 20:36:15 gaufille Exp $ */

#include <pic18fregs.h>
#include "flash.h"

/* Set the PIC config words */
code char at __CONFIG1L _conf0  = _USBPLL_CLOCK_SRC_FROM_96MHZ_PLL_2_1L &			//source clock USB : PLL 		ok
                                  _CPUDIV__OSC1_OSC2_SRC___1__96MHZ_PLL_SRC___2__1L &		//clock cpu : 96/2 MHz			ok
                                  _PLLDIV_DIVIDE_BY_4__16MHZ_INPUT__1L;				//clock input : 16MHz			ok
                                  
code char at __CONFIG1H _conf1  = _IESO_OFF_1H &						// ??					??
                                  _FCMEN_OFF_1H &						//fail safe monitor : off		??
                                  _OSC_HS__HS_PLL__USB_HS_1H;					//1H source clock : PLL			ok

code char at __CONFIG2L _conf2  = _VREGEN_ON_2L &						//Regu USB device : on			ok
                                  _BODENV_2_0V_2L &						//Brownout reset voltage : 2V		osef
                                  _BODEN_OFF_2L &						//Brownout reset : off			ok
                                  _PUT_ON_2L;							//Power up timer : on			ok

code char at __CONFIG2H _conf3  = _WDTPS_1_1_2H &						//Watchdog prescaler			osef
                                  _WDT_DISABLED_CONTROLLED_2H;					//Watchdog : off			ok

code char at __CONFIG3H _conf4  = _MCLRE_MCLR_ON_RE3_OFF_3H &					//MCLR on pin RE3 : on			ok
                                  _LPT1OSC_OFF_3H &						//Low Power Timer 1 : on		ok
                                  _PBADEN_PORTB_4_0__CONFIGURED_AS_DIGITAL_I_O_ON_RESET_3H &	//Port B : digital			ok
                                  _CCP2MUX_RC1_3H;						//CCP2 : RC1				ok

code char at __CONFIG4L _conf5  = _BACKBUG_OFF_4L &						//Debugger : off			ok
                                  _ENHCPU_OFF_4L & 						//Extended CPU : off			ok
                                  _ENICPORT_OFF_4L & 						//ICD-ICSP : off			ok
                                  _LVP_OFF_4L & 						//Low Voltage Program : off		ok
                                  _STVR_OFF_4L;							//Stack overflow reset : off		ok

code char at __CONFIG5L _conf6  = _CP_0_OFF_5L & 						//Code protection : off			ok
                                  _CP_1_OFF_5L & 						//..
                                  _CP_2_OFF_5L & 						//..
		                  _CP_3_OFF_5L;							//..

code char at __CONFIG5H _conf7  = _CPB_OFF_5H & 						//boot read protection : off		ok
                                  _CPD_OFF_5H;							//data read protection : off		ok

code char at __CONFIG6L _conf8  = _WRT_0_ON_6L & 						//Table write protection 1 : on		ok
                                  _WRT_1_OFF_6L & 						//Table write protection : off		ok
                                  _WRT_2_OFF_6L &						//..
		                  _WRT_3_OFF_6L;						//..

code char at __CONFIG6H _conf9  = _WRTB_ON_6H & 						//boot write protection : on		ok
                                  _WRTC_ON_6H & 						//config write protection : on		ok
                                  _WRTD_OFF_6H;							//data write protection : off		ok

code char at __CONFIG7L _conf10 = _EBTR_0_OFF_7L & 						//Table read protection : off		ok
                                  _EBTR_1_OFF_7L & 						//..
                                  _EBTR_2_OFF_7L &						//.. 
				  _EBTR_3_OFF_7L;						//..

code char at __CONFIG7H _conf11 = _EBTRB_OFF_7H;						//boot table read protection : off	ok

/* Memory sections for flash operations */
const uchar section_descriptor [22] = {3,
                                     FLASH_SECTION_READ,
                                     0x00, 0x00, 0x00,
                                     0xff, 0x1f, 0x00,
                                     FLASH_SECTION_READ | FLASH_SECTION_WRITE,
                                     0x00, 0x20, 0x00,
                                     0xff, 0x7f, 0x00,
                                     FLASH_SECTION_READ,
                                     0x00, 0x00, 0x30,
                                     0x0d, 0x00, 0x30};

