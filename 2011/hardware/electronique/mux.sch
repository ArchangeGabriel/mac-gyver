EESchema Schematic File Version 2  date mar. 18 mai 2010 19:38:00 CEST
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:st-microelectronics
LIBS:18f4550
LIBS:optocoupler-2
LIBS:relay
LIBS:tlc549
LIBS:master-cache
EELAYER 24  0
EELAYER END
$Descr A4 11700 8267
Sheet 2 7
Title ""
Date "18 may 2010"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 3250 2500 0    60   ~ 0
GND
Text Label 3250 1000 0    60   ~ 0
VCC
Text HLabel 2000 3250 0    60   Input ~ 0
MotClk
Text HLabel 2000 3050 0    60   Input ~ 0
MotSerial
Wire Wire Line
	2900 3050 2000 3050
Connection ~ 2800 4150
Wire Wire Line
	2500 4150 3300 4150
Wire Wire Line
	3300 2950 3300 2850
Wire Wire Line
	2800 2850 2800 3350
Wire Wire Line
	2800 3350 2900 3350
Wire Wire Line
	4300 3750 4600 3750
Wire Wire Line
	4300 3650 4600 3650
Wire Wire Line
	4300 3550 4600 3550
Wire Wire Line
	4300 3450 4600 3450
Wire Wire Line
	4300 3350 4600 3350
Wire Wire Line
	4300 3250 4600 3250
Wire Wire Line
	4300 3150 4600 3150
Wire Wire Line
	4300 3050 4600 3050
Connection ~ 3700 2500
Wire Wire Line
	2000 2500 4200 2500
Wire Wire Line
	2000 1000 4200 1000
Wire Wire Line
	4200 1000 4200 1100
Wire Wire Line
	2000 2250 3800 2250
Wire Wire Line
	2000 1250 2100 1250
Wire Wire Line
	5150 2350 5500 2350
Wire Wire Line
	5150 2250 5500 2250
Wire Wire Line
	5150 2150 5500 2150
Wire Wire Line
	5150 2050 5500 2050
Wire Wire Line
	5150 1950 5500 1950
Wire Wire Line
	5150 1850 5500 1850
Wire Wire Line
	5150 1750 5500 1750
Wire Wire Line
	5150 1650 5500 1650
Wire Wire Line
	5150 1550 5500 1550
Wire Wire Line
	3500 1150 3800 1150
Wire Wire Line
	3500 1250 3800 1250
Wire Wire Line
	3500 1350 3800 1350
Wire Wire Line
	3500 1450 3800 1450
Wire Wire Line
	3500 1550 3800 1550
Wire Wire Line
	3500 1650 3800 1650
Wire Wire Line
	3500 1750 3800 1750
Wire Wire Line
	3500 1850 3800 1850
Wire Wire Line
	3500 1950 3800 1950
Wire Wire Line
	5200 1250 5500 1250
Wire Wire Line
	2000 2100 3800 2100
Wire Wire Line
	3800 2350 3700 2350
Wire Wire Line
	3700 2350 3700 2500
Wire Wire Line
	4200 2500 4200 2400
Wire Wire Line
	3100 1100 3100 1000
Connection ~ 3100 1000
Wire Wire Line
	3100 2000 3100 2500
Connection ~ 3100 2500
Wire Wire Line
	2900 3650 2800 3650
Wire Wire Line
	2800 3650 2800 4150
Wire Wire Line
	3300 4150 3300 4050
Wire Wire Line
	3300 2850 2500 2850
Connection ~ 2800 2850
Wire Wire Line
	2900 3550 2700 3550
Wire Wire Line
	2700 3550 2700 3250
Wire Wire Line
	2000 3250 2900 3250
Connection ~ 2700 3250
Text Label 2500 4150 0    60   ~ 0
GND
Text Label 2500 2850 0    60   ~ 0
VCC
Text HLabel 4600 3750 2    60   Output ~ 0
SigMOT42
Text HLabel 4600 3550 2    60   Output ~ 0
SigMOT32
Text HLabel 4600 3350 2    60   Output ~ 0
SigMOT22
Text HLabel 4600 3650 2    60   Output ~ 0
SigMOT41
Text HLabel 4600 3450 2    60   Output ~ 0
SigMOT31
Text HLabel 4600 3250 2    60   Output ~ 0
SigMOT21
Text HLabel 4600 3150 2    60   Output ~ 0
SigMOT12
Text HLabel 4600 3050 2    60   Output ~ 0
SigMOT11
Text HLabel 2000 1000 0    60   Input ~ 0
VCC
Text HLabel 2000 2500 0    60   Input ~ 0
GND
Text HLabel 2000 1250 0    60   Output ~ 0
Int
Text HLabel 2000 2250 0    60   Input ~ 0
Cp
Text HLabel 2000 2100 0    60   Input ~ 0
PL
Text HLabel 5500 1250 2    60   Output ~ 0
Q7
Text HLabel 5500 2350 2    60   Input ~ 0
DIN8
Text HLabel 5500 2250 2    60   Input ~ 0
DIN7
Text HLabel 5500 2150 2    60   Input ~ 0
DIN6
Text HLabel 5500 2050 2    60   Input ~ 0
DIN5
Text HLabel 5500 1950 2    60   Input ~ 0
DIN4
Text HLabel 5500 1850 2    60   Input ~ 0
DIN3
Text HLabel 5500 1750 2    60   Input ~ 0
DIN2
Text HLabel 5500 1650 2    60   Input ~ 0
DIN1
Text HLabel 5500 1550 2    60   Input ~ 0
DIN0
Text Label 5150 1550 0    60   ~ 0
DIN0
Text Label 5150 1650 0    60   ~ 0
DIN1
Text Label 5150 1750 0    60   ~ 0
DIN2
Text Label 5150 1850 0    60   ~ 0
DIN3
Text Label 5150 1950 0    60   ~ 0
DIN4
Text Label 5150 2050 0    60   ~ 0
DIN5
Text Label 5150 2150 0    60   ~ 0
DIN6
Text Label 5150 2250 0    60   ~ 0
DIN7
Text Label 5150 2350 0    60   ~ 0
DIN8
Text Label 3550 1950 0    60   ~ 0
DIN8
Text Label 3550 1850 0    60   ~ 0
DIN7
Text Label 3550 1750 0    60   ~ 0
DIN6
Text Label 3550 1650 0    60   ~ 0
DIN5
Text Label 3550 1550 0    60   ~ 0
DIN4
Text Label 3550 1450 0    60   ~ 0
DIN3
Text Label 3550 1350 0    60   ~ 0
DIN2
Text Label 3550 1250 0    60   ~ 0
DIN1
Text Label 3550 1150 0    60   ~ 0
DIN0
$Comp
L 74LS165 U3
U 1 1 4BF292EF
P 4500 1750
F 0 "U3" H 4650 1700 60  0000 C CNN
F 1 "74LS165" H 4650 1500 60  0000 C CNN
	1    4500 1750
	1    0    0    -1  
$EndComp
$Comp
L 74LS280 U4
U 1 1 4BF292E2
P 2800 1550
F 0 "U4" H 2900 1450 60  0000 C CNN
F 1 "74LS280" H 2900 1350 60  0000 C CNN
	1    2800 1550
	-1   0    0    -1  
$EndComp
$Comp
L 74HC595 U1
U 1 1 4BF292D0
P 3600 3500
F 0 "U1" H 3750 4100 70  0000 C CNN
F 1 "74HC595" H 3600 2900 70  0000 C CNN
	1    3600 3500
	1    0    0    -1  
$EndComp
$EndSCHEMATC
