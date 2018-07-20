//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Programm      : HE 098 EA-Card Bootloader
// Dateiname     : CRC.h
// Funktionen    : Header für CRC-Berechnung für Modbus
// Pro.-Umgebung : emIDE Version 2.12b
// Zielsystem    : ADUCM360
// Firma         : Hesch Schroeder GmbH  31535 Neustadt, Boschstr. 8
// Autor         : H.-W. Zinke
// Revision      : V1.00  22.11.13  Erstellung
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned short calculate_crc16(unsigned char *Message_ptr,unsigned short Data_Lenght);

// ########################################################################################################
