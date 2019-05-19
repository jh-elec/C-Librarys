/* Generated by CodeDescriptor 1.2.0.0713 */
/*
* Project Name      -> I2C 
* Version           -> 1.0.0.0714
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 15.05.2019 07:34:00
* Description       -> I2C Treiber f�r die AVR Serie
*
*
*
*/
#ifndef __I2C_H__
#define __I2C_H__

#include <inttypes.h>
#include <avr/io.h>

#define I2C_READ		0x01
#define I2C_WRITE		0x00

#define I2C_F_SCL		800000UL // SCL frequency
#define I2C_PRESCALER	1
#define I2C_TWBR		((((F_CPU / I2C_F_SCL) / I2C_PRESCALER) - 16 ) / 2)

enum I2c_Return_Codes
{
	_I2C_RETURN_NACK_,
	_I2C_RETURN_ACK_,	
};

enum I2c_Transfer
{
	I2C_TRANSFER_ADDR_NIDS = 0xFF, // Register Adresse ist nicht im Datenstrom enthalten (NotInDataStream)
};

typedef struct  
{
	uint8_t	*ptrData;
	
	uint8_t	uiLength;
	
	uint8_t	uiSlaveAddress;
	
	uint8_t	uiRegisterAddress;
	
	struct  
	{
		/*	Haendelt das senden der Registeradresse
		*/
		enum I2c_Transfer Handling;	
		uint8_t uiHandlingAddress;
		
	}SpecialAddressHandling;
	
}I2cTransfer_t;


void	i2c_init(void);

enum	I2c_Return_Codes I2cWriteBytes( I2cTransfer_t *Tx );

enum	I2c_Return_Codes I2cReadBytes( I2cTransfer_t *Rx );


#endif // I2C_MASTER_H
