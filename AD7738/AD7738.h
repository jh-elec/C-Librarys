/*
 * Copyright:      Jan Homann
 * Author   :      Jan Homann 
 * Version  :      1.0 
 * Date     :	   25.05.2016
 * Source   :      -Datasheet (ADE7738 @AnalogDevices)
 */




#ifndef F_CPU
#define F_CPU 16000000
#endif

/*shows @communication register @page 13 */
#define ADE7738_READ				0x00
#define ADE7738_WRITE				0x40


#define	COM_REG						0x00
#define	IO_REG						0x01
#define	REV_REG						0x02
#define	TEST_REG					0x03
#define ADC_STATE_REG				0x04
#define	CHECKSUM_REG				0x05
#define	ADC_ZERO_SCALE_CALC_REG		0x06
#define ADC_FULL_SCALE_REGISTER		0x07
#define CHA_DATA_REGISTER_0			0x08
#define CHA_DATA_REGISTER_1			0x09
#define CHA_DATA_REGISTER_2			0x0A
#define CHA_DATA_REGISTER_3			0x0B
#define CHA_DATA_REGISTER_4			0x0C
#define CHA_DATA_REGISTER_5			0x0D
#define CHA_DATA_REGISTER_6			0x0E
#define CHA_DATA_REGISTER_7			0x0F

#define CHA_ZERO_SCALE_CALC_REG_0	0x10
#define CHA_ZERO_SCALE_CALC_REG_1	0x11
#define CHA_ZERO_SCALE_CALC_REG_2	0x12
#define CHA_ZERO_SCALE_CALC_REG_3	0x13
#define CHA_ZERO_SCALE_CALC_REG_4	0x14
#define CHA_ZERO_SCALE_CALC_REG_5	0x15
#define CHA_ZERO_SCALE_CALC_REG_6	0x16
#define CHA_ZERO_SCALE_CALC_REG_7	0x17

#define CHA_FULL_SCALE_CALC_REG_0	0x18
#define CHA_FULL_SCALE_CALC_REG_1	0x19
#define CHA_FULL_SCALE_CALC_REG_2	0x1A
#define CHA_FULL_SCALE_CALC_REG_3	0x1B
#define CHA_FULL_SCALE_CALC_REG_4	0x1C
#define CHA_FULL_SCALE_CALC_REG_5	0x1D
#define CHA_FULL_SCALE_CALC_REG_6	0x1E
#define CHA_FULL_SCALE_CALC_REG_7	0x1F

#define CHA_STATE_REG_0				0x20
#define CHA_STATE_REG_1				0x21
#define CHA_STATE_REG_2				0x22
#define CHA_STATE_REG_3				0x23
#define CHA_STATE_REG_4				0x24
#define CHA_STATE_REG_5				0x25
#define CHA_STATE_REG_6				0x26
#define CHA_STATE_REG_7				0x27

#define CHA_SETUP_REG_0				0x28
#define CHA_SETUP_REG_1				0x29
#define CHA_SETUP_REG_2				0x2A
#define CHA_SETUP_REG_3				0x2B
#define CHA_SETUP_REG_4				0x2C
#define CHA_SETUP_REG_5				0x2D
#define CHA_SETUP_REG_6				0x2E
#define CHA_SETUP_REG_7				0x2F

#define CHA_CONV_TIME_REG_0			0x30
#define CHA_CONV_TIME_REG_1			0x31
#define CHA_CONV_TIME_REG_2			0x32
#define CHA_CONV_TIME_REG_3			0x33
#define CHA_CONV_TIME_REG_4			0x34
#define CHA_CONV_TIME_REG_5			0x35
#define CHA_CONV_TIME_REG_6			0x36
#define CHA_CONV_TIME_REG_7			0x37

#define MODE_REG					0x38 // the chip have only one mode register but is accessibly from 0x38 - 0x3F


/***Prototype functions***/


/* write data to the ad7738
* parameter : 
*				reg	 = address of register
*				byte = data to send			
*							
*							
*							
*
* returns	: none
*/
void AD7738_write_uint8(uint8_t reg, uint8_t byte);

/* write data to the ad7738
* parameter :
*				reg	 = address of register
*				byte = data to send
*
*
*
*
* returns	: none
*/
void AD7738_write_uint16(uint8_t reg, uint16_t byte);

/* write data to the ad7738
* parameter :
*				reg	 = address of register
*				byte = data to send
*
*
*
*
* returns	: none
*/
void AD7738_write_uint24(uint8_t reg, uint32_t byte);

/* read uint8 data from the ad7738
* parameter :
*				reg  = address of register
*				
*
*
*
*
* returns	: uint8_t data
*/
uint8_t AD7738_read_uint8(uint8_t reg);

/* read uint8 data from the ad7738
* parameter :
*				reg  = address of register
*
*
*
*
*
* returns	: uint16_t data
*/
uint16_t AD7738_read_uint16(uint8_t reg);

/* read uint8 data from the ad7738
* parameter :
*				reg  = address of register
*
*
*
*
*
* returns	: uint32_t data 
*/
uint32_t AD7738_read_uint24(uint8_t reg);