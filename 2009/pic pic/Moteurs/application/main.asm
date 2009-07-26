;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.6.1 #4482 (Mar 15 2009)
; This file generated Mon May 18 09:33:37 2009
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f4550

	radix dec

;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global _totol
	global _totoh
	global _init_pwm
	global _application_main
	global _high_priority_isr
	global _low_priority_isr

;--------------------------------------------------------
; extern variables in this module
;--------------------------------------------------------
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
	extern _dispatch_usb_event
	extern _usb_sleep
	extern _init_codeuses
;--------------------------------------------------------
;	Equates to used internal registers
;--------------------------------------------------------
STATUS	equ	0xfd8
PCLATH	equ	0xffa
PCLATU	equ	0xffb
WREG	equ	0xfe8
BSR	equ	0xfe0
FSR0L	equ	0xfe9
FSR0H	equ	0xfea
FSR1L	equ	0xfe1
FSR2L	equ	0xfd9
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4
PRODL	equ	0xff3
PRODH	equ	0xff4


; Internal registers
.registers	udata_ovr	0x0000
r0x00	res	1
r0x01	res	1
r0x02	res	1
r0x03	res	1

udata_main_0	udata
_totol	res	1

udata_main_1	udata
_totoh	res	1

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; ; Starting pCode block
S_main__high_priority_isr	code	0X002020
_high_priority_isr:
;	.line	83; main.c	void high_priority_isr(void) interrupt
	MOVFF	WREG, POSTDEC1
	MOVFF	STATUS, POSTDEC1
	MOVFF	BSR, POSTDEC1
	MOVFF	PRODL, POSTDEC1
	MOVFF	PRODH, POSTDEC1
	MOVFF	FSR0L, POSTDEC1
	MOVFF	FSR0H, POSTDEC1
	MOVFF	PCLATH, POSTDEC1
	MOVFF	PCLATU, POSTDEC1
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
;	.line	85; main.c	if(INTCONbits.TMR0IF)
	BTFSS	_INTCONbits, 2
	BRA	_00118_DS_
;	.line	87; main.c	ftoggle_A4();
	CLRF	r0x00
	BTFSC	_PORTAbits, 4
	INCF	r0x00, F
	MOVF	r0x00, W
	BSF	STATUS, 0
	TSTFSZ	WREG
	BCF	STATUS, 0
	CLRF	r0x00
	RLCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	SWAPF	WREG, W
	MOVWF	PRODH
	MOVF	_PORTAbits, W
	ANDLW	0xef
	IORWF	PRODH, W
	MOVWF	_PORTAbits
;	.line	88; main.c	TMR0H=totoh;
	MOVFF	_totoh, _TMR0H
;	.line	89; main.c	TMR0L=totol;
	MOVFF	_totol, _TMR0L
;	.line	90; main.c	INTCONbits.TMR0IF = 0;
	BCF	_INTCONbits, 2
_00118_DS_:
;	.line	93; main.c	if(INTCON3bits.INT1IF)
	BTFSS	_INTCON3bits, 0
	BRA	_00123_DS_
;	.line	95; main.c	if(PIN_SSCOD1) codeusecnt[1]++; else codeusecnt[1]--;
	BTFSS	_PORTBbits, 3
	BRA	_00120_DS_
	MOVFF	(_codeusecnt + 4), r0x00
	MOVFF	(_codeusecnt + 5), r0x01
	MOVFF	(_codeusecnt + 6), r0x02
	MOVFF	(_codeusecnt + 7), r0x03
	INCF	r0x00, F
	BTFSC	STATUS, 0
	INCF	r0x01, F
	BTFSC	STATUS, 0
	INCF	r0x02, F
	BTFSC	STATUS, 0
	INCF	r0x03, F
	MOVF	r0x00, W
	BANKSEL	(_codeusecnt + 4)
	MOVWF	(_codeusecnt + 4), B
	MOVF	r0x01, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 5), B
	MOVF	r0x02, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 6), B
	MOVF	r0x03, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 7), B
	BRA	_00121_DS_
_00120_DS_:
	MOVFF	(_codeusecnt + 4), r0x00
	MOVFF	(_codeusecnt + 5), r0x01
	MOVFF	(_codeusecnt + 6), r0x02
	MOVFF	(_codeusecnt + 7), r0x03
	MOVLW	0xff
	ADDWF	r0x00, F
	ADDWFC	r0x01, F
	ADDWFC	r0x02, F
	ADDWFC	r0x03, F
	MOVF	r0x00, W
	BANKSEL	(_codeusecnt + 4)
	MOVWF	(_codeusecnt + 4), B
	MOVF	r0x01, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 5), B
	MOVF	r0x02, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 6), B
	MOVF	r0x03, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 7), B
_00121_DS_:
;	.line	96; main.c	INTCON3bits.INT1IF = 0;
	BCF	_INTCON3bits, 0
_00123_DS_:
;	.line	98; main.c	if(INTCONbits.INT0IF)
	BTFSS	_INTCONbits, 1
	BRA	_00129_DS_
;	.line	100; main.c	if(PIN_SSCOD0) codeusecnt[0]++; else codeusecnt[0]--;
	BTFSS	_PORTBbits, 2
	BRA	_00125_DS_
	MOVFF	_codeusecnt, r0x00
	MOVFF	(_codeusecnt + 1), r0x01
	MOVFF	(_codeusecnt + 2), r0x02
	MOVFF	(_codeusecnt + 3), r0x03
	INCF	r0x00, F
	BTFSC	STATUS, 0
	INCF	r0x01, F
	BTFSC	STATUS, 0
	INCF	r0x02, F
	BTFSC	STATUS, 0
	INCF	r0x03, F
	MOVF	r0x00, W
	BANKSEL	_codeusecnt
	MOVWF	_codeusecnt, B
	MOVF	r0x01, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 1), B
	MOVF	r0x02, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 2), B
	MOVF	r0x03, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 3), B
	BRA	_00126_DS_
_00125_DS_:
	MOVFF	_codeusecnt, r0x00
	MOVFF	(_codeusecnt + 1), r0x01
	MOVFF	(_codeusecnt + 2), r0x02
	MOVFF	(_codeusecnt + 3), r0x03
	MOVLW	0xff
	ADDWF	r0x00, F
	ADDWFC	r0x01, F
	ADDWFC	r0x02, F
	ADDWFC	r0x03, F
	MOVF	r0x00, W
	BANKSEL	_codeusecnt
	MOVWF	_codeusecnt, B
	MOVF	r0x01, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 1), B
	MOVF	r0x02, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 2), B
	MOVF	r0x03, W
; removed redundant BANKSEL
	MOVWF	(_codeusecnt + 3), B
_00126_DS_:
;	.line	101; main.c	INTCONbits.INT0IF = 0;
	BCF	_INTCONbits, 1
_00129_DS_:
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, PCLATU
	MOVFF	PREINC1, PCLATH
	MOVFF	PREINC1, FSR0H
	MOVFF	PREINC1, FSR0L
	MOVFF	PREINC1, PRODH
	MOVFF	PREINC1, PRODL
	MOVFF	PREINC1, BSR
	MOVFF	PREINC1, STATUS
	MOVFF	PREINC1, WREG
	RETFIE	

; ; Starting pCode block
S_main__low_priority_isr	code	0X004000
_low_priority_isr:
;	.line	106; main.c	void low_priority_isr(void) interrupt
	MOVFF	WREG, POSTDEC1
	MOVFF	STATUS, POSTDEC1
	MOVFF	BSR, POSTDEC1
	MOVFF	PRODL, POSTDEC1
	MOVFF	PRODH, POSTDEC1
	MOVFF	FSR0L, POSTDEC1
	MOVFF	FSR0H, POSTDEC1
	MOVFF	PCLATH, POSTDEC1
	MOVFF	PCLATU, POSTDEC1
;	.line	108; main.c	}
	MOVFF	PREINC1, PCLATU
	MOVFF	PREINC1, PCLATH
	MOVFF	PREINC1, FSR0H
	MOVFF	PREINC1, FSR0L
	MOVFF	PREINC1, PRODH
	MOVFF	PREINC1, PRODL
	MOVFF	PREINC1, BSR
	MOVFF	PREINC1, STATUS
	MOVFF	PREINC1, WREG
	RETFIE	

; I code from now on!
; ; Starting pCode block
S_main__application_main	code
_application_main:
;	.line	49; main.c	TRISA = 0xEF;
	MOVLW	0xef
	MOVWF	_TRISA
;	.line	50; main.c	PORTA = 0x10;
	MOVLW	0x10
	MOVWF	_PORTA
;	.line	53; main.c	TMR0H = 0;
	CLRF	_TMR0H
;	.line	54; main.c	TMR0L = 0;
	CLRF	_TMR0L
;	.line	57; main.c	T0CON = 0x86; // TMR0ON, 16bits, CLKO, PSA on, 1:256
	MOVLW	0x86
	MOVWF	_T0CON
;	.line	59; main.c	init_pwm();
	CALL	_init_pwm
;	.line	60; main.c	init_codeuses();
	CALL	_init_codeuses
;	.line	64; main.c	PIE2bits.USBIE = 0; // Interrupt USB off : on ne touche pas a la gestion de l'USB !!
	BCF	_PIE2bits, 5
;	.line	66; main.c	INTCONbits.TMR0IE = 1; // Interrupt Timer 0
	BSF	_INTCONbits, 5
;	.line	67; main.c	INTCONbits.PEIE = 1; // Interrupts peripheriques (usb, timer 1...)
	BSF	_INTCONbits, 6
;	.line	68; main.c	INTCONbits.GIE = 1; // Interrupts global
	BSF	_INTCONbits, 7
; #	MOVLW	0x80
; #	MOVWF	_totol, B
; #	MOVLW	0x80
; ;     peep 5 - Removed redundant move
;	.line	70; main.c	totol=0x80;
	MOVLW	0x80
	BANKSEL	_totol
;	.line	71; main.c	totoh=0x80;
	MOVWF	_totol, B
	BANKSEL	_totoh
	MOVWF	_totoh, B
_00109_DS_:
;	.line	73; main.c	while(usb_active_cfg > 2)
	MOVLW	0x03
	SUBWF	_usb_active_cfg, W
	BNC	_00112_DS_
;	.line	75; main.c	usb_sleep();
	CALL	_usb_sleep
;	.line	76; main.c	dispatch_usb_event();
	CALL	_dispatch_usb_event
	BRA	_00109_DS_
_00112_DS_:
	RETURN	

; ; Starting pCode block
S_main__init_pwm	code
_init_pwm:
;	.line	38; main.c	PR2 = 0xFF; //periode Timer2 = 256
	SETF	_PR2
; #	MOVLW	0x80
; #	MOVWF	_CCPR1L
; #	MOVLW	0x80
; ;     peep 5 - Removed redundant move
;	.line	39; main.c	CCPR1L = 0x80; // Rapport cyclique 1 = 1/2 (pour les bits de poids faibles, voir CCP1CON bits 4 et 5)
	MOVLW	0x80
;	.line	40; main.c	CCPR2L = 0x80; // Rapport cyclique 2 = 1/2
	MOVWF	_CCPR1L
	MOVWF	_CCPR2L
;	.line	41; main.c	TRISC &= 0xF9; // 11111001 : CCP1, CCP2 = sorties
	MOVLW	0xf9
	ANDWF	_TRISC, F
;	.line	42; main.c	T2CON = 0x7C; // timer2 conf = 0 1111 1 00 : nU, postscaler 1:16, on, prescaler 1:1
	MOVLW	0x7c
	MOVWF	_T2CON
; #	MOVLW	0x0c
; #	MOVWF	_CCP1CON
; #	MOVLW	0x0c
; ;     peep 5 - Removed redundant move
;	.line	43; main.c	CCP1CON = 0x0C; // CCP1 conf = 00 00 1100 : nU, bits poids faibles, PWM mode
	MOVLW	0x0c
;	.line	44; main.c	CCP2CON = 0x0C; // CCP2 conf...
	MOVWF	_CCP1CON
	MOVWF	_CCP2CON
	RETURN	



; Statistics:
; code size:	  520 (0x0208) bytes ( 0.40%)
;           	  260 (0x0104) words
; udata size:	    2 (0x0002) bytes ( 0.11%)
; access size:	    4 (0x0004) bytes


	end
