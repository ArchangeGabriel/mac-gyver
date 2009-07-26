;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.6.1 #4482 (Mar 15 2009)
; This file generated Wed May 20 17:01:05 2009
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f4550

	radix dec

;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global _init_moteursetjack
	global _ordre_moteur
	global _moteursetjack_ints_RB_bas
	global _moteursetjack_ints_RB_haut
	global _moteursetjack_ints_INT0
	global _moteursetjack_ints_INT1
	global _moteursetjack_ints_INT2
	global _moteursetjack_ints_tmr0
	global _depart
	global _ticoutac
	global _mot1_phase
	global _mot1_sens
	global _etat_moteur
	global _mot1_position
	global _mot2_position
	global _mot1_pos_visee
	global _mot2_pos_visee
	global _compteur_mot

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
FSR1L	equ	0xfe1
FSR2L	equ	0xfd9
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4
PLUSW2	equ	0xfdb
PRODH	equ	0xff4


	idata
_depart	db	0x00
_ticoutac	db	0x00
_mot1_sens	db	0x00
_mot1_position	db	0x00
_mot2_position	db	0x00
_mot1_pos_visee	db	0x00
_mot2_pos_visee	db	0x00
_compteur_mot	db	0x00, 0x00


; Internal registers
.registers	udata_ovr	0x0000
r0x00	res	1
r0x01	res	1
r0x02	res	1
r0x03	res	1

udata_moteursetjack_0	udata
_etat_moteur	res	2

udata_moteursetjack_1	udata
_mot1_phase	res	1

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; I code from now on!
; ; Starting pCode block
S_moteursetjack__moteursetjack_ints_tmr0	code
_moteursetjack_ints_tmr0:
;	.line	192; moteursetjack.c	void moteursetjack_ints_tmr0(void)
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	BANKSEL	_depart
;	.line	194; moteursetjack.c	if((!depart)&&(etat_moteur[0] & ACTIV_MOTEUR_1))
	MOVF	_depart, W, B
	BTFSS	STATUS, 2
	BRA	_00240_DS_
	MOVFF	_etat_moteur, r0x00
	BTFSS	r0x00, 7
	BRA	_00240_DS_
	BANKSEL	_ticoutac
;	.line	196; moteursetjack.c	if(ticoutac)
	MOVF	_ticoutac, W, B
	BZ	_00224_DS_
;	.line	198; moteursetjack.c	PIN_MOT1_1 = !PIN_MOT1_1;
	CLRF	r0x01
	BTFSC	_PORTDbits, 2
	INCF	r0x01, F
	MOVF	r0x01, W
	BSF	STATUS, 0
	TSTFSZ	WREG
	BCF	STATUS, 0
	CLRF	r0x01
	RLCF	r0x01, F
	MOVF	r0x01, W
	ANDLW	0x01
	RLNCF	WREG, W
	RLNCF	WREG, W
	MOVWF	PRODH
	MOVF	_PORTDbits, W
	ANDLW	0xfb
	IORWF	PRODH, W
	MOVWF	_PORTDbits
;	.line	199; moteursetjack.c	PIN_MOT1_2 = !PIN_MOT1_2;
	CLRF	r0x01
	BTFSC	_PORTDbits, 3
	INCF	r0x01, F
	MOVF	r0x01, W
	BSF	STATUS, 0
	TSTFSZ	WREG
	BCF	STATUS, 0
	CLRF	r0x01
	RLCF	r0x01, F
	MOVF	r0x01, W
	ANDLW	0x01
	SWAPF	WREG, W
	RRNCF	WREG, W
	MOVWF	PRODH
	MOVF	_PORTDbits, W
	ANDLW	0xf7
	IORWF	PRODH, W
	MOVWF	_PORTDbits
; removed redundant BANKSEL
;	.line	200; moteursetjack.c	ticoutac = 0;
	CLRF	_ticoutac, B
	BRA	_00225_DS_
_00224_DS_:
;	.line	204; moteursetjack.c	PIN_MOT1_3 = !PIN_MOT1_3;
	CLRF	r0x01
	BTFSC	_PORTCbits, 6
	INCF	r0x01, F
	MOVF	r0x01, W
	BSF	STATUS, 0
	TSTFSZ	WREG
	BCF	STATUS, 0
	CLRF	r0x01
	RLCF	r0x01, F
	MOVF	r0x01, W
	ANDLW	0x01
	RRNCF	WREG, W
	RRNCF	WREG, W
	MOVWF	PRODH
	MOVF	_PORTCbits, W
	ANDLW	0xbf
	IORWF	PRODH, W
	MOVWF	_PORTCbits
;	.line	205; moteursetjack.c	PIN_MOT1_4 = !PIN_MOT1_4;
	CLRF	r0x01
	BTFSC	_PORTCbits, 7
	INCF	r0x01, F
	MOVF	r0x01, W
	BSF	STATUS, 0
	TSTFSZ	WREG
	BCF	STATUS, 0
	CLRF	r0x01
	RLCF	r0x01, F
	MOVF	r0x01, W
	ANDLW	0x01
	RRNCF	WREG, W
	MOVWF	PRODH
	MOVF	_PORTCbits, W
	ANDLW	0x7f
	IORWF	PRODH, W
	MOVWF	_PORTCbits
;	.line	206; moteursetjack.c	ticoutac = 1;
	MOVLW	0x01
	BANKSEL	_ticoutac
	MOVWF	_ticoutac, B
_00225_DS_:
	BANKSEL	_compteur_mot
;	.line	208; moteursetjack.c	compteur_mot++;
	INCF	_compteur_mot, F, B
	BNC	_10223_DS_
; removed redundant BANKSEL
	INCF	(_compteur_mot + 1), F, B
_10223_DS_:
;	.line	209; moteursetjack.c	if(compteur_mot >= PAS_PAR_ETAGE)
	MOVLW	0x00
	BANKSEL	(_compteur_mot + 1)
	SUBWF	(_compteur_mot + 1), W, B
	BNZ	_00253_DS_
	MOVLW	0x32
; removed redundant BANKSEL
	SUBWF	_compteur_mot, W, B
_00253_DS_:
	BTFSS	STATUS, 0
	BRA	_00243_DS_
	BANKSEL	_mot1_sens
;	.line	211; moteursetjack.c	if(mot1_sens) mot1_position++;
	MOVF	_mot1_sens, W, B
	BZ	_00227_DS_
	BANKSEL	_mot1_position
	INCF	_mot1_position, F, B
	BRA	_00228_DS_
_00227_DS_:
	BANKSEL	_mot1_position
;	.line	212; moteursetjack.c	else mot1_position--;
	DECF	_mot1_position, F, B
_00228_DS_:
;	.line	214; moteursetjack.c	etat_moteur[0] &= ~POSITION_MOTEUR_1;                // Maj position
	MOVLW	0xc0
	ANDWF	r0x00, F
	MOVF	r0x00, W
	BANKSEL	_etat_moteur
	MOVWF	_etat_moteur, B
;	.line	215; moteursetjack.c	etat_moteur[0] |= mot1_position & POSITION_MOTEUR_1; // ...
	MOVLW	0x3f
	BANKSEL	_mot1_position
	ANDWF	_mot1_position, W, B
	MOVWF	r0x00
	BANKSEL	_etat_moteur
	MOVF	_etat_moteur, W, B
	IORWF	r0x00, F
	MOVF	r0x00, W
; removed redundant BANKSEL
	MOVWF	_etat_moteur, B
	BANKSEL	_mot1_position
;	.line	217; moteursetjack.c	if(mot1_position == mot1_pos_visee)
	MOVF	_mot1_position, W, B
	BANKSEL	_mot1_pos_visee
	XORWF	_mot1_pos_visee, W, B
	BNZ	_00230_DS_
;	.line	219; moteursetjack.c	T0CON = CONFIG_TMR0_MOTEUR & 0x7f; // TMR0OFF
	MOVLW	0x06
	MOVWF	_T0CON
;	.line	220; moteursetjack.c	etat_moteur[0] &= ~ACTIV_MOTEUR_1; // Inactif
	MOVLW	0x7f
	BANKSEL	_etat_moteur
	ANDWF	_etat_moteur, W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	_etat_moteur, B
;	.line	222; moteursetjack.c	mot1_phase.b0 = PIN_MOT1_1;
	CLRF	r0x00
	BTFSC	_PORTDbits, 2
	INCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	MOVWF	PRODH
	BANKSEL	_mot1_phase
	MOVF	_mot1_phase, W, B
	ANDLW	0xfe
	IORWF	PRODH, W
; removed redundant BANKSEL
	MOVWF	_mot1_phase, B
;	.line	223; moteursetjack.c	mot1_phase.b1 = PIN_MOT1_3;
	CLRF	r0x00
	BTFSC	_PORTCbits, 6
	INCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	RLNCF	WREG, W
	MOVWF	PRODH
; removed redundant BANKSEL
	MOVF	_mot1_phase, W, B
	ANDLW	0xfd
	IORWF	PRODH, W
; removed redundant BANKSEL
	MOVWF	_mot1_phase, B
;	.line	224; moteursetjack.c	PIN_MOT1_1 = 0;
	BCF	_PORTDbits, 2
;	.line	225; moteursetjack.c	PIN_MOT1_2 = 0;
	BCF	_PORTDbits, 3
;	.line	226; moteursetjack.c	PIN_MOT1_3 = 0;
	BCF	_PORTCbits, 6
;	.line	227; moteursetjack.c	PIN_MOT1_4 = 0;
	BCF	_PORTCbits, 7
_00230_DS_:
	BANKSEL	_compteur_mot
;	.line	229; moteursetjack.c	compteur_mot = 0;
	CLRF	_compteur_mot, B
; removed redundant BANKSEL
	CLRF	(_compteur_mot + 1), B
	BRA	_00243_DS_
_00240_DS_:
;	.line	234; moteursetjack.c	if(depart < 3)
	MOVLW	0x03
	BANKSEL	_depart
	SUBWF	_depart, W, B
	BC	_00237_DS_
;	.line	236; moteursetjack.c	if(PIN_JACK)
	BTFSC	_PORTBbits, 7
	BRA	_00243_DS_
;	.line	249; moteursetjack.c	else depart = 1;
	MOVLW	0x01
; removed redundant BANKSEL
	MOVWF	_depart, B
	BRA	_00243_DS_
_00237_DS_:
	BANKSEL	_depart
;	.line	251; moteursetjack.c	else depart++; // Pour les tempos au changement de sens...
	INCF	_depart, F, B
_00243_DS_:
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_moteursetjack__moteursetjack_ints_INT2	code
_moteursetjack_ints_INT2:
;	.line	182; moteursetjack.c	void moteursetjack_ints_INT2(void) // position avant
	MOVFF	r0x00, POSTDEC1
;	.line	184; moteursetjack.c	PIN_MOT2_1 = 0;
	BCF	_PORTDbits, 1
;	.line	185; moteursetjack.c	PIN_MOT2_2 = 0;
	BCF	_PORTDbits, 0
;	.line	186; moteursetjack.c	mot2_position = 2;
	MOVLW	0x02
	BANKSEL	_mot2_position
	MOVWF	_mot2_position, B
;	.line	187; moteursetjack.c	etat_moteur[1] &= ~POSITION_MOTEUR_2; // Maj position
	MOVLW	0xfc
	BANKSEL	(_etat_moteur + 1)
	ANDWF	(_etat_moteur + 1), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	(_etat_moteur + 1), B
;	.line	188; moteursetjack.c	etat_moteur[1] |= 0x02;               // ..
	MOVLW	0x02
; removed redundant BANKSEL
	IORWF	(_etat_moteur + 1), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	r0x00
; removed redundant BANKSEL
	MOVWF	(_etat_moteur + 1), B
;	.line	189; moteursetjack.c	etat_moteur[1] &= ~ACTIV_MOTEUR_2;    // Inactif !
	BCF	r0x00, 3
	MOVF	r0x00, W
; removed redundant BANKSEL
	MOVWF	(_etat_moteur + 1), B
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_moteursetjack__moteursetjack_ints_INT1	code
_moteursetjack_ints_INT1:
	BANKSEL	_mot2_pos_visee
;	.line	172; moteursetjack.c	if(mot2_pos_visee == 1)
	MOVF	_mot2_pos_visee, W, B
	XORLW	0x01
	BNZ	_00209_DS_
;	.line	174; moteursetjack.c	PIN_MOT2_1 = 0;
	BCF	_PORTDbits, 1
;	.line	175; moteursetjack.c	PIN_MOT2_2 = 0;
	BCF	_PORTDbits, 0
;	.line	176; moteursetjack.c	etat_moteur[1] &= ~ACTIV_MOTEUR_2;// Inactif !
	MOVLW	0xf7
	BANKSEL	(_etat_moteur + 1)
	ANDWF	(_etat_moteur + 1), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	(_etat_moteur + 1), B
_00209_DS_:
;	.line	178; moteursetjack.c	mot2_position = 1;
	MOVLW	0x01
	BANKSEL	_mot2_position
	MOVWF	_mot2_position, B
;	.line	179; moteursetjack.c	etat_moteur[1] &= ~POSITION_MOTEUR_2; // Maj position
	MOVLW	0xfc
	BANKSEL	(_etat_moteur + 1)
	ANDWF	(_etat_moteur + 1), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	(_etat_moteur + 1), B
;	.line	180; moteursetjack.c	etat_moteur[1] |= 0x01;               // ..
	MOVLW	0x01
; removed redundant BANKSEL
	IORWF	(_etat_moteur + 1), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	(_etat_moteur + 1), B
	RETURN	

; ; Starting pCode block
S_moteursetjack__moteursetjack_ints_INT0	code
_moteursetjack_ints_INT0:
;	.line	163; moteursetjack.c	PIN_MOT2_1 = 0;
	BCF	_PORTDbits, 1
;	.line	164; moteursetjack.c	PIN_MOT2_2 = 0;
	BCF	_PORTDbits, 0
	BANKSEL	_mot2_position
;	.line	165; moteursetjack.c	mot2_position = 0;
	CLRF	_mot2_position, B
;	.line	166; moteursetjack.c	etat_moteur[1] &= ~POSITION_MOTEUR_2; // Maj position
	MOVLW	0xfc
	BANKSEL	(_etat_moteur + 1)
	ANDWF	(_etat_moteur + 1), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	(_etat_moteur + 1), B
;	.line	168; moteursetjack.c	etat_moteur[1] &= ~ACTIV_MOTEUR_2;    // Inactif !
	MOVLW	0xf7
; removed redundant BANKSEL
	ANDWF	(_etat_moteur + 1), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	(_etat_moteur + 1), B
	RETURN	

; ; Starting pCode block
S_moteursetjack__moteursetjack_ints_RB_haut	code
_moteursetjack_ints_RB_haut:
;	.line	140; moteursetjack.c	void moteursetjack_ints_RB_haut(void)
	MOVFF	r0x00, POSTDEC1
; #	MOVLW	0x3f
; #	MOVWF	_mot1_pos_visee, B
; #	MOVLW	0x3f
; ;     peep 5 - Removed redundant move
;	.line	146; moteursetjack.c	mot1_pos_visee = POSITION_MAX_1;
	MOVLW	0x3f
	BANKSEL	_mot1_pos_visee
;	.line	147; moteursetjack.c	mot1_position = POSITION_MAX_1;
	MOVWF	_mot1_pos_visee, B
	BANKSEL	_mot1_position
	MOVWF	_mot1_position, B
;	.line	148; moteursetjack.c	etat_moteur[0] &= ~POSITION_MOTEUR_1; // Maj position
	MOVLW	0xc0
	BANKSEL	_etat_moteur
	ANDWF	_etat_moteur, W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	_etat_moteur, B
;	.line	149; moteursetjack.c	etat_moteur[0] |= POSITION_MAX_1;     // ..
	MOVLW	0x3f
; removed redundant BANKSEL
	IORWF	_etat_moteur, W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	r0x00
; removed redundant BANKSEL
	MOVWF	_etat_moteur, B
;	.line	151; moteursetjack.c	T0CON = CONFIG_TMR0_MOTEUR & 0x7f; // TMR0OFF
	MOVLW	0x06
	MOVWF	_T0CON
;	.line	152; moteursetjack.c	etat_moteur[0] &= ~ACTIV_MOTEUR_1; // Inactif
	BCF	r0x00, 7
	MOVF	r0x00, W
; removed redundant BANKSEL
	MOVWF	_etat_moteur, B
;	.line	154; moteursetjack.c	mot1_phase.b0 = PIN_MOT1_1;
	CLRF	r0x00
	BTFSC	_PORTDbits, 2
	INCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	MOVWF	PRODH
	BANKSEL	_mot1_phase
	MOVF	_mot1_phase, W, B
	ANDLW	0xfe
	IORWF	PRODH, W
; removed redundant BANKSEL
	MOVWF	_mot1_phase, B
;	.line	155; moteursetjack.c	mot1_phase.b1 = PIN_MOT1_3;
	CLRF	r0x00
	BTFSC	_PORTCbits, 6
	INCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	RLNCF	WREG, W
	MOVWF	PRODH
; removed redundant BANKSEL
	MOVF	_mot1_phase, W, B
	ANDLW	0xfd
	IORWF	PRODH, W
; removed redundant BANKSEL
	MOVWF	_mot1_phase, B
;	.line	156; moteursetjack.c	PIN_MOT1_1 = 0;
	BCF	_PORTDbits, 2
;	.line	157; moteursetjack.c	PIN_MOT1_2 = 0;
	BCF	_PORTDbits, 3
;	.line	158; moteursetjack.c	PIN_MOT1_3 = 0;
	BCF	_PORTCbits, 6
;	.line	159; moteursetjack.c	PIN_MOT1_4 = 0;
	BCF	_PORTCbits, 7
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_moteursetjack__moteursetjack_ints_RB_bas	code
_moteursetjack_ints_RB_bas:
;	.line	119; moteursetjack.c	void moteursetjack_ints_RB_bas(void)
	MOVFF	r0x00, POSTDEC1
	BANKSEL	_mot1_pos_visee
;	.line	125; moteursetjack.c	mot1_pos_visee = 0;
	CLRF	_mot1_pos_visee, B
	BANKSEL	_mot1_position
;	.line	126; moteursetjack.c	mot1_position = 0;
	CLRF	_mot1_position, B
;	.line	127; moteursetjack.c	etat_moteur[0] &= ~POSITION_MOTEUR_1; // Maj position
	MOVLW	0xc0
	BANKSEL	_etat_moteur
	ANDWF	_etat_moteur, W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	_etat_moteur, B
;	.line	130; moteursetjack.c	T0CON = CONFIG_TMR0_MOTEUR & 0x7f; // TMR0OFF
	MOVLW	0x06
	MOVWF	_T0CON
;	.line	131; moteursetjack.c	etat_moteur[0] &= ~ACTIV_MOTEUR_1; // Inactif
	MOVLW	0x7f
; removed redundant BANKSEL
	ANDWF	_etat_moteur, W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	_etat_moteur, B
;	.line	133; moteursetjack.c	mot1_phase.b0 = PIN_MOT1_1;
	CLRF	r0x00
	BTFSC	_PORTDbits, 2
	INCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	MOVWF	PRODH
	BANKSEL	_mot1_phase
	MOVF	_mot1_phase, W, B
	ANDLW	0xfe
	IORWF	PRODH, W
; removed redundant BANKSEL
	MOVWF	_mot1_phase, B
;	.line	134; moteursetjack.c	mot1_phase.b1 = PIN_MOT1_3;
	CLRF	r0x00
	BTFSC	_PORTCbits, 6
	INCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	RLNCF	WREG, W
	MOVWF	PRODH
; removed redundant BANKSEL
	MOVF	_mot1_phase, W, B
	ANDLW	0xfd
	IORWF	PRODH, W
; removed redundant BANKSEL
	MOVWF	_mot1_phase, B
;	.line	135; moteursetjack.c	PIN_MOT1_1 = 0;
	BCF	_PORTDbits, 2
;	.line	136; moteursetjack.c	PIN_MOT1_2 = 0;
	BCF	_PORTDbits, 3
;	.line	137; moteursetjack.c	PIN_MOT1_3 = 0;
	BCF	_PORTCbits, 6
;	.line	138; moteursetjack.c	PIN_MOT1_4 = 0;
	BCF	_PORTCbits, 7
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_moteursetjack__ordre_moteur	code
_ordre_moteur:
;	.line	35; moteursetjack.c	void ordre_moteur(unsigned char ordre1, unsigned char ordre2)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
;	.line	38; moteursetjack.c	if(ordre1 & ACTIV_MOTEUR_1)
	BTFSS	r0x00, 7
	BRA	_00133_DS_
;	.line	40; moteursetjack.c	if(ordre1 & INITIALISATION_1) 
	BTFSS	r0x00, 6
	BRA	_00113_DS_
;	.line	42; moteursetjack.c	dest = 0;
	CLRF	r0x02
;	.line	43; moteursetjack.c	if(FIN_COURSE1_BAS)
	BTFSS	_PORTBbits, 3
	BRA	_00110_DS_
	BANKSEL	_mot1_position
;	.line	44; moteursetjack.c	mot1_position = 0;
	CLRF	_mot1_position, B
	BRA	_00111_DS_
_00110_DS_:
;	.line	46; moteursetjack.c	mot1_position = 255;
	MOVLW	0xff
	BANKSEL	_mot1_position
	MOVWF	_mot1_position, B
_00111_DS_:
;	.line	47; moteursetjack.c	etat_moteur[0] &= ~INITIALISATION_1;
	MOVLW	0xbf
	BANKSEL	_etat_moteur
	ANDWF	_etat_moteur, W, B
; #	MOVWF	r0x03
; #	MOVF	r0x03, W
; ;     peep 2 - Removed redundant move
	MOVWF	_etat_moteur, B
	BRA	_00114_DS_
_00113_DS_:
;	.line	49; moteursetjack.c	else dest = (ordre1 & POSITION_MOTEUR_1);
	MOVLW	0x3f
	ANDWF	r0x00, W
	MOVWF	r0x02
_00114_DS_:
;	.line	50; moteursetjack.c	if((dest != mot1_position) || (etat_moteur[0] & ACTIV_MOTEUR_1))
	MOVF	r0x02, W
	BANKSEL	_mot1_position
	XORWF	_mot1_position, W, B
	BNZ	_00129_DS_
_00175_DS_:
	BANKSEL	_etat_moteur
	BTFSS	_etat_moteur, 7, B
	BRA	_00133_DS_
_00129_DS_:
;	.line	52; moteursetjack.c	if(etat_moteur[0] & ACTIV_MOTEUR_1)
	MOVFF	_etat_moteur, r0x00
	BTFSS	r0x00, 7
	BRA	_00124_DS_
;	.line	54; moteursetjack.c	if(((dest > mot1_position)&&(!mot1_sens))||((dest < mot1_position)&&(mot1_sens))) // ie si on change de sens
	MOVF	r0x02, W
	BANKSEL	_mot1_position
	SUBWF	_mot1_position, W, B
	BC	_00122_DS_
	BANKSEL	_mot1_sens
	MOVF	_mot1_sens, W, B
	BZ	_00118_DS_
_00122_DS_:
	BANKSEL	_mot1_position
	MOVF	_mot1_position, W, B
	SUBWF	r0x02, W
	BTFSC	STATUS, 0
	BRA	_00125_DS_
	BANKSEL	_mot1_sens
	MOVF	_mot1_sens, W, B
	BTFSC	STATUS, 2
	BRA	_00125_DS_
_00118_DS_:
	BANKSEL	_compteur_mot
;	.line	56; moteursetjack.c	compteur_mot = PAS_PAR_ETAGE - compteur_mot;
	MOVF	_compteur_mot, W, B
	SUBLW	0x32
; removed redundant BANKSEL
	MOVWF	_compteur_mot, B
	MOVLW	0x00
; removed redundant BANKSEL
	SUBFWB	(_compteur_mot + 1), F, B
;	.line	57; moteursetjack.c	depart = -TEMPS_ATTENTE; // Attend TEMPS_ATTENTE interruptions du timer0 avant de repartir dans l'autre sens
	MOVLW	0xec
	BANKSEL	_depart
	MOVWF	_depart, B
	BANKSEL	_mot1_sens
;	.line	58; moteursetjack.c	if(mot1_sens) mot1_position++;
	MOVF	_mot1_sens, W, B
	BZ	_00116_DS_
	BANKSEL	_mot1_position
	INCF	_mot1_position, F, B
	BRA	_00125_DS_
_00116_DS_:
	BANKSEL	_mot1_position
;	.line	59; moteursetjack.c	else mot1_position--;
	DECF	_mot1_position, F, B
	BRA	_00125_DS_
_00124_DS_:
;	.line	64; moteursetjack.c	etat_moteur[0] |= ACTIV_MOTEUR_1;
	BSF	r0x00, 7
	MOVF	r0x00, W
	BANKSEL	_etat_moteur
	MOVWF	_etat_moteur, B
	BANKSEL	_compteur_mot
;	.line	65; moteursetjack.c	compteur_mot = 0;
	CLRF	_compteur_mot, B
; removed redundant BANKSEL
	CLRF	(_compteur_mot + 1), B
;	.line	66; moteursetjack.c	PIN_MOT1_1 = mot1_phase.b0;
	CLRF	r0x00
	BANKSEL	_mot1_phase
	BTFSC	_mot1_phase, 0, B
	INCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	RLNCF	WREG, W
	RLNCF	WREG, W
	MOVWF	PRODH
	MOVF	_PORTDbits, W
	ANDLW	0xfb
	IORWF	PRODH, W
	MOVWF	_PORTDbits
;	.line	67; moteursetjack.c	PIN_MOT1_2 = !mot1_phase.b0;
	CLRF	r0x00
; removed redundant BANKSEL
	BTFSC	_mot1_phase, 0, B
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
	RRNCF	WREG, W
	MOVWF	PRODH
	MOVF	_PORTDbits, W
	ANDLW	0xf7
	IORWF	PRODH, W
	MOVWF	_PORTDbits
;	.line	68; moteursetjack.c	PIN_MOT1_3 = mot1_phase.b1;
	CLRF	r0x00
; removed redundant BANKSEL
	BTFSC	_mot1_phase, 1, B
	INCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	RRNCF	WREG, W
	RRNCF	WREG, W
	MOVWF	PRODH
	MOVF	_PORTCbits, W
	ANDLW	0xbf
	IORWF	PRODH, W
	MOVWF	_PORTCbits
;	.line	69; moteursetjack.c	PIN_MOT1_4 = !mot1_phase.b1;
	CLRF	r0x00
; removed redundant BANKSEL
	BTFSC	_mot1_phase, 1, B
	INCF	r0x00, F
	MOVF	r0x00, W
	BSF	STATUS, 0
	TSTFSZ	WREG
	BCF	STATUS, 0
	CLRF	r0x00
	RLCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	RRNCF	WREG, W
	MOVWF	PRODH
	MOVF	_PORTCbits, W
	ANDLW	0x7f
	IORWF	PRODH, W
	MOVWF	_PORTCbits
_00125_DS_:
;	.line	71; moteursetjack.c	T0CON = CONFIG_TMR0_MOTEUR;
	MOVLW	0x86
	MOVWF	_T0CON
;	.line	72; moteursetjack.c	mot1_sens = (dest > mot1_position);
	MOVF	r0x02, W
	BANKSEL	_mot1_position
	SUBWF	_mot1_position, W, B
	BTG	STATUS, 0
	BANKSEL	_mot1_sens
	CLRF	_mot1_sens, B
	RLCF	_mot1_sens, F, B
;	.line	73; moteursetjack.c	if(PIN_MOT1_1 == PIN_MOT1_3) ticoutac = mot1_sens; // A modifier si mauvais sens
	CLRF	r0x00
	BTFSC	_PORTDbits, 2
	INCF	r0x00, F
	CLRF	r0x03
	BTFSC	_PORTCbits, 6
	INCF	r0x03, F
	MOVF	r0x00, W
	XORWF	r0x03, W
	BNZ	_00127_DS_
	MOVFF	_mot1_sens, _ticoutac
	BRA	_00133_DS_
_00127_DS_:
	BANKSEL	_mot1_sens
;	.line	74; moteursetjack.c	else ticoutac = !mot1_sens;
	MOVF	_mot1_sens, W, B
	BSF	STATUS, 0
	TSTFSZ	WREG
	BCF	STATUS, 0
	BANKSEL	_ticoutac
	CLRF	_ticoutac, B
	RLCF	_ticoutac, F, B
_00133_DS_:
;	.line	77; moteursetjack.c	if(ordre2 & ACTIV_MOTEUR_2)
	BTFSS	r0x01, 3
	BRA	_00145_DS_
;	.line	79; moteursetjack.c	if(ordre2 & INITIALISATION_2)
	BTFSS	r0x01, 2
	BRA	_00138_DS_
;	.line	81; moteursetjack.c	if(FIN_COURSE2_ARRIERE) mot2_position = 0; else mot2_position = 2;
	BTFSS	_PORTBbits, 0
	BRA	_00135_DS_
	BANKSEL	_mot2_position
	CLRF	_mot2_position, B
	BRA	_00138_DS_
_00135_DS_:
	MOVLW	0x02
	BANKSEL	_mot2_position
	MOVWF	_mot2_position, B
_00138_DS_:
;	.line	84; moteursetjack.c	dest = (ordre2 & POSITION_MOTEUR_2);
	MOVLW	0x03
	ANDWF	r0x01, W
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
; ;     peep 2 - Removed redundant move
;	.line	85; moteursetjack.c	if(dest != mot2_position)
	MOVWF	r0x02
	BANKSEL	_mot2_position
	XORWF	_mot2_position, W, B
	BZ	_00145_DS_
;	.line	87; moteursetjack.c	mot2_pos_visee = dest;
	MOVFF	r0x02, _mot2_pos_visee
;	.line	88; moteursetjack.c	if(dest > mot2_position)
	MOVF	r0x02, W
	BANKSEL	_mot2_position
	SUBWF	_mot2_position, W, B
	BC	_00140_DS_
;	.line	90; moteursetjack.c	PIN_MOT2_1 = 1;
	BSF	_PORTDbits, 1
;	.line	91; moteursetjack.c	PIN_MOT2_2 = 0;
	BCF	_PORTDbits, 0
	BRA	_00141_DS_
_00140_DS_:
;	.line	95; moteursetjack.c	PIN_MOT2_1 = 0;
	BCF	_PORTDbits, 1
;	.line	96; moteursetjack.c	PIN_MOT2_2 = 1;
	BSF	_PORTDbits, 0
_00141_DS_:
;	.line	98; moteursetjack.c	etat_moteur[1] |= ACTIV_MOTEUR_2;
	MOVLW	0x08
	BANKSEL	(_etat_moteur + 1)
	IORWF	(_etat_moteur + 1), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
; ;     peep 2 - Removed redundant move
	MOVWF	(_etat_moteur + 1), B
_00145_DS_:
;	.line	101; moteursetjack.c	if(ordre2 & ACTIV_MOTEUR_3)
	BTFSS	r0x01, 6
	BRA	_00152_DS_
;	.line	103; moteursetjack.c	ftoggle_A4();
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
;	.line	104; moteursetjack.c	switch (ordre2 & MOTEUR_3)
	MOVLW	0x30
	ANDWF	r0x01, W
	MOVWF	r0x00
	MOVF	r0x00, W
	BZ	_00146_DS_
	MOVF	r0x00, W
	XORLW	0x10
	BZ	_00147_DS_
	MOVF	r0x00, W
	XORLW	0x20
	BZ	_00148_DS_
	BRA	_00152_DS_
_00146_DS_:
;	.line	106; moteursetjack.c	case 0x00: PIN_MOT3_1 = 0;
	BCF	_PORTCbits, 0
;	.line	107; moteursetjack.c	PIN_MOT3_2 = 0;
	BCF	_PORTCbits, 1
;	.line	108; moteursetjack.c	break;
	BRA	_00152_DS_
_00147_DS_:
;	.line	109; moteursetjack.c	case 0x10: PIN_MOT3_1 = 1;
	BSF	_PORTCbits, 0
;	.line	110; moteursetjack.c	PIN_MOT3_2 = 0;
	BCF	_PORTCbits, 1
;	.line	111; moteursetjack.c	break;
	BRA	_00152_DS_
_00148_DS_:
;	.line	112; moteursetjack.c	case 0x20: PIN_MOT3_1 = 0;
	BCF	_PORTCbits, 0
;	.line	113; moteursetjack.c	PIN_MOT3_2 = 1;
	BSF	_PORTCbits, 1
_00152_DS_:
;	.line	115; moteursetjack.c	}
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_moteursetjack__init_moteursetjack	code
_init_moteursetjack:
;	.line	22; moteursetjack.c	T0CON = CONFIG_TMR0_MOTEUR & 0x7F;// TMR0 off...    //| 0x08; // PSA off
	MOVLW	0x06
	MOVWF	_T0CON
;	.line	23; moteursetjack.c	etat_moteur[0] = INITIALISATION_1;
	MOVLW	0x40
	BANKSEL	_etat_moteur
	MOVWF	_etat_moteur, B
;	.line	24; moteursetjack.c	etat_moteur[1] = INITIALISATION_2;
	MOVLW	0x04
; removed redundant BANKSEL
	MOVWF	(_etat_moteur + 1), B
;	.line	25; moteursetjack.c	INTCON |= 0x38;                    // INT0, TMR0, RB on...
	MOVLW	0x38
	IORWF	_INTCON, F
;	.line	26; moteursetjack.c	INTCON &= 0xf8;                    // ...
	MOVLW	0xf8
	ANDWF	_INTCON, F
;	.line	27; moteursetjack.c	INTCON2 = 0x85;                    // Rising edge for INT0,1,2. High priority for RB.
	MOVLW	0x85
	MOVWF	_INTCON2
;	.line	28; moteursetjack.c	INTCON3 = 0xD8;                    // INT1,2 on, High priority.
	MOVLW	0xd8
	MOVWF	_INTCON3
;	.line	29; moteursetjack.c	TRISD &= 0xF0;                     // RD0..3 en sorties
	MOVLW	0xf0
	ANDWF	_TRISD, F
;	.line	30; moteursetjack.c	TRISC &= 0x3C;                     // RC0, 1, 6, 7 en sorties
	MOVLW	0x3c
	ANDWF	_TRISC, F
;	.line	31; moteursetjack.c	PORTD &= 0xF0;
	MOVLW	0xf0
	ANDWF	_PORTD, F
;	.line	32; moteursetjack.c	PORTC &= 0x3C;
	MOVLW	0x3c
	ANDWF	_PORTC, F
	RETURN	



; Statistics:
; code size:	 1232 (0x04d0) bytes ( 0.94%)
;           	  616 (0x0268) words
; udata size:	    3 (0x0003) bytes ( 0.17%)
; access size:	    4 (0x0004) bytes


	end
