EESchema Schematic File Version 2  date ven. 29 janv. 2010 20:01:37 CET
LIBS:power,relay,18f4550,optocoupler,rr4,st-microelectronics,device,transistors,conn,linear,regul,74xx,cmos4000,adc-dac,memory,xilinx,special,microcontrollers,dsp,microchip,analog_switches,motorola,texas,intel,audio,interface,digital-audio,philips,display,cypress,siliconi,opto,atmel,contrib,valves,./master.cache
EELAYER 24  0
EELAYER END
$Descr A4 11700 8267
Sheet 7 8
Title ""
Date "29 jan 2010"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 3700 5300 2    60   ~ 0
GND
Wire Wire Line
	3450 5300 3700 5300
Wire Wire Line
	4150 3600 3950 3600
Wire Wire Line
	4550 5600 4500 5600
Wire Wire Line
	1850 2100 1450 2100
Connection ~ 9900 6050
Wire Wire Line
	9900 5950 9900 6050
Connection ~ 9900 5450
Wire Wire Line
	9900 5450 9900 5550
Wire Wire Line
	8150 3750 8150 3650
Connection ~ 8150 4350
Wire Wire Line
	8150 4250 8150 4350
Wire Wire Line
	8150 3650 7400 3650
Wire Wire Line
	8500 4350 7400 4350
Wire Wire Line
	7400 4450 8500 4450
Wire Wire Line
	9550 6050 10700 6050
Wire Wire Line
	9550 5450 10700 5450
Wire Wire Line
	7350 6050 8450 6050
Wire Wire Line
	6700 5300 6200 5300
Wire Wire Line
	3850 5600 4100 5600
Wire Wire Line
	4500 2100 3950 2100
Wire Wire Line
	1500 2500 1850 2500
Wire Wire Line
	8750 5450 8350 5450
Wire Wire Line
	7350 5450 7750 5450
Wire Wire Line
	7650 5950 7650 6050
Connection ~ 7650 5450
Wire Wire Line
	7650 5550 7650 5450
Wire Wire Line
	8450 5550 8450 5450
Connection ~ 8450 5450
Wire Wire Line
	8450 6050 8450 5950
Connection ~ 7650 6050
Wire Wire Line
	5850 5900 6200 5900
Connection ~ 6200 5900
Wire Wire Line
	6200 5300 6200 5350
Wire Wire Line
	6200 5850 6200 5950
Wire Wire Line
	2300 5850 2300 5900
Wire Wire Line
	2300 5350 2300 5300
Wire Wire Line
	5400 5600 5050 5600
Wire Wire Line
	1450 1800 1850 1800
Wire Wire Line
	1500 1700 1850 1700
Wire Wire Line
	1850 2750 1500 2750
Wire Wire Line
	1500 2850 1850 2850
Wire Wire Line
	1450 2000 1850 2000
Wire Wire Line
	1850 1500 1450 1500
Wire Wire Line
	1850 1300 1450 1300
Wire Wire Line
	1450 1400 1850 1400
Wire Wire Line
	1450 1600 1850 1600
Wire Wire Line
	1500 3400 1850 3400
Wire Wire Line
	4550 1700 3950 1700
Wire Wire Line
	4550 1800 3950 1800
Wire Wire Line
	1450 3100 1850 3100
Wire Wire Line
	1850 3500 1500 3500
Wire Wire Line
	1450 3200 1850 3200
Wire Wire Line
	3950 1900 4550 1900
Wire Wire Line
	3950 1600 4550 1600
Wire Wire Line
	1500 3300 1850 3300
Wire Wire Line
	1850 1200 1500 1200
Wire Wire Line
	1500 2400 1850 2400
Wire Wire Line
	4500 2200 3950 2200
Wire Wire Line
	2300 5900 2050 5900
Wire Wire Line
	6700 6150 6400 6150
Wire Wire Line
	7850 4150 7850 4250
Wire Wire Line
	7850 4250 7400 4250
Connection ~ 7850 3650
Wire Wire Line
	7850 3750 7850 3650
Wire Wire Line
	10150 5450 10150 5550
Connection ~ 10150 5450
Wire Wire Line
	10150 6050 10150 5950
Connection ~ 10150 6050
Wire Wire Line
	1850 2200 1450 2200
Wire Wire Line
	3950 1200 4500 1200
Wire Wire Line
	2300 5300 2850 5300
$Comp
L SW_PUSH SW11
U 1 1 4B607380
P 3150 5300
F 0 "SW11" H 3300 5410 50  0000 C CNN
F 1 "SW_PUSH" H 3150 5220 50  0000 C CNN
	1    3150 5300
	1    0    0    -1  
$EndComp
Text HLabel 4150 3600 2    60   Output ~ 0
On1
Kmarq B 1850 3000 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 1850 3600 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 1300 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 1400 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 1500 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 3000 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 3100 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 3200 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 3300 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 3400 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 3500 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 3700 "Attention: Pin BiDi Non connectée" F=1
Kmarq B 3950 2100 "Attention: Pin power_in non pilotée (Net 44)" F=1
Text HLabel 1450 2200 0    60   Input ~ 0
AN7
Text HLabel 1450 2100 0    60   Input ~ 0
AN6
$Comp
L CAPAPOL C13
U 1 1 4B13C074
P 9900 5750
F 0 "C13" H 9950 5850 50  0000 L CNN
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
L C C11
U 1 1 4B13A96D
P 7850 3950
F 0 "C11" H 7900 4050 50  0000 L CNN
F 1 "100nF" H 7900 3850 50  0000 L CNN
	1    7850 3950
	-1   0    0    1   
$EndComp
$Comp
L R R22
U 1 1 4B13A96C
P 8150 4000
F 0 "R22" V 8230 4000 50  0000 C CNN
F 1 "1.5ko" V 8150 4000 50  0000 C CNN
	1    8150 4000
	1    0    0    -1  
$EndComp
$Comp
L C C14
U 1 1 4B12C59B
P 10150 5750
F 0 "C14" H 10200 5850 50  0000 L CNN
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
VDD
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
L C C10
U 1 1 4B12A192
P 7650 5750
F 0 "C10" H 7700 5850 50  0000 L CNN
F 1 "22pF" H 7700 5650 50  0000 L CNN
	1    7650 5750
	1    0    0    -1  
$EndComp
$Comp
L C C12
U 1 1 4B12A191
P 8450 5750
F 0 "C12" H 8500 5850 50  0000 L CNN
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
L SW_PUSH_SMALL SW4
U 1 1 4B12A17B
P 6300 6050
F 0 "SW4" V 6450 6160 30  0000 C CNN
F 1 "SW_PUSH_SMALL" V 6250 5850 30  0000 C CNN
	1    6300 6050
	0    1    -1   0   
$EndComp
$Comp
L R R21
U 1 1 4B12A17A
P 6200 5600
F 0 "R21" V 6280 5600 50  0000 C CNN
F 1 "10ko" V 6200 5600 50  0000 C CNN
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
L R R19
U 1 1 4B12A16C
P 2300 5600
F 0 "R19" V 2380 5600 50  0000 C CNN
F 1 "10ko" V 2300 5600 50  0000 C CNN
	1    2300 5600
	1    0    0    -1  
$EndComp
$Comp
L R R20
U 1 1 4B12A16B
P 4800 5600
F 0 "R20" V 4880 5600 50  0000 C CNN
F 1 "470o" V 4800 5600 50  0000 C CNN
	1    4800 5600
	0    1    1    0   
$EndComp
$Comp
L LED D3
U 1 1 4B12A16A
P 4300 5600
F 0 "D3" H 4300 5700 50  0000 C CNN
F 1 "LED" H 4300 5500 50  0000 C CNN
	1    4300 5600
	-1   0    0    1   
$EndComp
Text HLabel 4550 1700 2    60   Input ~ 0
SENS1
Text HLabel 4550 1600 2    60   Input ~ 0
SENS2
Text HLabel 4550 1800 2    60   Input ~ 0
COD2
Text HLabel 4550 1900 2    60   Input ~ 0
COD1
Text HLabel 1450 3200 0    60   Output ~ 0
PWM1
Text HLabel 1450 3100 0    60   Output ~ 0
PWM2
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
$Comp
L PIC18F4550 U2
U 1 1 4B12A02F
P 2900 2550
F 0 "U2" H 2900 2650 70  0000 C CNN
F 1 "PIC18F4550" H 2900 2450 70  0000 C CNN
	1    2900 2550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
