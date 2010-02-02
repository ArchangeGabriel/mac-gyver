EESchema Schematic File Version 2  date mar. 08 déc. 2009 23:08:43 CET
LIBS:power,18f4550,optocoupler,rr4,st-microelectronics,device,transistors,conn,linear,regul,74xx,cmos4000,adc-dac,memory,xilinx,special,microcontrollers,dsp,microchip,analog_switches,motorola,texas,intel,audio,interface,digital-audio,philips,display,cypress,siliconi,opto,atmel,contrib,valves,./master.cache
EELAYER 24  0
EELAYER END
$Descr A4 11700 8267
Sheet 6 8
Title ""
Date "7 dec 2009"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Kmarq B 1850 3000 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 2100 "Attention: Pin power_in non pilotée (Net 19)" F=1
Text HLabel 6350 1650 0    60   Input ~ 0
PWR-
Text HLabel 6350 1550 0    60   Input ~ 0
PWRVDD
Text HLabel 6350 1450 0    60   Input ~ 0
PWR+
Text HLabel 8050 2500 2    60   Output ~ 0
PINMOT2-2
Text HLabel 8050 2300 2    60   Output ~ 0
PINMOT2-1
Text HLabel 8050 2050 2    60   Output ~ 0
PINMOT1-2
Text HLabel 8050 1850 2    60   Output ~ 0
PINMOT1-1
Text Label 5900 2450 0    60   ~ 0
MOT22
Text Label 5900 2350 0    60   ~ 0
MOT21
Wire Wire Line
	5900 2350 6450 2350
Wire Wire Line
	5900 2450 6450 2450
Text Label 5900 2000 0    60   ~ 0
MOT12
Text Label 5900 1900 0    60   ~ 0
MOT11
Wire Wire Line
	5900 1900 6450 1900
Wire Wire Line
	5900 2000 6450 2000
Text Label 1500 3700 0    60   ~ 0
MOT12
Text Label 1500 3600 0    60   ~ 0
MOT11
Text Label 4500 1400 2    60   ~ 0
MOT21
Text Label 4500 1300 2    60   ~ 0
MOT22
Wire Wire Line
	6450 1250 5900 1250
Wire Wire Line
	7950 2500 8050 2500
Wire Wire Line
	7950 2300 8050 2300
Wire Wire Line
	7950 2050 8050 2050
Wire Wire Line
	7950 1850 8050 1850
Wire Wire Line
	6450 1450 6350 1450
Wire Wire Line
	6450 1650 6350 1650
Wire Wire Line
	6450 1550 6350 1550
Text Label 5900 1250 0    60   ~ 0
GND
$Sheet
S 6450 1050 1500 1650
U 4B156757
F0 "2 Moteurs" 60
F1 "2moteurs.sch" 60
F2 "GND" I L 6450 1250 60 
F3 "PWR-" I L 6450 1650 60 
F4 "PWRVDD" I L 6450 1550 60 
F5 "PINMOT2-2" O R 7950 2500 60 
F6 "PINMOT2-1" O R 7950 2300 60 
F7 "PINMOT1-2" O R 7950 2050 60 
F8 "PINMOT1-1" O R 7950 1850 60 
F9 "SIGMOT2-2" I L 6450 2450 60 
F10 "SIGMOT2-1" I L 6450 2350 60 
F11 "SIGMOT1-2" I L 6450 2000 60 
F12 "SIGMOT1-1" I L 6450 1900 60 
F13 "PWR+" I L 6450 1450 60 
$EndSheet
Connection ~ 10150 6050
Wire Wire Line
	10150 6050 10150 5950
Connection ~ 10150 5450
Wire Wire Line
	10150 5450 10150 5550
Wire Wire Line
	7850 3750 7850 3650
Connection ~ 7850 3650
Wire Wire Line
	7400 4250 7850 4250
Wire Wire Line
	7850 4250 7850 4150
Wire Wire Line
	6700 6150 6400 6150
Wire Wire Line
	2050 5900 2300 5900
Wire Wire Line
	4500 2200 3950 2200
Wire Wire Line
	1500 2400 1850 2400
Wire Wire Line
	1850 1200 1500 1200
Wire Wire Line
	4500 1200 3950 1200
Wire Wire Line
	3950 3500 4550 3500
Wire Wire Line
	3950 3600 4550 3600
Wire Wire Line
	3950 3300 4550 3300
Wire Wire Line
	4550 3400 3950 3400
Wire Wire Line
	3950 3200 4550 3200
Wire Wire Line
	3950 3000 4550 3000
Wire Wire Line
	4550 3100 3950 3100
Wire Wire Line
	1450 2200 1850 2200
Wire Wire Line
	1500 3300 1850 3300
Wire Wire Line
	3950 1500 4550 1500
Wire Wire Line
	3950 1900 4550 1900
Wire Wire Line
	1450 3200 1850 3200
Wire Wire Line
	1850 3500 1500 3500
Wire Wire Line
	1450 3100 1850 3100
Wire Wire Line
	4550 1800 3950 1800
Wire Wire Line
	4550 1600 3950 1600
Wire Wire Line
	1500 3400 1850 3400
Wire Wire Line
	3950 1300 4500 1300
Wire Wire Line
	1500 3600 1850 3600
Wire Wire Line
	1450 1600 1850 1600
Wire Wire Line
	1450 1400 1850 1400
Wire Wire Line
	1850 1300 1450 1300
Wire Wire Line
	1850 1500 1450 1500
Wire Wire Line
	1450 2000 1850 2000
Wire Wire Line
	1500 3700 1850 3700
Wire Wire Line
	3950 1400 4500 1400
Wire Wire Line
	1500 2850 1850 2850
Wire Wire Line
	1850 2750 1500 2750
Wire Wire Line
	1850 2100 1450 2100
Wire Wire Line
	3950 3700 4550 3700
Wire Wire Line
	1500 1700 1850 1700
Wire Wire Line
	3950 1700 4550 1700
Wire Wire Line
	1450 1800 1850 1800
Wire Wire Line
	2800 5300 2300 5300
Wire Wire Line
	5400 5600 5050 5600
Wire Wire Line
	2300 5300 2300 5350
Wire Wire Line
	4600 5600 4650 5600
Wire Wire Line
	2300 5900 2300 5850
Wire Wire Line
	6200 5850 6200 5950
Wire Wire Line
	6200 5350 6200 5300
Connection ~ 6200 5900
Wire Wire Line
	5850 5900 6200 5900
Connection ~ 7650 6050
Wire Wire Line
	8450 5950 8450 6050
Connection ~ 8450 5450
Wire Wire Line
	8450 5550 8450 5450
Wire Wire Line
	7650 5550 7650 5450
Connection ~ 7650 5450
Wire Wire Line
	7650 5950 7650 6050
Wire Wire Line
	7350 5450 7750 5450
Wire Wire Line
	8750 5450 8350 5450
Wire Wire Line
	1500 2500 1850 2500
Wire Wire Line
	4500 2100 3950 2100
Wire Wire Line
	3850 5600 4100 5600
Wire Wire Line
	6200 5300 6700 5300
Wire Wire Line
	8450 6050 7350 6050
Wire Wire Line
	9550 5450 10700 5450
Wire Wire Line
	9550 6050 10700 6050
Wire Wire Line
	7400 4450 8500 4450
Wire Wire Line
	8500 4350 7400 4350
Wire Wire Line
	7400 3650 8150 3650
Wire Wire Line
	8150 4250 8150 4350
Connection ~ 8150 4350
Wire Wire Line
	8150 3650 8150 3750
Wire Wire Line
	9900 5450 9900 5550
Connection ~ 9900 5450
Wire Wire Line
	9900 5950 9900 6050
Connection ~ 9900 6050
$Comp
L CAPAPOL C9
U 1 1 4B13C074
P 9900 5750
F 0 "C9" H 9950 5850 50  0000 L CNN
F 1 "10uF" H 9950 5650 50  0000 L CNN
	1    9900 5750
	1    0    0    -1  
$EndComp
Text Notes 7800 3350 0    120  ~ 0
USB
Text Label 7400 4250 0    60   ~ 0
GND
Text Label 7400 3650 0    60   ~ 0
Vusb
Text HLabel 8500 4350 2    60   BiDi ~ 0
D+
Text Label 7400 4350 0    60   ~ 0
D+
Text HLabel 8500 4450 2    60   BiDi ~ 0
D-
Text Label 7400 4450 0    60   ~ 0
D-
Text Label 1500 3500 0    60   ~ 0
D+
Text Label 1500 3400 0    60   ~ 0
D-
Text Label 1500 3300 0    60   ~ 0
Vusb
$Comp
L C C1
U 1 1 4B13A96D
P 7850 3950
F 0 "C1" H 7900 4050 50  0000 L CNN
F 1 "100nF" H 7900 3850 50  0000 L CNN
	1    7850 3950
	-1   0    0    1   
$EndComp
$Comp
L R R1
U 1 1 4B13A96C
P 8150 4000
F 0 "R1" V 8230 4000 50  0000 C CNN
F 1 "1.5ko" V 8150 4000 50  0000 C CNN
	1    8150 4000
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 4B12C59B
P 10150 5750
F 0 "C2" H 10200 5850 50  0000 L CNN
F 1 "100nF" H 10200 5650 50  0000 L CNN
	1    10150 5750
	1    0    0    -1  
$EndComp
Text HLabel 1450 2000 0    60   Input ~ 0
AN5
Text HLabel 1450 1800 0    60   Input ~ 0
AN4
Text HLabel 1450 1600 0    60   Input ~ 0
AN3
Text HLabel 1450 1500 0    60   Input ~ 0
AN2
Text HLabel 1450 1400 0    60   Input ~ 0
AN1
Text HLabel 1450 1300 0    60   Input ~ 0
AN0
Text Notes 9250 5150 0    120  ~ 0
Alimentation
Text HLabel 9550 6050 0    60   Input ~ 0
GND
Text HLabel 9550 5450 0    60   Input ~ 0
VDD
Text Label 10700 6050 2    60   ~ 0
GND
Text Label 10700 5450 2    60   ~ 0
VDD
Text Label 7350 6050 0    60   ~ 0
GND
Text Label 6700 6150 2    60   ~ 0
GND
Text Label 6700 5300 2    60   ~ 0
VDD
Text Label 3850 5600 0    60   ~ 0
GND
Text Label 2050 5900 0    60   ~ 0
GND
Text Label 4500 2200 2    60   ~ 0
GND
Text Label 4500 2100 2    60   ~ 0
VDD
Text Label 1500 2400 0    60   ~ 0
VDD
Text Label 1500 2500 0    60   ~ 0
GND
Text Label 8750 5450 2    60   ~ 0
OSC2
Text Label 7350 5450 0    60   ~ 0
OSC1
Text Notes 7650 5150 0    120  ~ 0
Quartz
$Comp
L C C26
U 1 1 4B12A192
P 7650 5750
F 0 "C26" H 7700 5850 50  0000 L CNN
F 1 "22pF" H 7700 5650 50  0000 L CNN
	1    7650 5750
	1    0    0    -1  
$EndComp
$Comp
L C C27
U 1 1 4B12A191
P 8450 5750
F 0 "C27" H 8500 5850 50  0000 L CNN
F 1 "22pF" H 8500 5650 50  0000 L CNN
	1    8450 5750
	1    0    0    -1  
$EndComp
$Comp
L CRYSTAL X1
U 1 1 4B12A190
P 8050 5450
F 0 "X1" H 8050 5600 60  0000 C CNN
F 1 "CRYSTAL" H 8050 5300 60  0000 C CNN
	1    8050 5450
	1    0    0    -1  
$EndComp
Text Label 5850 5900 0    60   ~ 0
MCLR\
Text Notes 6050 5150 0    120  ~ 0
Reset
$Comp
L SW_PUSH_SMALL SW7
U 1 1 4B12A17B
P 6300 6050
F 0 "SW7" V 6450 6160 30  0000 C CNN
F 1 "SW_PUSH_SMALL" V 6250 5850 30  0000 C CNN
	1    6300 6050
	0    1    -1   0   
$EndComp
$Comp
L R R23
U 1 1 4B12A17A
P 6200 5600
F 0 "R23" V 6280 5600 50  0000 C CNN
F 1 "4.7ko" V 6200 5600 50  0000 C CNN
	1    6200 5600
	-1   0    0    -1  
$EndComp
Text Label 2800 5300 2    60   ~ 0
FORCEBL
Text Label 5400 5600 2    60   ~ 0
Debug
Text Notes 1100 5150 0    120  ~ 0
Pulldown Bootloader
Text Notes 4300 5150 0    120  ~ 0
Debug
$Comp
L R R2
U 1 1 4B12A16C
P 2300 5600
F 0 "R2" V 2380 5600 50  0000 C CNN
F 1 "10ko" V 2300 5600 50  0000 C CNN
	1    2300 5600
	1    0    0    -1  
$EndComp
$Comp
L R R49
U 1 1 4B12A16B
P 4350 5600
F 0 "R49" V 4430 5600 50  0000 C CNN
F 1 "470o" V 4350 5600 50  0000 C CNN
	1    4350 5600
	0    1    1    0   
$EndComp
$Comp
L LED D7
U 1 1 4B12A16A
P 4850 5600
F 0 "D7" H 4850 5700 50  0000 C CNN
F 1 "LED" H 4850 5500 50  0000 C CNN
	1    4850 5600
	-1   0    0    1   
$EndComp
Text HLabel 4550 1600 2    60   Input ~ 0
SENS1
Text HLabel 4550 1500 2    60   Input ~ 0
SENS2
Text HLabel 4550 1800 2    60   Input ~ 0
COD2
Text HLabel 4550 1900 2    60   Input ~ 0
COD1
Text HLabel 1450 3200 0    60   Output ~ 0
PWM1
Text HLabel 1450 3100 0    60   Output ~ 0
PWM2
Text HLabel 4550 1700 2    60   Input ~ 0
CALLSPP
Text Label 1500 1200 0    60   ~ 0
MCLR\
Text Label 4500 1200 2    60   ~ 0
FORCEBL
Text Label 1500 1700 0    60   ~ 0
Debug
Text Label 1500 2750 0    60   ~ 0
OSC1
Text Label 1500 2850 0    60   ~ 0
OSC2
Text HLabel 1450 2200 0    60   Output ~ 0
OESPP
Text HLabel 1450 2100 0    60   Output ~ 0
CK2SPP
Text HLabel 4550 3000 2    60   BiDi ~ 0
SPP7
Text HLabel 4550 3100 2    60   BiDi ~ 0
SPP6
Text HLabel 4550 3200 2    60   BiDi ~ 0
SPP5
Text HLabel 4550 3300 2    60   BiDi ~ 0
SPP4
Text HLabel 4550 3400 2    60   BiDi ~ 0
SPP3
Text HLabel 4550 3500 2    60   BiDi ~ 0
SPP2
Text HLabel 4550 3600 2    60   BiDi ~ 0
SPP1
Text HLabel 4550 3700 2    60   BiDi ~ 0
SPP0
$Comp
L PIC18F4550 U5
U 1 1 4B12A02F
P 2900 2550
F 0 "U5" H 2900 2650 70  0000 C CNN
F 1 "PIC18F4550" H 2900 2450 70  0000 C CNN
	1    2900 2550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
