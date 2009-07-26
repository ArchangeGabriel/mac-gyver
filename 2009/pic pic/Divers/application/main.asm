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
	extern _ep2_num_bytes_to_send
	extern _ep2_source_data
	extern _an_data
	extern _mydata
	extern _etat_moteur
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
	extern _init_servos
	extern _gestionservos_tmr1
	extern _gestionservos_tmr2
	extern _init_adconversion
	extern _maj_result
	extern _init_moteursetjack
	extern _moteursetjack_ints_tmr0
	extern _moteursetjack_ints_RB_bas
	extern _moteursetjack_ints_RB_haut
	extern _moteursetjack_ints_INT0
	extern _moteursetjack_ints_INT1
	extern _moteursetjack_ints_INT2
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

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; ; Starting pCode block
S_main__high_priority_isr	code	0X002020
_high_priority_isr:
;	.line	62; main.c	void high_priority_isr(void) interrupt
	MOVFF	WREG, POSTDEC1
	MOVFF	STATUS, POSTDEC1
	MOVFF	BSR, POSTDEC1
	MOVFF	PRODL, POSTDEC1
	MOVFF	PRODH, POSTDEC1
	MOVFF	FSR0L, POSTDEC1
	MOVFF	FSR0H, POSTDEC1
	MOVFF	PCLATH, POSTDEC1
	MOVFF	PCLATU, POSTDEC1
;	.line	65; main.c	if(INTCONbits.TMR0IF)
	BTFSS	_INTCONbits, 2
	BRA	_00114_DS_
;	.line	67; main.c	moteursetjack_ints_tmr0();
	CALL	_moteursetjack_ints_tmr0
;	.line	68; main.c	INTCONbits.TMR0IF = 0;
	BCF	_INTCONbits, 2
_00114_DS_:
;	.line	71; main.c	if(INTCONbits.RBIF)   // Fin de course moteur 1
	BTFSS	_INTCONbits, 0
; #	GOTO	_00120_DS_
; #	BTFSS	_PORTBbits, 3
; #	GOTO	_00116_DS_
; #	CALL	_moteursetjack_ints_RB_bas
; #	BTFSS	_PORTBbits, 4
; ;     peep 1 - test/jump to test/skip
;	.line	73; main.c	if(FIN_COURSE1_BAS) moteursetjack_ints_RB_bas();
	BRA	_00120_DS_
;	.line	74; main.c	if(FIN_COURSE1_HAUT) moteursetjack_ints_RB_haut();
	BTFSC	_PORTBbits, 3
; #	CALL	_moteursetjack_ints_RB_bas
; #	BTFSS	_PORTBbits, 4
; #	GOTO	_00118_DS_
; #	CALL	_moteursetjack_ints_RB_haut
; #	BCF	_INTCONbits, 0
; ;     peep 1 - test/jump to test/skip
;	.line	75; main.c	INTCONbits.RBIF = 0;
	CALL	_moteursetjack_ints_RB_bas
	BTFSC	_PORTBbits, 4
	CALL	_moteursetjack_ints_RB_haut
	BCF	_INTCONbits, 0
_00120_DS_:
;	.line	77; main.c	if(INTCONbits.INT0IF) // Fin de course moteur 2-0 / Derriere
	BTFSS	_INTCONbits, 1
	BRA	_00122_DS_
;	.line	79; main.c	moteursetjack_ints_INT0();
	CALL	_moteursetjack_ints_INT0
;	.line	80; main.c	INTCONbits.INT0IF = 0;
	BCF	_INTCONbits, 1
_00122_DS_:
;	.line	82; main.c	if(INTCON3bits.INT1IF) // Fin de course moteur 2-1 / Median
	BTFSS	_INTCON3bits, 0
	BRA	_00124_DS_
;	.line	84; main.c	moteursetjack_ints_INT1();
	CALL	_moteursetjack_ints_INT1
;	.line	85; main.c	INTCON3bits.INT1IF = 0;
	BCF	_INTCON3bits, 0
_00124_DS_:
;	.line	87; main.c	if(INTCON3bits.INT2IF) // Fin de course moteur 2-2 / Devant
	BTFSS	_INTCON3bits, 1
	BRA	_00126_DS_
;	.line	89; main.c	moteursetjack_ints_INT2();
	CALL	_moteursetjack_ints_INT2
;	.line	90; main.c	INTCON3bits.INT2IF = 0;
	BCF	_INTCON3bits, 1
_00126_DS_:
;	.line	93; main.c	if(PIR1bits.TMR1IF)
	BTFSS	_PIR1bits, 0
	BRA	_00128_DS_
;	.line	95; main.c	gestionservos_tmr1();
	CALL	_gestionservos_tmr1
;	.line	96; main.c	PIR1bits.TMR1IF = 0;
	BCF	_PIR1bits, 0
_00128_DS_:
;	.line	98; main.c	if(PIR1bits.TMR2IF)
	BTFSS	_PIR1bits, 1
	BRA	_00130_DS_
;	.line	100; main.c	gestionservos_tmr2();
	CALL	_gestionservos_tmr2
;	.line	101; main.c	PIR1bits.TMR2IF = 0;
	BCF	_PIR1bits, 1
_00130_DS_:
;	.line	104; main.c	if(PIR1bits.ADIF)
	BTFSS	_PIR1bits, 6
	BRA	_00133_DS_
;	.line	106; main.c	maj_result();
	CALL	_maj_result
;	.line	107; main.c	PIR1bits.ADIF = 0;
	BCF	_PIR1bits, 6
_00133_DS_:
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
;	.line	112; main.c	void low_priority_isr(void) interrupt
	MOVFF	WREG, POSTDEC1
	MOVFF	STATUS, POSTDEC1
	MOVFF	BSR, POSTDEC1
	MOVFF	PRODL, POSTDEC1
	MOVFF	PRODH, POSTDEC1
	MOVFF	FSR0L, POSTDEC1
	MOVFF	FSR0H, POSTDEC1
	MOVFF	PCLATH, POSTDEC1
	MOVFF	PCLATU, POSTDEC1
;	.line	114; main.c	}
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
;	.line	36; main.c	TRISA = 0xEF;
	MOVLW	0xef
	MOVWF	_TRISA
;	.line	37; main.c	PORTA = 0x10;
	MOVLW	0x10
	MOVWF	_PORTA
;	.line	40; main.c	TMR0H = 0;
	CLRF	_TMR0H
;	.line	41; main.c	TMR0L = 0;
	CLRF	_TMR0L
;	.line	43; main.c	init_moteursetjack();
	CALL	_init_moteursetjack
;	.line	44; main.c	init_adconversion();
	CALL	_init_adconversion
;	.line	45; main.c	init_servos();
	CALL	_init_servos
;	.line	48; main.c	PIE2bits.USBIE = 0; // Interrupt USB off : on ne touche pas a la gestion de l'USB !!
	BCF	_PIE2bits, 5
;	.line	50; main.c	INTCONbits.PEIE = 1; // Interrupts peripheriques (usb, timer 1...)
	BSF	_INTCONbits, 6
;	.line	51; main.c	INTCONbits.GIE = 1; // Interrupts global
	BSF	_INTCONbits, 7
_00105_DS_:
;	.line	53; main.c	while(usb_active_cfg > 2)
	MOVLW	0x03
	SUBWF	_usb_active_cfg, W
	BNC	_00108_DS_
;	.line	55; main.c	usb_sleep();
	CALL	_usb_sleep
;	.line	56; main.c	dispatch_usb_event();
	CALL	_dispatch_usb_event
	BRA	_00105_DS_
_00108_DS_:
	RETURN	



; Statistics:
; code size:	  284 (0x011c) bytes ( 0.22%)
;           	  142 (0x008e) words
; udata size:	    0 (0x0000) bytes ( 0.00%)
; access size:	    0 (0x0000) bytes


	end
