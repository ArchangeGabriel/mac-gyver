;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.6.1 #4482 (Mar 15 2009)
; This file generated Tue May 19 21:44:04 2009
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f4550

	radix dec

;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global _init_adconversion
	global _maj_result
	global _an
	global _compteur_an

;--------------------------------------------------------
; extern variables in this module
;--------------------------------------------------------
	extern __gptrput1
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
;--------------------------------------------------------
;	Equates to used internal registers
;--------------------------------------------------------
STATUS	equ	0xfd8
WREG	equ	0xfe8
FSR0L	equ	0xfe9
FSR1L	equ	0xfe1
FSR2L	equ	0xfd9
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4
PRODL	equ	0xff3


; Internal registers
.registers	udata_ovr	0x0000
r0x00	res	1
r0x01	res	1
r0x02	res	1

udata_analogs_0	udata
_compteur_an	res	1

udata_analogs_1	udata
_an	res	3

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; I code from now on!
; ; Starting pCode block
S_analogs__maj_result	code
_maj_result:
;	.line	27; analogs.c	void maj_result(void)
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	BANKSEL	_compteur_an
;	.line	29; analogs.c	an[compteur_an] = ADRESH;
	MOVF	_compteur_an, W, B
	BANKSEL	_an
	ADDWF	_an, W, B
	MOVWF	r0x00
	CLRF	WREG
; removed redundant BANKSEL
	ADDWFC	(_an + 1), W, B
	MOVWF	r0x01
	CLRF	WREG
; removed redundant BANKSEL
	ADDWFC	(_an + 2), W, B
	MOVFF	_ADRESH, POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	CALL	__gptrput1
;	.line	30; analogs.c	an[compteur_an + 1] = ADRESL;
	MOVFF	_compteur_an, r0x00
	CLRF	r0x01
	INCF	r0x00, F
	BTFSC	STATUS, 0
	INCF	r0x01, F
	BANKSEL	_an
	MOVF	_an, W, B
	ADDWF	r0x00, F
; removed redundant BANKSEL
	MOVF	(_an + 1), W, B
	ADDWFC	r0x01, F
; removed redundant BANKSEL
	MOVF	(_an + 2), W, B
	CLRF	r0x02
	BTFSC	r0x01, 7
	SETF	r0x02
	ADDWFC	r0x02, F
	MOVFF	_ADRESL, POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
	BANKSEL	_compteur_an
;	.line	31; analogs.c	compteur_an += 2;
	INCF	_compteur_an, F, B
	INCF	_compteur_an, F, B
;	.line	32; analogs.c	compteur_an %= 2 * NB_ANALOGS;
	MOVLW	0x07
; removed redundant BANKSEL
	ANDWF	_compteur_an, F, B
;	.line	33; analogs.c	ADCON0 = (compteur_an << 1) + 1;
	MOVF	_compteur_an, W, B
	ADDWF	_compteur_an, W, B
	MOVWF	r0x00
	INCF	r0x00, W
	MOVWF	_ADCON0
;	.line	34; analogs.c	ADCON0bits.GO = 1;
	BSF	_ADCON0bits, 1
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_analogs__init_adconversion	code
_init_adconversion:
;	.line	13; analogs.c	OSCCON |= 0x80; // 10000000 : IDLEN = 1 : pas de SLEEP (pour AD conversion)
	BSF	_OSCCON, 7
;	.line	14; analogs.c	OSCCON &= 0xfc; // 11111100 : SCS = 0 : primary clock source (on ne sait jamais !)
	MOVLW	0xfc
	ANDWF	_OSCCON, F
;	.line	15; analogs.c	TRISA |= TRISA_CONF; // bons ports en input
	MOVLW	0xcf
	IORWF	_TRISA, F
;	.line	17; analogs.c	ADCON1 = NB_DIG; // 00 0 0 xxxx : nU, Vref- = VSS, Vref+ = VDD, nb d'analogs
	MOVLW	0x0c
	MOVWF	_ADCON1
;	.line	18; analogs.c	ADCON0 = 0x00; // 00 0000 0 0 : nU, ANinput = AN0, idle, ADOFF
	CLRF	_ADCON0
;	.line	19; analogs.c	ADCON2 = 0x2E; // 0 0 101 110 : left justified, nU, 12 Tac d'acquisition, freq de Fosc/64
	MOVLW	0x2e
	MOVWF	_ADCON2
;	.line	20; analogs.c	ADCON0 |= 0x01; // xx xxxx x 1 : ADON
	BSF	_ADCON0, 0
	BANKSEL	_compteur_an
;	.line	21; analogs.c	compteur_an = 0;
	CLRF	_compteur_an, B
;	.line	22; analogs.c	PIE1bits.ADIE = 1;
	BSF	_PIE1bits, 6
;	.line	23; analogs.c	an = &(an_data[1]);
	MOVLW	HIGH(_an_data + 1)
	BANKSEL	(_an + 1)
	MOVWF	(_an + 1), B
	MOVLW	LOW(_an_data + 1)
; removed redundant BANKSEL
	MOVWF	_an, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_an + 2), B
;	.line	24; analogs.c	ADCON0bits.GO = 1; //commence la conversion
	BSF	_ADCON0bits, 1
	RETURN	



; Statistics:
; code size:	  180 (0x00b4) bytes ( 0.14%)
;           	   90 (0x005a) words
; udata size:	    4 (0x0004) bytes ( 0.22%)
; access size:	    3 (0x0003) bytes


	end
