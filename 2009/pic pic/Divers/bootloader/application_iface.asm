;--------------------------------------------------------
; File Created by SDCC : FreeWare ANSI-C Compiler
; Version 2.6.1 #4482 (Mar 15 2009)
; This file generated Tue Apr 14 19:33:59 2009
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f4550

	radix dec

;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global _application_data

;--------------------------------------------------------
; extern variables in this module
;--------------------------------------------------------
	extern _section_descriptor

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; ; Starting pCode block for absolute Ival
S_application_iface__application_data	code	0X002000
_application_data:
	DB	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	DB	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	DB	0x00

; I code from now on!


; Statistics:
; code size:	    6 (0x0006) bytes ( 0.00%)
;           	    3 (0x0003) words
; udata size:	    0 (0x0000) bytes ( 0.00%)
; access size:	    0 (0x0000) bytes


	end
