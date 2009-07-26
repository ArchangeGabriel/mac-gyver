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
	global _i
	global _checksum
	global _my_ep1_init
	global _my_ep1_out
	global _ep1_OutBuffer
	global _tetedelecture

;--------------------------------------------------------
; extern variables in this module
;--------------------------------------------------------
	extern __gptrput1
	extern __gptrget1
	extern _ep2_num_bytes_to_send
	extern _ep2_source_data
	extern _an_data
	extern _mydata
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
	extern _my_prepare_ep2_in
	extern _set_servo
	extern _ordre_moteur
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
FSR0L	equ	0xfe9
FSR0H	equ	0xfea
FSR1L	equ	0xfe1
FSR2L	equ	0xfd9
INDF0	equ	0xfef
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4
PRODL	equ	0xff3


; Internal registers
.registers	udata_ovr	0x0000
r0x00	res	1
r0x01	res	1
r0x02	res	1
r0x03	res	1
r0x04	res	1
r0x05	res	1

udata_my_ep1_0	udata
_checksum	res	1

udata_my_ep1_1	udata
_i	res	1

udata_my_ep1_2	udata
_tetedelecture	res	1

udata_my_ep1_3	udata
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
;	.line	29; my_ep1.c	void my_ep1_out(void)
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
;	.line	31; my_ep1.c	if(EP_OUT_BD(1).Cnt >= 1)
	MOVFF	(_ep_bdt + 9), (iTemp0 + 1)
	MOVLW	0x01
	BANKSEL	(iTemp0 + 1)
	SUBWF	(iTemp0 + 1), W, B
	BTFSS	STATUS, 0
	BRA	_00124_DS_
;	.line	33; my_ep1.c	ep2_num_bytes_to_send = 1;
	MOVLW	0x01
	BANKSEL	_ep2_num_bytes_to_send
	MOVWF	_ep2_num_bytes_to_send, B
; removed redundant BANKSEL
	CLRF	(_ep2_num_bytes_to_send + 1), B
;	.line	34; my_ep1.c	ep2_source_data = mydata;
	MOVFF	_mydata, _ep2_source_data
	MOVFF	(_mydata + 1), (_ep2_source_data + 1)
	MOVFF	(_mydata + 2), (_ep2_source_data + 2)
;	.line	35; my_ep1.c	mydata[0] =  RECU;
	MOVFF	_mydata, r0x00
	MOVFF	(_mydata + 1), r0x01
	MOVFF	(_mydata + 2), r0x02
	CLRF	POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
;	.line	36; my_ep1.c	tetedelecture = 1;
	MOVLW	0x01
	BANKSEL	_tetedelecture
	MOVWF	_tetedelecture, B
	BANKSEL	_ep1_OutBuffer
;	.line	38; my_ep1.c	if(ep1_OutBuffer[0] & ANALOGS)
	BTFSS	_ep1_OutBuffer, 7, B
	BRA	_00110_DS_
;	.line	40; my_ep1.c	ep2_num_bytes_to_send += 2 * NB_ANALOGS;
	MOVLW	0x09
	BANKSEL	_ep2_num_bytes_to_send
	MOVWF	_ep2_num_bytes_to_send, B
; removed redundant BANKSEL
	CLRF	(_ep2_num_bytes_to_send + 1), B
;	.line	41; my_ep1.c	ep2_source_data = an_data;
	MOVLW	HIGH(_an_data)
	BANKSEL	(_ep2_source_data + 1)
	MOVWF	(_ep2_source_data + 1), B
	MOVLW	LOW(_an_data)
; removed redundant BANKSEL
	MOVWF	_ep2_source_data, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_ep2_source_data + 2), B
;	.line	42; my_ep1.c	ep2_source_data[0] = ANALOGS | RECU;
	MOVFF	_ep2_source_data, r0x00
	MOVFF	(_ep2_source_data + 1), r0x01
	MOVFF	(_ep2_source_data + 2), r0x02
	MOVWF	POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
_00110_DS_:
	BANKSEL	_ep1_OutBuffer
;	.line	44; my_ep1.c	if(ep1_OutBuffer[0] & DIGITALS)
	BTFSS	_ep1_OutBuffer, 6, B
	BRA	_00112_DS_
;	.line	46; my_ep1.c	ep2_source_data[ep2_num_bytes_to_send++] = PORTB & 0xEF;
	MOVFF	_ep2_num_bytes_to_send, r0x00
	MOVFF	(_ep2_num_bytes_to_send + 1), r0x01
	BANKSEL	_ep2_num_bytes_to_send
	INCF	_ep2_num_bytes_to_send, F, B
	BNC	_10109_DS_
; removed redundant BANKSEL
	INCF	(_ep2_num_bytes_to_send + 1), F, B
_10109_DS_:
	BANKSEL	_ep2_source_data
	MOVF	_ep2_source_data, W, B
	ADDWF	r0x00, F
; removed redundant BANKSEL
	MOVF	(_ep2_source_data + 1), W, B
	ADDWFC	r0x01, F
; removed redundant BANKSEL
	MOVF	(_ep2_source_data + 2), W, B
	CLRF	r0x02
	ADDWFC	r0x02, F
	MOVLW	0xef
	ANDWF	_PORTB, W
	MOVWF	r0x03
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
;	.line	47; my_ep1.c	ep2_source_data[0] |= DIGITALS;
	MOVFF	_ep2_source_data, r0x00
	MOVFF	(_ep2_source_data + 1), r0x01
	MOVFF	(_ep2_source_data + 2), r0x02
	MOVFF	_ep2_source_data, r0x03
	MOVFF	(_ep2_source_data + 1), r0x04
	MOVFF	(_ep2_source_data + 2), r0x05
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrget1
	MOVWF	r0x03
	BSF	r0x03, 6
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
_00112_DS_:
	BANKSEL	_ep1_OutBuffer
;	.line	49; my_ep1.c	if(ep1_OutBuffer[0] & SERVO)
	BTFSS	_ep1_OutBuffer, 4, B
	BRA	_00117_DS_
; removed redundant BANKSEL
;	.line	51; my_ep1.c	if(set_servo(0, ep1_OutBuffer[tetedelecture]))
	MOVF	(_ep1_OutBuffer + 1), W, B
	MOVWF	POSTDEC1
	MOVLW	0x00
	CLRF	POSTDEC1
	CALL	_set_servo
	MOVWF	r0x00
	MOVLW	0x02
	ADDWF	FSR1L, F
	MOVF	r0x00, W
	BZ	_00114_DS_
;	.line	52; my_ep1.c	ep2_source_data[0] |= SERVO;
	MOVFF	_ep2_source_data, r0x00
	MOVFF	(_ep2_source_data + 1), r0x01
	MOVFF	(_ep2_source_data + 2), r0x02
	MOVFF	_ep2_source_data, r0x03
	MOVFF	(_ep2_source_data + 1), r0x04
	MOVFF	(_ep2_source_data + 2), r0x05
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrget1
	MOVWF	r0x03
	BSF	r0x03, 4
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
	BRA	_00115_DS_
_00114_DS_:
;	.line	54; my_ep1.c	ep2_source_data[0] |= ERREUR_SERVO;
	MOVFF	_ep2_source_data, r0x00
	MOVFF	(_ep2_source_data + 1), r0x01
	MOVFF	(_ep2_source_data + 2), r0x02
	MOVFF	_ep2_source_data, r0x03
	MOVFF	(_ep2_source_data + 1), r0x04
	MOVFF	(_ep2_source_data + 2), r0x05
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrget1
	MOVWF	r0x03
	BSF	r0x03, 1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
_00115_DS_:
	BANKSEL	_tetedelecture
;	.line	55; my_ep1.c	tetedelecture ++;
	INCF	_tetedelecture, F, B
_00117_DS_:
	BANKSEL	_ep1_OutBuffer
;	.line	57; my_ep1.c	if(ep1_OutBuffer[0] & MOTEURS)
	BTFSS	_ep1_OutBuffer, 5, B
	BRA	_00119_DS_
;	.line	59; my_ep1.c	ordre_moteur(ep1_OutBuffer[tetedelecture],ep1_OutBuffer[tetedelecture+1]);
	MOVLW	LOW(_ep1_OutBuffer)
	BANKSEL	_tetedelecture
	ADDWF	_tetedelecture, W, B
	MOVWF	r0x00
	CLRF	r0x01
	MOVLW	HIGH(_ep1_OutBuffer)
	ADDWFC	r0x01, F
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, FSR0H
	MOVFF	INDF0, r0x00
; removed redundant BANKSEL
	INCF	_tetedelecture, W, B
	MOVWF	r0x01
	MOVLW	LOW(_ep1_OutBuffer)
	ADDWF	r0x01, F
	MOVLW	HIGH(_ep1_OutBuffer)
	CLRF	r0x02
	ADDWFC	r0x02, F
	MOVFF	r0x01, FSR0L
	MOVFF	r0x02, FSR0H
	MOVFF	INDF0, r0x01
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_ordre_moteur
	MOVLW	0x02
	ADDWF	FSR1L, F
	BANKSEL	_tetedelecture
;	.line	60; my_ep1.c	tetedelecture+=2;
	INCF	_tetedelecture, F, B
	INCF	_tetedelecture, F, B
;	.line	61; my_ep1.c	ep2_source_data[0] |= MOTEURS;
	MOVFF	_ep2_source_data, r0x00
	MOVFF	(_ep2_source_data + 1), r0x01
	MOVFF	(_ep2_source_data + 2), r0x02
	MOVFF	_ep2_source_data, r0x03
	MOVFF	(_ep2_source_data + 1), r0x04
	MOVFF	(_ep2_source_data + 2), r0x05
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrget1
	MOVWF	r0x03
	BSF	r0x03, 5
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
;	.line	62; my_ep1.c	ep2_source_data[ep2_num_bytes_to_send++] = etat_moteur[0];
	MOVFF	_ep2_num_bytes_to_send, r0x00
	MOVFF	(_ep2_num_bytes_to_send + 1), r0x01
	BANKSEL	_ep2_num_bytes_to_send
	INCF	_ep2_num_bytes_to_send, F, B
	BNC	_20110_DS_
; removed redundant BANKSEL
	INCF	(_ep2_num_bytes_to_send + 1), F, B
_20110_DS_:
	BANKSEL	_ep2_source_data
	MOVF	_ep2_source_data, W, B
	ADDWF	r0x00, F
; removed redundant BANKSEL
	MOVF	(_ep2_source_data + 1), W, B
	ADDWFC	r0x01, F
; removed redundant BANKSEL
	MOVF	(_ep2_source_data + 2), W, B
	CLRF	r0x02
	ADDWFC	r0x02, F
	MOVFF	_etat_moteur, r0x03
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
;	.line	63; my_ep1.c	ep2_source_data[ep2_num_bytes_to_send++] = etat_moteur[1];
	MOVFF	_ep2_num_bytes_to_send, r0x00
	MOVFF	(_ep2_num_bytes_to_send + 1), r0x01
	BANKSEL	_ep2_num_bytes_to_send
	INCF	_ep2_num_bytes_to_send, F, B
	BNC	_30111_DS_
; removed redundant BANKSEL
	INCF	(_ep2_num_bytes_to_send + 1), F, B
_30111_DS_:
	BANKSEL	_ep2_source_data
	MOVF	_ep2_source_data, W, B
	ADDWF	r0x00, F
; removed redundant BANKSEL
	MOVF	(_ep2_source_data + 1), W, B
	ADDWFC	r0x01, F
; removed redundant BANKSEL
	MOVF	(_ep2_source_data + 2), W, B
	CLRF	r0x02
	ADDWFC	r0x02, F
	MOVFF	(_etat_moteur + 1), r0x03
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
_00119_DS_:
;	.line	67; my_ep1.c	my_prepare_ep2_in(); // Envoi
	CALL	_my_prepare_ep2_in
;	.line	69; my_ep1.c	EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 9)
	MOVWF	(_ep_bdt + 9), B
; removed redundant BANKSEL
;	.line	70; my_ep1.c	if(EP_OUT_BD(1).Stat.DTS == 0)
	BTFSC	(_ep_bdt + 8), 6, B
	BRA	_00121_DS_
;	.line	72; my_ep1.c	EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
	MOVLW	0xc8
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 8), B
	BRA	_00126_DS_
_00121_DS_:
;	.line	76; my_ep1.c	EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
	MOVLW	0x88
	BANKSEL	(_ep_bdt + 8)
	MOVWF	(_ep_bdt + 8), B
	BRA	_00126_DS_
_00124_DS_:
;	.line	81; my_ep1.c	EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 9)
	MOVWF	(_ep_bdt + 9), B
; removed redundant BANKSEL
;	.line	82; my_ep1.c	EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
	CLRF	(_ep_bdt + 10), B
	MOVLW	0x05
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 11), B
;	.line	83; my_ep1.c	EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_BSTALL;
	MOVLW	0x84
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 8), B
_00126_DS_:
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_my_ep1__my_ep1_init	code
_my_ep1_init:
;	.line	23; my_ep1.c	EP_OUT_BD(1).Cnt = MY_EP1_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 9)
	MOVWF	(_ep_bdt + 9), B
; removed redundant BANKSEL
;	.line	24; my_ep1.c	EP_OUT_BD(1).ADR = (uchar __data *)&ep1_OutBuffer;
	CLRF	(_ep_bdt + 10), B
	MOVLW	0x05
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 11), B
;	.line	25; my_ep1.c	EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
	MOVLW	0x88
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 8), B
;	.line	26; my_ep1.c	UEP1 = EPHSHK_EN | EPOUTEN_EN | EPCONDIS_EN;       // Init EP1 as an OUT EP
	MOVLW	0x1c
	MOVWF	_UEP1
	RETURN	



; Statistics:
; code size:	  764 (0x02fc) bytes ( 0.58%)
;           	  382 (0x017e) words
; udata size:	   68 (0x0044) bytes ( 3.79%)
; access size:	    6 (0x0006) bytes


	end
