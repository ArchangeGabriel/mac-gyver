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
	global _svo_order
	global _next_event_svo
	global _init_servos
	global _set_servo
	global _gestionservos_tmr2
	global _gestionservos_tmr1
	global _positions
	global _change_svocmd
	global _event_count
	global _svo_event

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
FSR0H	equ	0xfea
FSR1L	equ	0xfe1
FSR2L	equ	0xfd9
INDF0	equ	0xfef
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4
PLUSW2	equ	0xfdb


; Internal registers
.registers	udata_ovr	0x0000
r0x00	res	1
r0x01	res	1
r0x02	res	1

udata_servos_0	udata
_next_event_svo	res	1

udata_servos_1	udata
_svo_order	res	1

udata_servos_2	udata
_change_svocmd	res	1

udata_servos_3	udata
_positions	res	1

udata_servos_4	udata
_event_count	res	1

udata_servos_5	udata
_svo_event	res	1

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; I code from now on!
; ; Starting pCode block
S_servos__gestionservos_tmr1	code
_gestionservos_tmr1:
	BANKSEL	_change_svocmd
;	.line	51; servos.c	if(change_svocmd) // si Flag = 1 (ie si on a ordonne un changement de position)
	MOVF	_change_svocmd, W, B
	BZ	_00124_DS_
	BANKSEL	_positions
;	.line	53; servos.c	svo_event[0] = positions[0];
	MOVF	_positions, W, B
	BANKSEL	_svo_event
	MOVWF	_svo_event, B
	BANKSEL	_change_svocmd
;	.line	54; servos.c	change_svocmd = 0; // Flag a 0
	CLRF	_change_svocmd, B
_00124_DS_:
	BANKSEL	_event_count
;	.line	56; servos.c	event_count=0;
	CLRF	_event_count, B
;	.line	57; servos.c	PIN_SVO0=1; // Mise a 1 des sorties
	BSF	_PORTDbits, 7
;	.line	59; servos.c	PR2 = svo_event[0];
	MOVFF	_svo_event, _PR2
;	.line	60; servos.c	TMR2 = 0;
	CLRF	_TMR2
	RETURN	

; ; Starting pCode block
S_servos__gestionservos_tmr2	code
_gestionservos_tmr2:
;	.line	36; servos.c	void gestionservos_tmr2(void) // 1 avant
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
;	.line	38; servos.c	PIN_SVO0 = 0;
	BCF	_PORTDbits, 7
	BANKSEL	_event_count
;	.line	40; servos.c	event_count++;  // On passe au prochain evenement
	INCF	_event_count, F, B
;	.line	42; servos.c	if(event_count<NB_SERVO)
	MOVLW	0x01
; removed redundant BANKSEL
	SUBWF	_event_count, W, B
	BC	_00118_DS_
;	.line	44; servos.c	PR2 = svo_event[event_count];
	MOVLW	LOW(_svo_event)
; removed redundant BANKSEL
	ADDWF	_event_count, W, B
	MOVWF	r0x00
	CLRF	r0x01
	MOVLW	HIGH(_svo_event)
	ADDWFC	r0x01, F
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, FSR0H
	MOVFF	INDF0, _PR2
;	.line	45; servos.c	TMR2 = 0;
	CLRF	_TMR2
_00118_DS_:
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_servos__set_servo	code
_set_servo:
;	.line	25; servos.c	unsigned char set_servo(unsigned char servo_id, unsigned /*int*/ char position)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
;	.line	27; servos.c	if(servo_id<NB_SERVO)
	MOVLW	0x01
	SUBWF	r0x00, W
	BC	_00110_DS_
;	.line	29; servos.c	positions[servo_id]=position; // changer position
	MOVLW	LOW(_positions)
	ADDWF	r0x00, F
	MOVLW	HIGH(_positions)
	CLRF	r0x02
	ADDWFC	r0x02, F
	MOVFF	r0x00, FSR0L
	MOVFF	r0x02, FSR0H
	MOVFF	r0x01, INDF0
; #	MOVLW	0x01
; #	MOVWF	_change_svocmd, B
; #	MOVLW	0x01
; ;     peep 5 - Removed redundant move
;	.line	30; servos.c	change_svocmd = 1; // mise du Flag a 1
	MOVLW	0x01
	BANKSEL	_change_svocmd
;	.line	31; servos.c	return 1;
	MOVWF	_change_svocmd, B
	BRA	_00111_DS_
_00110_DS_:
;	.line	33; servos.c	return 0;
	CLRF	WREG
_00111_DS_:
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_servos__init_servos	code
_init_servos:
;	.line	14; servos.c	PR2 = 0xFF; //periode overflow Timer2 = 256
	SETF	_PR2
;	.line	15; servos.c	T2CON = 0x3F; // timer2 conf = 0 0111 1 11 : nU, postscaler 1:8, on, prescaler 1:16
	MOVLW	0x3f
	MOVWF	_T2CON
;	.line	16; servos.c	T1CON = 0xA1; // timer1 conf = 1 0 10 0 0 0 1 : 16bits, vers proc = off, prescaler 1:4, oscillator off, Extclk input nU, internal clock, on
	MOVLW	0xa1
	MOVWF	_T1CON
;	.line	17; servos.c	PIE1bits.TMR1IE = 1; //Interrupt Timer1
	BSF	_PIE1bits, 0
;	.line	18; servos.c	PIE1bits.TMR2IE = 1; //Interrupt Timer2
	BSF	_PIE1bits, 1
;	.line	19; servos.c	positions[0]=POS_DEFAULT0;                // Positions par defaut
	MOVLW	0x40
	BANKSEL	_positions
	MOVWF	_positions, B
;	.line	20; servos.c	TRIS_SVO0 = 0;
	BCF	_TRISDbits, 7
;	.line	21; servos.c	change_svocmd = 1; // Flag de changement de position a 1
	MOVLW	0x01
	BANKSEL	_change_svocmd
	MOVWF	_change_svocmd, B
;	.line	22; servos.c	TMR2 = 0;
	CLRF	_TMR2
	RETURN	



; Statistics:
; code size:	  208 (0x00d0) bytes ( 0.16%)
;           	  104 (0x0068) words
; udata size:	    6 (0x0006) bytes ( 0.33%)
; access size:	    3 (0x0003) bytes


	end
