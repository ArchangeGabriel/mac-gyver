EESchema Schematic File Version 2  date ven. 29 janv. 2010 20:01:37 CET
LIBS:power,relay,18f4550,optocoupler,rr4,st-microelectronics,device,transistors,conn,linear,regul,74xx,cmos4000,adc-dac,memory,xilinx,special,microcontrollers,dsp,microchip,analog_switches,motorola,texas,intel,audio,interface,digital-audio,philips,display,cypress,siliconi,opto,atmel,contrib,valves,./master.cache
EELAYER 24  0
EELAYER END
$Descr A4 11700 8267
Sheet 2 8
Title ""
Date "29 jan 2010"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5450 4300 5750 4300
Wire Wire Line
	6350 3700 6500 3700
Wire Wire Line
	5850 5200 5550 5200
Wire Wire Line
	7300 3900 7400 3900
Wire Wire Line
	7400 4400 7400 3900
Wire Wire Line
	6750 4400 7400 4400
Wire Wire Line
	7300 5400 9050 5400
Wire Wire Line
	6950 4250 6850 4250
Connection ~ 6850 4400
Wire Wire Line
	6850 4250 6850 4400
Wire Wire Line
	4400 5850 4850 5850
Connection ~ 7350 1700
Wire Wire Line
	5900 1700 7400 1700
Wire Wire Line
	8450 1700 8200 1700
Wire Wire Line
	2650 2150 4950 2150
Wire Wire Line
	8600 3500 8850 3500
Wire Wire Line
	7600 3500 7500 3500
Wire Wire Line
	7500 3500 7500 3700
Wire Wire Line
	8100 3500 8200 3500
Connection ~ 7350 900 
Wire Wire Line
	7250 900  7400 900 
Wire Wire Line
	2650 1050 2750 1050
Wire Wire Line
	2750 1050 2750 1200
Wire Wire Line
	2750 1200 2650 1200
Connection ~ 2300 5450
Wire Wire Line
	2400 5450 2200 5450
Connection ~ 3850 950 
Wire Wire Line
	3850 2050 3850 2150
Wire Wire Line
	3850 1050 3850 950 
Wire Wire Line
	8450 900  8200 900 
Connection ~ 7350 1400
Wire Wire Line
	7800 1200 7800 1400
Wire Wire Line
	7350 950  7350 900 
Wire Wire Line
	7350 1400 7350 1350
Wire Wire Line
	8250 1400 8250 1350
Connection ~ 7800 1400
Wire Wire Line
	8250 950  8250 900 
Connection ~ 8250 900 
Wire Wire Line
	5900 1400 8250 1400
Wire Wire Line
	3850 1550 3850 1650
Connection ~ 3850 2150
Wire Wire Line
	2300 6550 2300 6650
Wire Wire Line
	2300 5550 2300 5450
Wire Wire Line
	2300 6050 2300 6150
Wire Wire Line
	2800 5450 3150 5450
Connection ~ 2300 6650
Connection ~ 7350 2200
Wire Wire Line
	7350 1750 7350 1700
Wire Wire Line
	7350 2200 7350 2150
Wire Wire Line
	8250 2200 8250 2150
Wire Wire Line
	8250 1750 8250 1700
Connection ~ 8250 1700
Wire Wire Line
	5900 2200 8250 2200
Wire Wire Line
	3000 5850 3600 5850
Connection ~ 3550 6350
Wire Wire Line
	4000 6350 4000 6150
Wire Wire Line
	3550 5900 3550 5850
Wire Wire Line
	3550 6350 3550 6300
Connection ~ 4000 6350
Connection ~ 3550 5850
Wire Wire Line
	1100 5350 1200 5350
Wire Wire Line
	3150 6650 1100 6650
Wire Wire Line
	3950 950  2650 950 
Wire Wire Line
	5900 900  6650 900 
Connection ~ 4450 6350
Wire Wire Line
	4450 6300 4450 6350
Connection ~ 4450 5850
Wire Wire Line
	4450 5900 4450 5850
Wire Wire Line
	4650 6350 3000 6350
Connection ~ 7500 3700
Wire Wire Line
	4950 950  4350 950 
Wire Wire Line
	7800 2200 7800 2000
Connection ~ 7800 2200
Wire Wire Line
	5750 4500 5500 4500
Wire Wire Line
	5750 4900 5500 4900
Wire Wire Line
	8600 5200 8850 5200
Wire Wire Line
	7600 5200 7500 5200
Wire Wire Line
	7500 5200 7500 5400
Wire Wire Line
	8100 5200 8200 5200
Connection ~ 7500 5400
Wire Wire Line
	6850 4650 6850 4800
Connection ~ 6850 4800
Wire Wire Line
	6850 4650 6950 4650
Wire Wire Line
	7300 5200 7400 5200
Wire Wire Line
	7400 4800 7400 5200
Wire Wire Line
	6750 4800 7400 4800
Wire Wire Line
	7300 3700 9050 3700
Wire Wire Line
	5550 5400 6500 5400
Wire Wire Line
	5550 3900 6500 3900
Wire Wire Line
	6500 5200 6350 5200
Wire Wire Line
	5550 3700 5850 3700
Wire Wire Line
	5450 4700 5750 4700
$Comp
L R R72
U 1 1 4B606B69
P 6100 5200
F 0 "R72" V 6180 5200 50  0000 C CNN
F 1 "1ko" V 6100 5200 50  0000 C CNN
	1    6100 5200
	0    1    1    0   
$EndComp
$Comp
L R R71
U 1 1 4B606B67
P 6100 3700
F 0 "R71" V 6180 3700 50  0000 C CNN
F 1 "1ko" V 6100 3700 50  0000 C CNN
	1    6100 3700
	0    1    1    0   
$EndComp
Text Label 5550 5400 0    60   ~ 0
GND
Text Label 5550 3900 0    60   ~ 0
GND
Text HLabel 5550 5200 0    60   Input ~ 0
On2
Text HLabel 5550 3700 0    60   Input ~ 0
On1
$Comp
L AQW21 K13
U 2 1 4B606463
P 6900 5300
F 0 "K13" H 6600 5550 50  0000 L BNN
F 1 "AQW21" H 6600 4980 50  0000 L BNN
F 2 "relay-DIL08" H 6900 5450 50  0001 C CNN
	2    6900 5300
	1    0    0    -1  
$EndComp
$Comp
L AQW21 K13
U 1 1 4B60645E
P 6900 3800
F 0 "K13" H 6600 4050 50  0000 L BNN
F 1 "AQW21" H 6600 3480 50  0000 L BNN
F 2 "relay-DIL08" H 6900 3950 50  0001 C CNN
	1    6900 3800
	1    0    0    -1  
$EndComp
Kmarq B 2200 5250 "Attention: Pin passive Non connectée" F=1
Text HLabel 6950 4650 2    60   Output ~ 0
Vpic2
Text HLabel 6950 4250 2    60   Output ~ 0
Vpic1
Text Label 8850 5200 2    60   ~ 0
GND
Text HLabel 9050 5400 2    60   Output ~ 0
VDD2
$Comp
L R R46
U 1 1 4B2A61E0
P 7850 5200
F 0 "R46" V 7930 5200 50  0000 C CNN
F 1 "470o" V 7850 5200 50  0000 C CNN
	1    7850 5200
	0    1    1    0   
$EndComp
$Comp
L LED D8
U 1 1 4B2A61DF
P 8400 5200
F 0 "D8" H 8400 5300 50  0000 C CNN
F 1 "LED" H 8400 5100 50  0000 C CNN
	1    8400 5200
	1    0    0    -1  
$EndComp
Text Label 5500 4900 0    60   ~ 0
5v
$Comp
L SWITCH_INV CHGALIM2
U 1 1 4B2A615E
P 6250 4800
F 0 "CHGALIM2" H 6050 4950 50  0000 C CNN
F 1 "SWITCH_INV" H 6100 4650 50  0000 C CNN
	1    6250 4800
	-1   0    0    1   
$EndComp
Text HLabel 5450 4700 0    60   Input ~ 0
Vusb2
Text Label 5500 4500 0    60   ~ 0
5v
Text Label 4850 5850 2    60   ~ 0
5v
$Comp
L SW_PUSH SW6
U 1 1 4B1CECF0
P 6950 900
F 0 "SW6" H 7100 1010 50  0000 C CNN
F 1 "SW_PUSH" H 6950 820 50  0000 C CNN
	1    6950 900 
	1    0    0    -1  
$EndComp
Text HLabel 4950 2150 2    60   Output ~ 0
PWR-
Text HLabel 4950 950  2    60   Output ~ 0
PWR+
Text Label 8850 3500 2    60   ~ 0
GND
Text HLabel 9050 3700 2    60   Output ~ 0
VDD1
$Comp
L R R45
U 1 1 4B13BDC4
P 7850 3500
F 0 "R45" V 7930 3500 50  0000 C CNN
F 1 "470o" V 7850 3500 50  0000 C CNN
	1    7850 3500
	0    1    1    0   
$EndComp
$Comp
L LED D7
U 1 1 4B13BDC3
P 8400 3500
F 0 "D7" H 8400 3600 50  0000 C CNN
F 1 "LED" H 8400 3400 50  0000 C CNN
	1    8400 3500
	1    0    0    -1  
$EndComp
Text HLabel 4650 6350 2    60   Output ~ 0
GND
$Comp
L SWITCH_INV CHGALIM1
U 1 1 4B13B5B3
P 6250 4400
F 0 "CHGALIM1" H 6050 4550 50  0000 C CNN
F 1 "SWITCH_INV" H 6100 4250 50  0000 C CNN
	1    6250 4400
	-1   0    0    1   
$EndComp
Text HLabel 5450 4300 0    60   Input ~ 0
Vusb1
Text HLabel 2650 950  0    60   Input ~ 0
URG2
Text HLabel 2650 1050 0    60   Output ~ 0
URG1
$Comp
L SWITCH_INV SWCMD1
U 1 1 4B13B235
P 1700 5350
F 0 "SWCMD1" H 1500 5500 50  0000 C CNN
F 1 "SWITCH_INV" H 1550 5200 50  0000 C CNN
	1    1700 5350
	1    0    0    -1  
$EndComp
Text Label 3000 6350 0    60   ~ 0
GND
Text Label 3000 5850 0    60   ~ 0
CMD+
$Comp
L C C22
U 1 1 4B13B114
P 4450 6100
F 0 "C22" H 4500 6200 50  0000 L CNN
F 1 "100nF" H 4500 6000 50  0000 L CNN
	1    4450 6100
	-1   0    0    -1  
$EndComp
$Comp
L C C21
U 1 1 4B13B113
P 3550 6100
F 0 "C21" H 3600 6200 50  0000 L CNN
F 1 "220nF" H 3600 6000 50  0000 L CNN
	1    3550 6100
	1    0    0    -1  
$EndComp
$Comp
L 7805 U4
U 1 1 4B13B112
P 4000 5850
F 0 "U4" H 4150 5654 60  0000 C CNN
F 1 "7805" H 4000 6050 60  0000 C CNN
	1    4000 5850
	1    0    0    -1  
$EndComp
Text HLabel 8450 1700 2    60   Output ~ 0
PWRVDDMOT
Text Label 5900 2200 0    60   ~ 0
PWR-
Text Label 5900 1700 0    60   ~ 0
PWR+
$Comp
L C C26
U 1 1 4B13B0E2
P 8250 1950
F 0 "C26" H 8300 2050 50  0000 L CNN
F 1 "100nF" H 8300 1850 50  0000 L CNN
	1    8250 1950
	-1   0    0    -1  
$EndComp
$Comp
L C C24
U 1 1 4B13B0E1
P 7350 1950
F 0 "C24" H 7400 2050 50  0000 L CNN
F 1 "220nF" H 7400 1850 50  0000 L CNN
	1    7350 1950
	1    0    0    -1  
$EndComp
$Comp
L 7805 U6
U 1 1 4B13B0E0
P 7800 1700
F 0 "U6" H 7950 1504 60  0000 C CNN
F 1 "7805" H 7800 1900 60  0000 C CNN
	1    7800 1700
	1    0    0    -1  
$EndComp
Text Label 3150 6650 2    60   ~ 0
GND
Text Label 3150 5450 2    60   ~ 0
CMD+
Text HLabel 1100 6650 0    60   Input ~ 0
CMDBATT-
Text HLabel 1100 5350 0    60   Input ~ 0
CMDBATT+
$Comp
L R R43
U 1 1 4B13B05E
P 2300 5800
F 0 "R43" V 2380 5800 50  0000 C CNN
F 1 "1ko" V 2300 5800 50  0000 C CNN
	1    2300 5800
	1    0    0    -1  
$EndComp
$Comp
L LED D5
U 1 1 4B13B05D
P 2300 6350
F 0 "D5" H 2300 6450 50  0000 C CNN
F 1 "LED" H 2300 6250 50  0000 C CNN
	1    2300 6350
	0    1    1    0   
$EndComp
$Comp
L DIODE DAC1
U 1 1 4B13B05C
P 2600 5450
F 0 "DAC1" H 2600 5550 40  0000 C CNN
F 1 "DIODE" H 2600 5350 40  0000 C CNN
	1    2600 5450
	1    0    0    -1  
$EndComp
Text Label 4700 2150 2    60   ~ 0
PWR-
Text Label 4700 950  2    60   ~ 0
PWR+
Text HLabel 2650 2150 0    60   Input ~ 0
PWRBATT-
Text HLabel 2650 1200 0    60   Input ~ 0
PWRBATT+
Text HLabel 8450 900  2    60   Output ~ 0
PWRVDDSVO
Text Label 5900 1400 0    60   ~ 0
PWR-
Text Label 5900 900  0    60   ~ 0
PWR+
$Comp
L C C25
U 1 1 4B13AD5B
P 8250 1150
F 0 "C25" H 8300 1250 50  0000 L CNN
F 1 "100nF" H 8300 1050 50  0000 L CNN
	1    8250 1150
	-1   0    0    -1  
$EndComp
$Comp
L C C23
U 1 1 4B13AD5A
P 7350 1150
F 0 "C23" H 7400 1250 50  0000 L CNN
F 1 "220nF" H 7400 1050 50  0000 L CNN
	1    7350 1150
	1    0    0    -1  
$EndComp
$Comp
L 7805 U5
U 1 1 4B13AD59
P 7800 900
F 0 "U5" H 7950 704 60  0000 C CNN
F 1 "7805" H 7800 1100 60  0000 C CNN
	1    7800 900 
	1    0    0    -1  
$EndComp
$Comp
L R R44
U 1 1 4B13AD58
P 3850 1300
F 0 "R44" V 3930 1300 50  0000 C CNN
F 1 "1ko" V 3850 1300 50  0000 C CNN
	1    3850 1300
	1    0    0    -1  
$EndComp
$Comp
L LED D6
U 1 1 4B13AD57
P 3850 1850
F 0 "D6" H 3850 1950 50  0000 C CNN
F 1 "LED" H 3850 1750 50  0000 C CNN
	1    3850 1850
	0    1    1    0   
$EndComp
$Comp
L DIODE DAC2
U 1 1 4B13AD56
P 4150 950
F 0 "DAC2" H 4150 1050 40  0000 C CNN
F 1 "DIODE" H 4150 850 40  0000 C CNN
	1    4150 950 
	1    0    0    -1  
$EndComp
$EndSCHEMATC
