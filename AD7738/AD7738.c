/*
 * Copyright:      Jan Homann
 * Author   :      Jan Homann 
 * Version  :      1.0 
 * Date     :	   25.05.2016
 * Source   :      -Datasheet (ADE7738 @AnalogDevices)
 */

#include <avr/io.h>
#include "spi.h"

uint8_t tmp[3];

void AD7738_write_uint8(uint8_t reg, uint8_t byte)
{
	spi_disable_chip();
	spi_enable_chip();
	spi_master_transmit(reg & 0x0F) ; // 0x40 for communication register read / 0x0F for write
	spi_master_transmit(byte);
	spi_disable_chip();
}

void AD7738_write_uint16(uint8_t reg, uint16_t byte)
{
	tmp[0] = byte >> 8;	
	tmp[1] = byte & 0x00FF;
	
	spi_disable_chip();
	spi_enable_chip();
	spi_master_transmit(reg & 0x0F) ; // 0x40 for communication register read / 0x0F for write
	spi_master_transmit(tmp[0]);
	spi_master_transmit(tmp[1]);
	spi_disable_chip();
}

void AD7738_write_uint24(uint8_t reg, uint32_t byte)
{
	tmp[0] = byte >> 16;
	tmp[1] = byte >> 8;
	tmp[2] = byte & 0x00FF;
			
	spi_disable_chip();
	spi_enable_chip();
	spi_master_transmit(reg & 0x0F) ; // 0x40 for communication register read / 0x0F for write
	spi_master_transmit(tmp[0]);
	spi_master_transmit(tmp[1]);
	spi_master_transmit(tmp[2]);
	spi_disable_chip();
}

uint8_t AD7738_read_uint8(uint8_t reg)
{
	spi_disable_chip();
	spi_enable_chip();
	spi_master_transmit(reg | 0x40) ; // 0x40 for communication register read / 0x0F for write
	spi_master_transmit(0x00);
	tmp[0] = spi_receive();
	spi_disable_chip();
	
	return tmp[0];
}

uint16_t AD7738_read_uint16(uint8_t reg)
{	
	spi_disable_chip();
	spi_enable_chip();
	spi_master_transmit(reg | 0x40) ; // 0x40 for communication register read / 0x0F for write
	spi_master_transmit(0x00);
	tmp[0] = spi_receive();
	spi_master_transmit(0x00); // dummy byte
	tmp[1] = spi_receive();
	spi_disable_chip();

	uint16_t result = (uint16_t)tmp[0] << 8 | (uint16_t)tmp[1];
	
	return result;
}

uint32_t AD7738_read_uint24(uint8_t reg)
{
	spi_disable_chip();
	spi_enable_chip();
	spi_master_transmit(reg | 0x40) ; // 0x40 for communication register read / 0x0F for write
	spi_master_transmit(0x00);
	tmp[0] = spi_receive();
	spi_master_transmit(0x00); // dummy byte
	tmp[1] = spi_receive();
	spi_master_transmit(0x00); // dummy byte
	tmp[2] = spi_receive();
	spi_disable_chip();
	
	uint32_t result = ((uint32_t)tmp[0] << 16 | (uint32_t)tmp[1] << 8 | (uint32_t)tmp[2]);
	
	return result;
}