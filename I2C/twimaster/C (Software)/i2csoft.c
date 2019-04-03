/**********************************************************

Software I2C Library for AVR Devices.

Copyright 2008-2012
eXtreme Electronics, India
www.eXtremeElectronics.co.in
**********************************************************/
 
#include <avr/io.h>
#include <util/delay.h>

#include "i2csoft.h"





void I2cSoftInit( tI2c *Object ) 
{
	__PORT_DDR__( Object->Port ) |= ( 1 << Object->Scl ); // Scl als Ausgang konfigurieren
	__PORT_DDR__( Object->Port ) |= ( 1 << Object->Sda ); // Sda als Ausgang konfigurieren
	
	Object->Port |= ( 1 << Object->Scl ); // Default Pegel einstellen
	Object->Port |= ( 1 << Object->Sda ); // ..	
}

void I2cSoftStart( tI2c *Object )
{
	Object->Port |= ( 1 << Object->Scl ); // High
	H_DEL;
	
	Object->Port &= ~( 1 << Object->Sda ); // Low
	H_DEL;
}

void I2cSoftStop( tI2c *Object )
{
	Object->Port &= ~( 1 << Object->Sda ); // Low
	H_DEL;
	
	Object->Port |= ( 1 << Object->Scl ); // High
	Q_DEL;
	
	Object->Port |= ( 1 << Object->Sda ); // High;
	H_DEL;
}

uint8_t I2cSoftWrite( tI2c *Object , uint8_t Data )
{
	static uint8_t Acknowledge = 0;
	uint8_t ui;
	
	for ( ui = 0 ; ui < 8 ; ui++ )
	{
		Object->Port &= ~( 1 << Object->Scl ); // Low
		
		Q_DEL;
		if ( Data & 0x80 )
		{
			Object->Port |= ( 1 << Object->Sda ); // High
		}else
		{
			Object->Port &= ~( 1 << Object->Sda ); // Low
		}
		H_DEL;
		
		Object->Port |= ( 1 << Object->Scl ); // High
		H_DEL;
		
		__PORT_DDR__( Object->Port ) &= ~( 1 << Object->Scl ); // Scl als Eingang konfigurieren
		while ( __PORT_PIN__( Object->Port ) & ( 1 << Object->Scl ) == 0 ); // Warten bis "Scl" Low..
		__PORT_DDR__( Object->Port ) |= ( 1 << Object->Scl ); // Scl als Ausgang konfigurieren
		
		Data <<= 1;
	}

	//The 9th clock (ACK Phase)	
	Object->Port &= ~( 1 << Object->Scl ); // Low
	Q_DEL;
	
	Object->Port |= ( 1 << Object->Sda ); // High
	H_DEL;
	
	Object->Port |= ( 1 << Object->Scl ); // High
	H_DEL;
		
	__PORT_DDR__( Object->Port ) &= ~( 1 << Sda ); // Als Eingang verwenden
	Acknowledge = !(__PORT_PIN__( Object->Port ) & ( 1 << Object->Sda ) );
	__PORT_DDR__( Object->Port ) |= ( 1 << Sda ); // Als Ausgang verwenden	
	
	Object->Port &= ~( 1 << Object->Scl ); // Low 
	H_DEL;
	
	return Acknowledge;	
}

 
 uint8_t I2cSoftRead( tI2c *Object , uint8_t Acknowledge )
 {
	 uint8_t uiData = 0;
	 uint8_t ui;
	 
	 for ( ui = 0 ; ui < 8 ; ui++ )
	 {
		Object->Port &= ~( 1 << Object->Scl ); // Low
		H_DEL;
		Object->Port |= ( 1 << Object->Scl ); // High
		H_DEL;	

		__PORT_DDR__( Object->Port ) &= ~( 1 << Object->Scl ); // Als Eingang nutzen
		while((__PORT_PIN__( Object->Port ) & ( 1 << Object->Scl ) ) == 0 );
		__PORT_DDR__( Object->Port ) |= ( 1 << Object->Scl ); // Als Ausgang nutzen
		
		
		__PORT_DDR__( Object->Port ) &= ~( 1 << Object->Sda ); // Als Eingang nutzen
		if( __PORT_PIN__( Object->Port ) & ( 1 << Object->Sda ) )
		{
			uiData |= ( 0x80 >> ui );
		}
		__PORT_DDR__( Object->Port ) |= ( 1 << Object->Sda ); // Als Ausgang nutzen	
	 }

	Object->Port &= ~( 1 << Object->Scl ); //Soft_I2C_Put_Ack
	Q_DEL;

	if ( Acknowledge )
	{
		Object->Port &= ~( 1 << Object->Sda ); // Low
	}else
	{
		Object->Port |= ( 1 << Object->Sda ); // High
	}
	H_DEL;
	
	Object->Port |= ( 1 << Object->Scl ); // High
	H_DEL;
	
	Object->Port &= ~( 1 << Object->Scl ); // Low
	H_DEL;
			
	return uiData;
 }
 
















