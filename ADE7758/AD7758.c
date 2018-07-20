/*
 * Copyright:      Jan Homann
 * Author   :      Jan Homann 
 * Version  :      1.0 
 * Date     :	   24.02.2016
 * Source   :      none
 */


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#include "spi.h"
#include "AD7758.h"
#include "lcd.h"

char Buffer[20];



int8_t ADE7758_read_int8(uint8_t reg)
{
	int8_t temp = 0;
		
	spi_enable_chip();
	spi_disable_chip(); // start transmission condition
	
	spi_master_transmit(reg); // register address
	spi_master_transmit(0x00); // dummy byte	

	temp = (int8_t)spi_receive(); // first byte	

	spi_enable_chip(); // stop transmission condition	
	
	return (int8_t)temp;
}

int16_t ADE7758_read_int16(uint8_t reg)
{
	int8_t Byte[2]={0,0};
	
	spi_enable_chip();
	spi_disable_chip(); // start transmission condition
	
	spi_master_transmit(reg); // register address
	
	spi_master_transmit(0x00); // dummy byte
	Byte[0] = (int8_t) spi_receive(); // first byte
	
	spi_master_transmit(0x00); // dummy byte
	Byte[1] = (int8_t)spi_receive(); // second byte
	
	spi_enable_chip(); // stop transmission condition	
	
	int16_t result = (int16_t)Byte[0]<<8 | (int16_t)Byte[1];
	
	return result;
}

uint16_t ADE7758_read_uint16(uint8_t reg)
{
	uint8_t Byte[2]={0,0};

	
	spi_enable_chip();
	spi_disable_chip(); // start transmission condition
	
	spi_master_transmit(reg); // register address
	
	spi_master_transmit(0x00); // dummy byte
	Byte[0] = (uint8_t) spi_receive(); // first byte
	
	spi_master_transmit(0x00); // dummy byte
	Byte[1] = (uint8_t)spi_receive(); // second byte
	
	spi_enable_chip(); // stop transmission condition
	
	uint16_t result = (uint16_t)Byte[0]<<8 | (uint16_t)Byte[1];
	
	return result;
}

uint32_t ADE7758_read_uint24(uint8_t reg)
{
	uint8_t Byte[3]={0,0,0};
	
	spi_enable_chip();
	spi_disable_chip(); // start transmission condition
	
	spi_master_transmit(reg); // register address
	
	spi_master_transmit(0x00); // dummy byte
	Byte[0] = (uint8_t) spi_receive();
	
	spi_master_transmit(0x00); // dummy byte
	Byte[1] = (uint8_t) spi_receive();
	
	spi_master_transmit(0x00); // dummy byte
	Byte[2] = (uint8_t) spi_receive();
	
	spi_enable_chip(); // stop transmission condition
	
	int32_t result = ((uint32_t) Byte[0] << 16 | (uint32_t) Byte[1] << 8 | (uint32_t)Byte[2]);
	
	
	return result;
}

void ADE7758_write_uint8(uint8_t reg, uint8_t byte_1)
{
	spi_enable_chip();
	spi_disable_chip();

	spi_master_transmit(reg | 0x80); // 0x80 for write command
	spi_master_transmit(byte_1); // data transmission
	spi_enable_chip();
}

void ADE7758_write_uint16(uint8_t reg, uint8_t byte_1, uint8_t byte_2)
{
	spi_enable_chip();
	spi_disable_chip();

	spi_master_transmit(reg | 0x80); // 0x80 for write command
	spi_master_transmit(byte_1); // data transmission
	spi_master_transmit(byte_2);

	spi_enable_chip();
}

void ADE7758_write_uint24(uint8_t reg, uint8_t byte_3, uint8_t byte_2, uint8_t byte_1)
{
	spi_enable_chip();
	spi_disable_chip();

	spi_master_transmit(reg | 0x80); // 0x80 for write command
	spi_master_transmit(byte_3); // data transmission
	spi_master_transmit(byte_2);
	spi_master_transmit(byte_1);
	
	spi_enable_chip();
}

int8_t ADE7758_readTEMP_OFFSET(void)
{
	return (int8_t)ADE7758_read_int8(REG_TEMP);
}

int8_t ADE7758_readTEMP(void)
{
	int8_t temperature = 0;
	
	temperature	 = ADE7758_read_int8(REG_TEMP); // raw data
	temperature -= 40; // offset for this sensor
	temperature *= 3; // 3°C/Bit
	//temperature += 21; // ambient temperature
	
	return temperature;
}
