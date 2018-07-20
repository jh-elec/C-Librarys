#ifndef _com74hc595_H_
#define _com74hc595_H_

// Numbers of 74HC595
#define com74hc595_SIZE 1

// PORTS
#define PORT_com74hc595 PORTC
#define PORT_SER 		PC0		//74HC595 PIN 14
#define PORT_SCK 		PC1		//74HC595 PIN 12
#define PORT_RCK 		PC2		//74HC595 PIN 11
#define PORT_OE 		PB3		//74HC595 PIN 13

#define DDR_com74hc595  DDRC
#define DDR_SER 		DDC0	//74HC595 PIN 14
#define DDR_SCK 		DDC1	//74HC595 PIN 12
#define DDR_RCK 		DDC2	//74HC595 PIN 11
#define DDR_OE 			DDC3	//74HC595 PIN 13

// use with /OE
//#define WITH_OE

//********** only for internal use. Don't change any value ***************

// Numbers of Bits over all 74HC595
#define com74hc595_BYTES com74hc595_SIZE * 8

// global array 
extern unsigned char com74hc595[];

// initialize the mikrocontroller
extern void com74hc595_init();

// output to 74HC595
extern void com74hc595_out();

// set a single Bit in arrays
extern void com74hc595_setBit( unsigned char BitNumber );

// unset a single Bit in arrays
extern void com74hc595_unsetBit( unsigned char BitNumber );

// set a port with a 8 Bit value
extern void com74hc595_setPort( unsigned char Port, unsigned char Bits );

// unset a port
extern void com74hc595_unsetPort( unsigned char Port );

// set all bit to logical High = 1
extern void com74hc595_setall();

// set all bit to logical Low = 0
extern void com74hc595_unsetall();

#endif /* _com74hc595_H_ */
