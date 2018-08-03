/*
 * Copyright:      Jan Homann
 * Author:         Jan Homann
 * Version:        1.0
 * Description:    Funktion zum auslesen eines "Optischen Drehencoder"
 */



#ifndef F_CPU
#define F_CPU 8000000
#endif


#define Encoder_DDR			DDRD
#define Encoder_PORT		PIND
#define Encoder_PHASE_A		0
#define Encoder_PHASE_B		1


/**
 @brief    Initalisiert die benötigten Ports / Pins
 @param    keine                                         
 @return   keinen 
*/
void init_Encoder(void);


/**
 @brief    Funktion zum auslesen eines Optischen Drehencoders ( PORT als "Eingang" definieren! )
 @param    PinPORT (z.B PINA), BinaryNumber (z.B 0b00000001Bin | 0x01Hex | 1Dez)                                         
 @return   rechtsdrehnung = read_Encoder ++, linksdrehnung = read_Encoder --
*/
uint8_t read_Encoder(uint8_t PinPORT, uint8_t BinaryNumber);