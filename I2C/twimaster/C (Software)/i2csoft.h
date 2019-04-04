



#ifndef __I2CSOFT_H__
#define ___I2CSOFT_H__

#ifndef F_CPU
#define F_CPU	16e6
#endif

#include <avr/io.h>
#include <util/delay.h>

#ifdef __AVR__
#define	__PORT_DDR__( _p )		(*( (_p-1) ))	
#define __PORT_PIN__( _p )		(*( (_p-2) ))
#else
#error Library not Supported for our Device
#endif 

#define Q_DEL _delay_loop_2(3)
#define H_DEL _delay_loop_2(5)

typedef uint8_t PortBit_t;
typedef uint8_t Port_t;

typedef struct
{
	/*
	*	Port für das I²C Modul.
	*	Es wird die Adresse von dem Daten Richtungs Register
	*	später rechnerisch ermittelt.
	*/
	volatile Port_t *Port;
	
	/*
	*	SCL Pin 
	*/
	PortBit_t Scl;
	
	/*
	*	SDA Pin
	*/
	PortBit_t Sda;
	
}tI2c;


void I2cSoftInit(  tI2c *Object );

void I2cSoftStart( tI2c *Object );

void I2cSoftStop( tI2c *Object );

uint8_t I2cSoftWrite( tI2c *Object , uint8_t Data );

uint8_t I2cSoftRead( tI2c *Object , uint8_t Acknowledge );


#endif 