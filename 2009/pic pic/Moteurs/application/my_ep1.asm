;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.6.1 #4482 (Mar 15 2009)
; This file generated Tue May 19 12:00:28 2009
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f4550

	radix dec

;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global _checksum
	global _mydata
	global _ep2_source_data
	global _ep2_num_bytes_to_send
	global _my_ep1_init
	global _my_ep1_out
	global _ep1_OutBuffer

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
	extern _codeusecnt
	extern _totol
	extern _totoh
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
	extern _my_prepare_ep2_in
	extern _device_descriptor
	extern _configuration_descriptor
	extern _string_descriptor
	extern _str0
	extern _str1
	extern _str2
	extern _str3
	extern _ep_init
	extern _ep_in
	extern _ep_out
	extern _ep_setup
;--------------------------------------------------------
;	Equates to used internal registers
;--------------------------------------------------------
STATUS	equ	0xfd8
FSR1L	equ	0xfe1
FSR2L	equ	0xfd9
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4

udata_my_ep1_0	udata
_ep2_num_bytes_to_send	res	2

udata_my_ep1_1	udata
_ep2_source_data	res	3

udata_my_ep1_2	udata
_mydata	res	8

udata_my_ep1_3	udata
_checksum	res	1

udata_my_ep1_4	udata
iTemp0	res	1


ustat_my_ep1_00	udata	0X0500
_ep1_OutBuffer	res	64

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; I code from now on!
; ; Starting pCode block
S_my_ep1__my_ep1_out	code
_my_ep1_out:
;	.line	36; my_ep1.c	if(EP_OUT_BD(1).Cnt >= 1)
	MOVFF	(_ep_bdt + 9), (iTemp0 + 1)
	MOVLW	0x01
	BANKSEL	(iTemp0 + 1)
	SUBWF	(iTemp0 + 1), W, B
	BNC	_00117_DS_
; #	MOVF	_ep1_OutBuffer, W, B
; #	BTFSC	STATUS, 2
; #	GOTO	_00110_DS_
; #	MOVFF	_ep1_OutBuffer, _CCPR1L
; #	MOVF	(_ep1_OutBuffer + 1), W, B
; ;     peep 1 - test/jump to test/skip
	BANKSEL	_ep1_OutBuffer
;	.line	38; my_ep1.c	if(ep1_OutBuffer[0]) CCPR1L = ep1_OutBuffer[0];
	MOVF	_ep1_OutBuffer, W, B
;	.line	39; my_ep1.c	if(ep1_OutBuffer[1]) CCPR2L = ep1_OutBuffer[1];
	BTFSS	STATUS, 2
	MOVFF	_ep1_OutBuffer, _CCPR1L
; #	MOVF	(_ep1_OutBuffer + 1), W, B
; #	BTFSC	STATUS, 2
; #	GOTO	_00112_DS_
; #	MOVFF	(_ep1_OutBuffer + 1), _CCPR2L
; #	MOVLW	0x08
; ;     peep 1 - test/jump to test/skip
; removed redundant BANKSEL
;	.line	41; my_ep1.c	ep2_num_bytes_to_send = 8;
	MOVF	(_ep1_OutBuffer + 1), W, B
	BTFSS	STATUS, 2
	MOVFF	(_ep1_OutBuffer + 1), _CCPR2L
	MOVLW	0x08
	BANKSEL	_ep2_num_bytes_to_send
	MOVWF	_ep2_num_bytes_to_send, B
; removed redundant BANKSEL
	CLRF	(_ep2_num_bytes_to_send + 1), B
;	.line	42; my_ep1.c	ep2_source_data = (unsigned char*) codeusecnt;
	MOVLW	HIGH(_codeusecnt)
	BANKSEL	(_ep2_source_data + 1)
	MOVWF	(_ep2_source_data + 1), B
	MOVLW	LOW(_codeusecnt)
; removed redundant BANKSEL
	MOVWF	_ep2_source_data, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_ep2_source_data + 2), B
;	.line	44; my_ep1.c	my_prepare_ep2_in(); // Envoi
	CALL	_my_prepare_ep2_in
;	.line	46; my_ep1.c	EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 9)
	MOVWF	(_ep_bdt + 9), B
; removed redundant BANKSEL
;	.line	47; my_ep1.c	if(EP_OUT_BD(1).Stat.DTS == 0)
	BTFSC	(_ep_bdt + 8), 6, B
	BRA	_00114_DS_
;	.line	49; my_ep1.c	EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
	MOVLW	0xc8
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 8), B
	BRA	_00119_DS_
_00114_DS_:
;	.line	53; my_ep1.c	EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
	MOVLW	0x88
	BANKSEL	(_ep_bdt + 8)
	MOVWF	(_ep_bdt + 8), B
	BRA	_00119_DS_
_00117_DS_:
;	.line	58; my_ep1.c	EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 9)
	MOVWF	(_ep_bdt + 9), B
; removed redundant BANKSEL
;	.line	59; my_ep1.c	EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
	CLRF	(_ep_bdt + 10), B
	MOVLW	0x05
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 11), B
;	.line	60; my_ep1.c	EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_BSTALL;
	MOVLW	0x84
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 8), B
_00119_DS_:
	RETURN	

; ; Starting pCode block
S_my_ep1__my_ep1_init	code
_my_ep1_init:
;	.line	28; my_ep1.c	EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 9)
	MOVWF	(_ep_bdt + 9), B
; removed redundant BANKSEL
;	.line	29; my_ep1.c	EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
	CLRF	(_ep_bdt + 10), B
	MOVLW	0x05
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 11), B
;	.line	30; my_ep1.c	EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
	MOVLW	0x88
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 8), B
;	.line	31; my_ep1.c	UEP1 = EPHSHK_EN | EPOUTEN_EN | EPCONDIS_EN;       // Init EP1 as an OUT EP
	MOVLW	0x1c
	MOVWF	_UEP1
	RETURN	



; Statistics:
; code size:	  120 (0x0078) bytes ( 0.09%)
;           	   60 (0x003c) words
; udata size:	   79 (0x004f) bytes ( 4.41%)
; access size:	    0 (0x0000) bytes


	end
