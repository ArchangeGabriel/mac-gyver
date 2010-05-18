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
Sheet 5 7
Title ""
Date "18 may 2010"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 3250 1750
Wire Wire Line
	3650 1750 3250 1750
Wire Wire Line
	3250 1750 2750 1750
Wire Wire Line
	2750 1750 2250 1750
Wire Wire Line
	2250 1750 1400 1750
Connection ~ 2250 1750
Wire Wire Line
	2750 1750 2750 1450
Connection ~ 3250 1150
Wire Wire Line
	3250 1150 3250 1250
Connection ~ 2250 1150
Wire Wire Line
	2250 1150 2250 1250
Wire Wire Line
	1400 1150 1550 1150
Wire Wire Line
	4700 2900 3400 2900
Wire Wire Line
	3400 2900 3300 2900
Connection ~ 3400 2900
Wire Wire Line
	3400 2900 3400 3100
Wire Wire Line
	3400 3100 3450 3100
Wire Wire Line
	1550 2900 2600 2900
Wire Wire Line
	2000 2700 1550 2700
Wire Wire Line
	2500 2700 2600 2700
Wire Wire Line
	4700 2700 3300 2700
Wire Wire Line
	4700 3100 3950 3100
Wire Wire Line
	1550 3550 2600 3550
Wire Wire Line
	2000 3350 1550 3350
Wire Wire Line
	2500 3350 2600 3350
Wire Wire Line
	4700 3550 3400 3550
Wire Wire Line
	3400 3550 3300 3550
Connection ~ 3400 3550
Wire Wire Line
	3400 3550 3400 3750
Wire Wire Line
	3400 3750 3450 3750
Wire Wire Line
	4700 3350 3300 3350
Wire Wire Line
	4700 3750 3950 3750
Wire Wire Line
	1550 4200 2600 4200
Wire Wire Line
	2000 4000 1550 4000
Wire Wire Line
	2500 4000 2600 4000
Wire Wire Line
	1550 4850 2600 4850
Wire Wire Line
	2000 4650 1550 4650
Wire Wire Line
	2500 4650 2600 4650
Wire Wire Line
	4700 4200 3400 4200
Wire Wire Line
	3400 4200 3300 4200
Connection ~ 3400 4200
Wire Wire Line
	3400 4200 3400 4400
Wire Wire Line
	3400 4400 3450 4400
Wire Wire Line
	4700 4000 3300 4000
Wire Wire Line
	4700 4400 3950 4400
Wire Wire Line
	4700 4850 3400 4850
Wire Wire Line
	3400 4850 3300 4850
Connection ~ 3400 4850
Wire Wire Line
	3400 4850 3400 5050
Wire Wire Line
	3400 5050 3450 5050
Wire Wire Line
	4700 4650 3300 4650
Wire Wire Line
	4700 5050 3950 5050
Wire Wire Line
	1400 1950 3650 1950
Wire Wire Line
	2150 1150 2250 1150
Wire Wire Line
	2250 1150 2350 1150
Wire Wire Line
	3150 1150 3250 1150
Wire Wire Line
	3250 1150 3650 1150
Wire Wire Line
	2250 1750 2250 1650
Wire Wire Line
	3250 1750 3250 1650
Connection ~ 2750 1750
$Comp
L C C5
U 1 1 4BF29AE5
P 3250 1450
F 0 "C5" H 3300 1550 50  0000 L CNN
F 1 "100nF" H 3300 1350 50  0000 L CNN
	1    3250 1450
	1    0    0    -1  
$EndComp
$Comp
L 7805 U5
U 1 1 4BF29AD0
P 2750 1200
F 0 "U5" H 2900 1004 60  0000 C CNN
F 1 "7805" H 2750 1400 60  0000 C CNN
	1    2750 1200
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW6
U 1 1 4BF29AC1
P 1850 1150
F 0 "SW6" H 2000 1260 50  0000 C CNN
F 1 "SW_PUSH" H 1850 1070 50  0000 C CNN
	1    1850 1150
	1    0    0    -1  
$EndComp
$Comp
L C C14
U 1 1 4B139C43
P 2250 1450
F 0 "C14" H 2300 1550 50  0000 L CNN
F 1 "220nF" H 2300 1350 50  0000 L CNN
	1    2250 1450
	1    0    0    -1  
$EndComp
Text Notes 1000 900  0    120  ~ 0
Alimentation
Text HLabel 1400 1950 0    60   Input ~ 0
GND
Text HLabel 1400 1750 0    60   Input ~ 0
PWR-
Text HLabel 1400 1150 0    60   Input ~ 0
PWR+
Text Label 3650 1950 2    60   ~ 0
GND
Text Label 3650 1750 2    60   ~ 0
PWR-
Text Label 3650 1150 2    60   ~ 0
PWRVDD
Text HLabel 4700 4200 2    60   Output ~ 0
PINSVO3SIG
Text HLabel 4700 5050 2    60   Output ~ 0
PINSVO4-
Text HLabel 4700 4850 2    60   Output ~ 0
PINSVO4SIG
Text Label 4450 5050 2    60   ~ 0
PWR-
Text Label 4450 4650 2    60   ~ 0
PWRVDD
$Comp
L R R44
U 1 1 4B139A7E
P 3700 5050
F 0 "R44" V 3780 5050 50  0000 C CNN
F 1 "4.7ko" V 3700 5050 50  0000 C CNN
	1    3700 5050
	0    -1   1    0   
$EndComp
Text HLabel 4700 4650 2    60   Output ~ 0
PINSVO4+
Text HLabel 4700 4400 2    60   Output ~ 0
PINSVO3-
Text Label 4450 4400 2    60   ~ 0
PWR-
Text Label 4450 4000 2    60   ~ 0
PWRVDD
$Comp
L R R43
U 1 1 4B139A7D
P 3700 4400
F 0 "R43" V 3780 4400 50  0000 C CNN
F 1 "4.7ko" V 3700 4400 50  0000 C CNN
	1    3700 4400
	0    -1   1    0   
$EndComp
Text HLabel 4700 4000 2    60   Output ~ 0
PINSVO3+
Text HLabel 1550 4650 0    60   Input ~ 0
SIGSVO4
Text Label 1550 4850 0    60   ~ 0
GND
$Comp
L R R64
U 1 1 4B139A76
P 2250 4650
F 0 "R64" V 2330 4650 50  0000 C CNN
F 1 "470o" V 2250 4650 50  0000 C CNN
	1    2250 4650
	0    1    1    0   
$EndComp
Text HLabel 1550 4000 0    60   Input ~ 0
SIGSVO3
Text Label 1550 4200 0    60   ~ 0
GND
$Comp
L R R63
U 1 1 4B139A75
P 2250 4000
F 0 "R63" V 2330 4000 50  0000 C CNN
F 1 "470o" V 2250 4000 50  0000 C CNN
	1    2250 4000
	0    1    1    0   
$EndComp
Text HLabel 4700 3750 2    60   Output ~ 0
PINSVO2-
Text HLabel 4700 3550 2    60   Output ~ 0
PINSVO2SIG
Text Label 4450 3750 2    60   ~ 0
PWR-
Text Label 4450 3350 2    60   ~ 0
PWRVDD
$Comp
L R R42
U 1 1 4B139A24
P 3700 3750
F 0 "R42" V 3780 3750 50  0000 C CNN
F 1 "4.7ko" V 3700 3750 50  0000 C CNN
	1    3700 3750
	0    -1   1    0   
$EndComp
Text HLabel 4700 3350 2    60   Output ~ 0
PINSVO2+
Text HLabel 1550 3350 0    60   Input ~ 0
SIGSVO2
Text Label 1550 3550 0    60   ~ 0
GND
$Comp
L R R62
U 1 1 4B139A1A
P 2250 3350
F 0 "R62" V 2330 3350 50  0000 C CNN
F 1 "470o" V 2250 3350 50  0000 C CNN
	1    2250 3350
	0    1    1    0   
$EndComp
Text HLabel 4700 3100 2    60   Output ~ 0
PINSVO1-
Text HLabel 4700 2900 2    60   Output ~ 0
PINSVO1SIG
Text Label 4450 3100 2    60   ~ 0
PWR-
Text Label 4450 2700 2    60   ~ 0
PWRVDD
$Comp
L R R41
U 1 1 4B139928
P 3700 3100
F 0 "R41" V 3780 3100 50  0000 C CNN
F 1 "4.7ko" V 3700 3100 50  0000 C CNN
	1    3700 3100
	0    -1   1    0   
$EndComp
Text HLabel 1550 2700 0    60   Input ~ 0
SIGSVO1
Text Label 1550 2900 0    60   ~ 0
GND
$Comp
L R R61
U 1 1 4B139853
P 2250 2700
F 0 "R61" V 2330 2700 50  0000 C CNN
F 1 "470o" V 2250 2700 50  0000 C CNN
	1    2250 2700
	0    1    1    0   
$EndComp
Text HLabel 4700 2700 2    60   Output ~ 0
PINSVO1+
$Comp
L PC847 OK?
U 3 1 4B1397B3
P 3000 4100
AR Path="/4B0EC3AE/490B3848" Ref="OK?"  Part="3" 
AR Path="/4B139788/4B1397B3" Ref="OK?"  Part="3" 
AR Path="/4BF1871B/4B1397B3" Ref="OK3"  Part="3" 
F 0 "OK3" H 2750 4250 50  0000 L BNN
F 1 "PC847" H 2750 5150 50  0000 L BNN
F 2 "optocoupler-2-DIL16" H 3000 4250 50  0001 C CNN
	3    3000 4100
	1    0    0    -1  
$EndComp
$Comp
L PC847 OK?
U 4 1 4B1397AD
P 3000 2800
AR Path="/4B0EC3AE/490B384A" Ref="OK?"  Part="1" 
AR Path="/4B139788/4B1397AD" Ref="OK?"  Part="4" 
AR Path="/4BF1871B/4B1397AD" Ref="OK3"  Part="4" 
F 0 "OK3" H 2750 2950 50  0000 L BNN
F 1 "PC847" H 2750 3050 50  0000 L BNN
F 2 "optocoupler-2-DIL16" H 3000 2950 50  0001 C CNN
	4    3000 2800
	1    0    0    -1  
$EndComp
$Comp
L PC847 OK?
U 1 1 4B1397AC
P 3000 3450
AR Path="/4B0EC3AE/490B3849" Ref="OK?"  Part="2" 
AR Path="/4B139788/4B1397AC" Ref="OK?"  Part="1" 
AR Path="/4BF1871B/4B1397AC" Ref="OK3"  Part="1" 
F 0 "OK3" H 2750 3600 50  0000 L BNN
F 1 "PC847" H 2750 4100 50  0000 L BNN
F 2 "optocoupler-2-DIL16" H 3000 3600 50  0001 C CNN
	1    3000 3450
	1    0    0    -1  
$EndComp
$Comp
L PC847 OK?
U 2 1 4B1397AB
P 3000 4750
AR Path="/4B0EC3AE/490B3847" Ref="OK?"  Part="4" 
AR Path="/4B139788/4B1397AB" Ref="OK?"  Part="2" 
AR Path="/4BF1871B/4B1397AB" Ref="OK3"  Part="2" 
F 0 "OK3" H 2750 4900 50  0000 L BNN
F 1 "PC847" H 2750 4450 50  0000 L BNN
F 2 "optocoupler-2-DIL16" H 3000 4900 50  0001 C CNN
	2    3000 4750
	1    0    0    -1  
$EndComp
$EndSCHEMATC
