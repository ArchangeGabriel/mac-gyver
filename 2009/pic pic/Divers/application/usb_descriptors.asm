;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.6.1 #4482 (Mar 15 2009)
; This file generated Tue May 19 21:44:03 2009
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f4550

	radix dec

;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global _null_function
	global _device_descriptor
	global _default_cfg
	global _flash_cfg
	global _application_cfg
	global _configuration_descriptor
	global _str0
	global _str1
	global _str2
	global _str3
	global _str4
	global _str5
	global _string_descriptor
	global _ep_init
	global _ep_in
	global _ep_out
	global _ep_setup

;--------------------------------------------------------
; extern variables in this module
;--------------------------------------------------------
	extern _ep_bdt
	extern _SPPDATAbits
	extern _SPPCFGbits
	extern _SPPEPSbits
	extern _SPPCONbits
	extern _UFRMLbits
	extern _UFRMHbits
	extern _UIRbits
	extern _UIEbits
	extern _UEIRbits
	extern _UEIEbits
	extern _USTATbits
	extern _UCONbits
	extern _UADDRbits
	extern _UCFGbits
	extern _UEP0bits
	extern _UEP1bits
	extern _UEP2bits
	extern _UEP3bits
	extern _UEP4bits
	extern _UEP5bits
	extern _UEP6bits
	extern _UEP7bits
	extern _UEP8bits
	extern _UEP9bits
	extern _UEP10bits
	extern _UEP11bits
	extern _UEP12bits
	extern _UEP13bits
	extern _UEP14bits
	extern _UEP15bits
	extern _PORTAbits
	extern _PORTBbits
	extern _PORTCbits
	extern _PORTDbits
	extern _PORTEbits
	extern _LATAbits
	extern _LATBbits
	extern _LATCbits
	extern _LATDbits
	extern _LATEbits
	extern _TRISAbits
	extern _TRISBbits
	extern _TRISCbits
	extern _TRISDbits
	extern _TRISEbits
	extern _OSCTUNEbits
	extern _PIE1bits
	extern _PIR1bits
	extern _IPR1bits
	extern _PIE2bits
	extern _PIR2bits
	extern _IPR2bits
	extern _EECON1bits
	extern _RCSTAbits
	extern _TXSTAbits
	extern _T3CONbits
	extern _CMCONbits
	extern _CVRCONbits
	extern _ECCP1ASbits
	extern _ECCP1DELbits
	extern _BAUDCONbits
	extern _CCP2CONbits
	extern _CCP1CONbits
	extern _ADCON2bits
	extern _ADCON1bits
	extern _ADCON0bits
	extern _SSPCON2bits
	extern _SSPCON1bits
	extern _SSPSTATbits
	extern _T2CONbits
	extern _T1CONbits
	extern _RCONbits
	extern _WDTCONbits
	extern _HLVDCONbits
	extern _OSCCONbits
	extern _T0CONbits
	extern _STATUSbits
	extern _FSR2Hbits
	extern _BSRbits
	extern _FSR1Hbits
	extern _FSR0Hbits
	extern _INTCON3bits
	extern _INTCON2bits
	extern _INTCONbits
	extern _TBLPTRUbits
	extern _PCLATHbits
	extern _PCLATUbits
	extern _STKPTRbits
	extern _TOSUbits
	extern _usb_device_state
	extern _usb_active_cfg
	extern _usb_active_alt_setting
	extern _ep2_num_bytes_to_send
	extern _ep2_source_data
	extern _an_data
	extern _mydata
	extern _SPPDATA
	extern _SPPCFG
	extern _SPPEPS
	extern _SPPCON
	extern _UFRML
	extern _UFRMH
	extern _UIR
	extern _UIE
	extern _UEIR
	extern _UEIE
	extern _USTAT
	extern _UCON
	extern _UADDR
	extern _UCFG
	extern _UEP0
	extern _UEP1
	extern _UEP2
	extern _UEP3
	extern _UEP4
	extern _UEP5
	extern _UEP6
	extern _UEP7
	extern _UEP8
	extern _UEP9
	extern _UEP10
	extern _UEP11
	extern _UEP12
	extern _UEP13
	extern _UEP14
	extern _UEP15
	extern _PORTA
	extern _PORTB
	extern _PORTC
	extern _PORTD
	extern _PORTE
	extern _LATA
	extern _LATB
	extern _LATC
	extern _LATD
	extern _LATE
	extern _TRISA
	extern _TRISB
	extern _TRISC
	extern _TRISD
	extern _TRISE
	extern _OSCTUNE
	extern _PIE1
	extern _PIR1
	extern _IPR1
	extern _PIE2
	extern _PIR2
	extern _IPR2
	extern _EECON1
	extern _EECON2
	extern _EEDATA
	extern _EEADR
	extern _RCSTA
	extern _TXSTA
	extern _TXREG
	extern _RCREG
	extern _SPBRG
	extern _SPBRGH
	extern _T3CON
	extern _TMR3L
	extern _TMR3H
	extern _CMCON
	extern _CVRCON
	extern _ECCP1AS
	extern _ECCP1DEL
	extern _BAUDCON
	extern _CCP2CON
	extern _CCPR2L
	extern _CCPR2H
	extern _CCP1CON
	extern _CCPR1L
	extern _CCPR1H
	extern _ADCON2
	extern _ADCON1
	extern _ADCON0
	extern _ADRESL
	extern _ADRESH
	extern _SSPCON2
	extern _SSPCON1
	extern _SSPSTAT
	extern _SSPADD
	extern _SSPBUF
	extern _T2CON
	extern _PR2
	extern _TMR2
	extern _T1CON
	extern _TMR1L
	extern _TMR1H
	extern _RCON
	extern _WDTCON
	extern _HLVDCON
	extern _OSCCON
	extern _T0CON
	extern _TMR0L
	extern _TMR0H
	extern _STATUS
	extern _FSR2L
	extern _FSR2H
	extern _PLUSW2
	extern _PREINC2
	extern _POSTDEC2
	extern _POSTINC2
	extern _INDF2
	extern _BSR
	extern _FSR1L
	extern _FSR1H
	extern _PLUSW1
	extern _PREINC1
	extern _POSTDEC1
	extern _POSTINC1
	extern _INDF1
	extern _WREG
	extern _FSR0L
	extern _FSR0H
	extern _PLUSW0
	extern _PREINC0
	extern _POSTDEC0
	extern _POSTINC0
	extern _INDF0
	extern _INTCON3
	extern _INTCON2
	extern _INTCON
	extern _PRODL
	extern _PRODH
	extern _TABLAT
	extern _TBLPTRL
	extern _TBLPTRH
	extern _TBLPTRU
	extern _PCL
	extern _PCLATH
	extern _PCLATU
	extern _STKPTR
	extern _TOSL
	extern _TOSH
	extern _TOSU
	extern _ep0_init
	extern _ep1_init
	extern _ep2_init
	extern _my_ep1_init
	extern _my_ep2_init
	extern _ep0_in
	extern _ep2_in
	extern _my_ep2_in
	extern _ep1_out
	extern _my_ep1_out
	extern _ep0_setup

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; I code from now on!
; ; Starting pCode block
S_usb_descriptors__null_function	code
_null_function:
	return
	
; ; Starting pCode block for Ival
	code
_device_descriptor:
	DB	0x12, 0x01, 0x00, 0x02, 0xff, 0x00, 0xff, 0x08, 0xa5, 0xa5, 0x02, 0x00
	DB	0x00, 0x01, 0x01, 0x02, 0x03, 0x03
; ; Starting pCode block for Ival
_default_cfg:
	DB	0x09, 0x02, 0x12, 0x00, 0x01, 0x01, 0x04, 0xc0, 0x00, 0x09, 0x04, 0x00
	DB	0x00, 0x00, 0xff, 0xff, 0xff, 0x04
; ; Starting pCode block for Ival
_flash_cfg:
	DB	0x09, 0x02, 0x20, 0x00, 0x01, 0x02, 0x05, 0xc0, 0x00, 0x09, 0x04, 0x00
	DB	0x00, 0x02, 0xff, 0xff, 0xff, 0x05, 0x07, 0x05, 0x01, 0x02, 0x40, 0x00
	DB	0x00, 0x07, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00
; ; Starting pCode block for Ival
_application_cfg:
	DB	0x09, 0x02, 0x20, 0x00, 0x01, 0x03, 0x04, 0xc0, 0x00, 0x09, 0x04, 0x00
	DB	0x00, 0x02, 0xff, 0xff, 0xff, 0x04, 0x07, 0x05, 0x01, 0x02, 0x40, 0x00
	DB	0x00, 0x07, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00
; ; Starting pCode block for Ival
_configuration_descriptor:
	DB	LOW(_default_cfg), HIGH(_default_cfg), UPPER(_default_cfg), LOW(_flash_cfg), HIGH(_flash_cfg), UPPER(_flash_cfg), LOW(_application_cfg), HIGH(_application_cfg), UPPER(_application_cfg)
; ; Starting pCode block for Ival
_str0:
	DB	0x04, 0x03, 0x09, 0x04
; ; Starting pCode block for Ival
_str1:
	DB	0x1c, 0x03, 0x56, 0x00, 0x61, 0x00, 0x73, 0x00, 0x63, 0x00, 0x6f, 0x00
	DB	0x20, 0x00, 0x50, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x6a, 0x00, 0x65, 0x00
	DB	0x63, 0x00, 0x74, 0x00
; ; Starting pCode block for Ival
_str2:
	DB	0x1c, 0x03, 0x54, 0x00, 0x6f, 0x00, 0x67, 0x00, 0x67, 0x00, 0x6c, 0x00
	DB	0x65, 0x00, 0x20, 0x00, 0x6d, 0x00, 0x6f, 0x00, 0x64, 0x00, 0x75, 0x00
	DB	0x6c, 0x00, 0x65, 0x00
; ; Starting pCode block for Ival
_str3:
	DB	0x04, 0x03, 0x31, 0x00
; ; Starting pCode block for Ival
_str4:
	DB	0x10, 0x03, 0x44, 0x00, 0x65, 0x00, 0x66, 0x00, 0x61, 0x00, 0x75, 0x00
	DB	0x6c, 0x00, 0x74, 0x00
; ; Starting pCode block for Ival
_str5:
	DB	0x0c, 0x03, 0x46, 0x00, 0x6c, 0x00, 0x61, 0x00, 0x73, 0x00, 0x68, 0x00
; ; Starting pCode block for Ival
_string_descriptor:
	DB	LOW(_str0), HIGH(_str0), UPPER(_str0), LOW(_str1), HIGH(_str1), UPPER(_str1), LOW(_str2), HIGH(_str2), UPPER(_str2), LOW(_str3), HIGH(_str3), UPPER(_str3)
	DB	LOW(_str4), HIGH(_str4), UPPER(_str4), LOW(_str5), HIGH(_str5), UPPER(_str5)
; ; Starting pCode block for Ival
_ep_init_cfg0:
	DB	LOW(_ep0_init), HIGH(_ep0_init), UPPER(_ep0_init), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
; ; Starting pCode block for Ival
_ep_init_cfg1:
	DB	LOW(_ep0_init), HIGH(_ep0_init), UPPER(_ep0_init), LOW(_ep1_init), HIGH(_ep1_init), UPPER(_ep1_init), LOW(_ep2_init), HIGH(_ep2_init), UPPER(_ep2_init), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
; ; Starting pCode block for Ival
_ep_init_cfg2:
	DB	LOW(_ep0_init), HIGH(_ep0_init), UPPER(_ep0_init), LOW(_my_ep1_init), HIGH(_my_ep1_init), UPPER(_my_ep1_init), LOW(_my_ep2_init), HIGH(_my_ep2_init), UPPER(_my_ep2_init), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
; ; Starting pCode block for Ival
_ep_init:
	DB	LOW(_ep_init_cfg0), HIGH(_ep_init_cfg0), UPPER(_ep_init_cfg0), LOW(_ep_init_cfg0), HIGH(_ep_init_cfg0), UPPER(_ep_init_cfg0), LOW(_ep_init_cfg1), HIGH(_ep_init_cfg1), UPPER(_ep_init_cfg1), LOW(_ep_init_cfg2), HIGH(_ep_init_cfg2), UPPER(_ep_init_cfg2)
; ; Starting pCode block for Ival
_ep_in_cfg0:
	DB	LOW(_ep0_in), HIGH(_ep0_in), UPPER(_ep0_in), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
; ; Starting pCode block for Ival
_ep_in_cfg1:
	DB	LOW(_ep0_in), HIGH(_ep0_in), UPPER(_ep0_in), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_ep2_in), HIGH(_ep2_in), UPPER(_ep2_in), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
; ; Starting pCode block for Ival
_ep_in_cfg2:
	DB	LOW(_ep0_in), HIGH(_ep0_in), UPPER(_ep0_in), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_my_ep2_in), HIGH(_my_ep2_in), UPPER(_my_ep2_in), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
; ; Starting pCode block for Ival
_ep_in:
	DB	LOW(_ep_in_cfg0), HIGH(_ep_in_cfg0), UPPER(_ep_in_cfg0), LOW(_ep_in_cfg0), HIGH(_ep_in_cfg0), UPPER(_ep_in_cfg0), LOW(_ep_in_cfg1), HIGH(_ep_in_cfg1), UPPER(_ep_in_cfg1), LOW(_ep_in_cfg2), HIGH(_ep_in_cfg2), UPPER(_ep_in_cfg2)
; ; Starting pCode block for Ival
_ep_out_cfg0:
	DB	LOW(_ep0_init), HIGH(_ep0_init), UPPER(_ep0_init), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
; ; Starting pCode block for Ival
_ep_out_cfg1:
	DB	LOW(_ep0_init), HIGH(_ep0_init), UPPER(_ep0_init), LOW(_ep1_out), HIGH(_ep1_out), UPPER(_ep1_out), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
; ; Starting pCode block for Ival
_ep_out_cfg2:
	DB	LOW(_ep0_init), HIGH(_ep0_init), UPPER(_ep0_init), LOW(_my_ep1_out), HIGH(_my_ep1_out), UPPER(_my_ep1_out), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
; ; Starting pCode block for Ival
_ep_out:
	DB	LOW(_ep_out_cfg0), HIGH(_ep_out_cfg0), UPPER(_ep_out_cfg0), LOW(_ep_out_cfg0), HIGH(_ep_out_cfg0), UPPER(_ep_out_cfg0), LOW(_ep_out_cfg1), HIGH(_ep_out_cfg1), UPPER(_ep_out_cfg1), LOW(_ep_out_cfg2), HIGH(_ep_out_cfg2), UPPER(_ep_out_cfg2)
; ; Starting pCode block for Ival
_ep_setup_cfg0:
	DB	LOW(_ep0_setup), HIGH(_ep0_setup), UPPER(_ep0_setup), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
	DB	LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function), LOW(_null_function), HIGH(_null_function), UPPER(_null_function)
; ; Starting pCode block for Ival
_ep_setup:
	DB	LOW(_ep_setup_cfg0), HIGH(_ep_setup_cfg0), UPPER(_ep_setup_cfg0), LOW(_ep_setup_cfg0), HIGH(_ep_setup_cfg0), UPPER(_ep_setup_cfg0), LOW(_ep_setup_cfg0), HIGH(_ep_setup_cfg0), UPPER(_ep_setup_cfg0), LOW(_ep_setup_cfg0), HIGH(_ep_setup_cfg0), UPPER(_ep_setup_cfg0)


; Statistics:
; code size:	  142 (0x008e) bytes ( 0.11%)
;           	   71 (0x0047) words
; udata size:	    0 (0x0000) bytes ( 0.00%)
; access size:	    0 (0x0000) bytes


	end
