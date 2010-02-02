EESchema Schematic File Version 2  date ven. 29 janv. 2010 20:01:37 CET
LIBS:power,relay,18f4550,optocoupler,rr4,st-microelectronics,device,transistors,conn,linear,regul,74xx,cmos4000,adc-dac,memory,xilinx,special,microcontrollers,dsp,microchip,analog_switches,motorola,texas,intel,audio,interface,digital-audio,philips,display,cypress,siliconi,opto,atmel,contrib,valves,./master.cache
EELAYER 24  0
EELAYER END
$Descr A4 11700 8267
Sheet 3 8
Title ""
Date "29 jan 2010"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 1850 1750
Wire Wire Line
	1850 1650 1850 1750
Wire Wire Line
	1400 1950 2550 1950
Wire Wire Line
	1400 1750 2550 1750
Wire Wire Line
	1400 1150 2550 1150
Wire Wire Line
	4700 5050 3950 5050
Wire Wire Line
	4700 4650 3300 4650
Wire Wire Line
	3450 5050 3400 5050
Wire Wire Line
	3400 5050 3400 4850
Connection ~ 3400 4850
Wire Wire Line
	4700 4850 3300 4850
Wire Wire Line
	4700 4400 3950 4400
Wire Wire Line
	4700 4000 3300 4000
Wire Wire Line
	3450 4400 3400 4400
Wire Wire Line
	3400 4400 3400 4200
Connection ~ 3400 4200
Wire Wire Line
	4700 4200 3300 4200
Wire Wire Line
	2500 4650 2600 4650
Wire Wire Line
	2000 4650 1550 4650
Wire Wire Line
	1550 4850 2600 4850
Wire Wire Line
	2500 4000 2600 4000
Wire Wire Line
	2000 4000 1550 4000
Wire Wire Line
	1550 4200 2600 4200
Wire Wire Line
	4700 3750 3950 3750
Wire Wire Line
	4700 3350 3300 3350
Wire Wire Line
	3450 3750 3400 3750
Wire Wire Line
	3400 3750 3400 3550
Connection ~ 3400 3550
Wire Wire Line
	4700 3550 3300 3550
Wire Wire Line
	2500 3350 2600 3350
Wire Wire Line
	2000 3350 1550 3350
Wire Wire Line
	1550 3550 2600 3550
Wire Wire Line
	4700 3100 3950 3100
Wire Wire Line
	4700 2700 3300 2700
Wire Wire Line
	2500 2700 2600 2700
Wire Wire Line
	2000 2700 1550 2700
Wire Wire Line
	1550 2900 2600 2900
Wire Wire Line
	3450 3100 3400 3100
Wire Wire Line
	3400 3100 3400 2900
Connection ~ 3400 2900
Wire Wire Line
	4700 2900 3300 2900
Wire Wire Line
	1850 1250 1850 1150
Connection ~ 1850 1150
$Comp
L C C20
U 1 1 4B139C43
P 1850 1450
F 0 "C20" H 1900 1550 50  0000 L CNN
F 1 "220nF" H 1900 1350 50  0000 L CNN
	1    1850 1450
	1    0    0    -1  
$EndComp
Text Notes 1000 900  0    120  ~ 0
Alimentation
Text HLabel 1400 1950 0    60   Input ~ 0
GND
Text HLabel 1400 1750 0    60   Input ~ 0
PWR-
Text HLabel 1400 1150 0    60   Input ~ 0
PWRVDD
Text Label 2550 1950 2    60   ~ 0
GND
Text Label 2550 1750 2    60   ~ 0
PWR-
Text Label 2550 1150 2    60   ~ 0
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
L R R42
U 1 1 4B139A7E
P 3700 5050
F 0 "R42" V 3780 5050 50  0000 C CNN
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
L R R41
U 1 1 4B139A7D
P 3700 4400
F 0 "R41" V 3780 4400 50  0000 C CNN
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
L R R38
U 1 1 4B139A76
P 2250 4650
F 0 "R38" V 2330 4650 50  0000 C CNN
F 1 "470o" V 2250 4650 50  0000 C CNN
	1    2250 4650
	0    1    1    0   
$EndComp
Text HLabel 1550 4000 0    60   Input ~ 0
SIGSVO3
Text Label 1550 4200 0    60   ~ 0
GND
$Comp
L R R37
U 1 1 4B139A75
P 2250 4000
F 0 "R37" V 2330 4000 50  0000 C CNN
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
L R R40
U 1 1 4B139A24
P 3700 3750
F 0 "R40" V 3780 3750 50  0000 C CNN
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
L R R36
U 1 1 4B139A1A
P 2250 3350
F 0 "R36" V 2330 3350 50  0000 C CNN
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
L R R39
U 1 1 4B139928
P 3700 3100
F 0 "R39" V 3780 3100 50  0000 C CNN
F 1 "4.7ko" V 3700 3100 50  0000 C CNN
	1    3700 3100
	0    -1   1    0   
$EndComp
Text HLabel 1550 2700 0    60   Input ~ 0
SIGSVO1
Text Label 1550 2900 0    60   ~ 0
GND
$Comp
L R R35
U 1 1 4B139853
P 2250 2700
F 0 "R35" V 2330 2700 50  0000 C CNN
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
AR Path="/4B139788/4B1397B3" Ref="OK2"  Part="3" 
F 0 "OK2" H 2750 4250 50  0000 L BNN
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
AR Path="/4B139788/4B1397AD" Ref="OK2"  Part="4" 
F 0 "OK2" H 2750 2950 50  0000 L BNN
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
AR Path="/4B139788/4B1397AC" Ref="OK2"  Part="1" 
F 0 "OK2" H 2750 3600 50  0000 L BNN
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
AR Path="/4B139788/4B1397AB" Ref="OK2"  Part="2" 
F 0 "OK2" H 2750 4900 50  0000 L BNN
F 1 "PC847" H 2750 4450 50  0000 L BNN
F 2 "optocoupler-2-DIL16" H 3000 4900 50  0001 C CNN
	2    3000 4750
	1    0    0    -1  
$EndComp
$EndSCHEMATC
