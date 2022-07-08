EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Camera"
Date "2022-07-08"
Rev "v0.1"
Comp "Spacelab"
Comment1 "Author: Vitória Beatriz Bianchin - Email: vitoriabbianchin@gmail.com"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_ST_STM32F1:STM32F103C8Tx U1
U 1 1 62BE24C7
P 2050 2650
F 0 "U1" H 2000 1061 50  0000 C CNN
F 1 "STM32F103C8Tx" H 2000 970 50  0000 C CNN
F 2 "Package_QFP:LQFP-48_7x7mm_P0.5mm" H 1450 1250 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00161566.pdf" H 2050 2650 50  0001 C CNN
	1    2050 2650
	1    0    0    -1  
$EndComp
$Comp
L Memory_Flash:W25Q128JVS U?
U 1 1 62C733A5
P 4700 2150
F 0 "U?" H 4700 2731 50  0000 C CNN
F 1 "W25Q128JVS" H 4700 2640 50  0000 C CNN
F 2 "Package_SO:SOIC-8_5.23x5.23mm_P1.27mm" H 4700 2150 50  0001 C CNN
F 3 "http://www.winbond.com/resource-files/w25q128jv_dtr%20revc%2003272018%20plus.pdf" H 4700 2150 50  0001 C CNN
	1    4700 2150
	1    0    0    -1  
$EndComp
$Comp
L Power_Management:TPS2044D U?
U 1 1 62C743E5
P 6250 3000
F 0 "U?" H 6250 3467 50  0000 C CNN
F 1 "TPS2044D" H 6250 3376 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 6250 3500 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tps2044.pdf" H 6200 3300 50  0001 C CNN
	1    6250 3000
	1    0    0    -1  
$EndComp
$Comp
L Interface_CAN_LIN:TCAN330G U?
U 1 1 62C754E5
P 5650 4500
F 0 "U?" H 5650 5081 50  0000 C CNN
F 1 "TCAN330G" H 5650 4990 50  0000 C CNN
F 2 "" H 5650 4000 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/tcan337.pdf" H 5650 4500 50  0001 C CNN
	1    5650 4500
	1    0    0    -1  
$EndComp
$Comp
L PicoBlade_532610671:532610671 J?
U 1 1 62C8D980
P 3500 2900
F 0 "J?" H 4028 2603 60  0000 L CNN
F 1 "532610671" H 4028 2497 60  0000 L CNN
F 2 "CON_532610671" H 3900 2440 60  0001 C CNN
F 3 "" H 3500 2900 60  0000 C CNN
	1    3500 2900
	1    0    0    -1  
$EndComp
$Comp
L 2022-07-08_17-15-54:532610371 J?
U 1 1 62C86F68
P 6150 1350
F 0 "J?" H 6678 1203 60  0000 L CNN
F 1 "532610371" H 6678 1097 60  0000 L CNN
F 2 "CON_532610371" H 6550 1090 60  0001 C CNN
F 3 "" H 6150 1350 60  0000 C CNN
	1    6150 1350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 62C89291
P 4900 3150
F 0 "J?" H 4980 3142 50  0000 L CNN
F 1 "Conn_01x04" H 4980 3051 50  0000 L CNN
F 2 "" H 4900 3150 50  0001 C CNN
F 3 "~" H 4900 3150 50  0001 C CNN
	1    4900 3150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J?
U 1 1 62C89A87
P 4350 4500
F 0 "J?" H 4430 4492 50  0000 L CNN
F 1 "Conn_01x08" H 4430 4401 50  0000 L CNN
F 2 "" H 4350 4500 50  0001 C CNN
F 3 "~" H 4350 4500 50  0001 C CNN
	1    4350 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal_GND24 Y?
U 1 1 62C8D4FE
P 3300 4350
F 0 "Y?" H 3494 4396 50  0000 L CNN
F 1 "Crystal_GND24" H 3494 4305 50  0000 L CNN
F 2 "" H 3300 4350 50  0001 C CNN
F 3 "~" H 3300 4350 50  0001 C CNN
	1    3300 4350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
