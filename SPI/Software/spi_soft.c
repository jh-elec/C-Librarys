

#include <avr/io.h>
#include "spi_soft.h"



void spiSoftInit( void )
{
	SPI_SOFT_SCK_DDR	|= 1<<SPI_SOFT_SCK_BP;
	SPI_SOFT_MOSI_DDR	|= 1<<SPI_SOFT_MOSI_BP;
	SPI_SOFT_SCS_DDR	|= 1<<SPI_SOFT_SCS_BP;

	/*
	*	MISO als Eingang konfigurieren
	*/
	SPI_SOFT_MISO_DDR	&= ~(1<<SPI_SOFT_MISO_BP);
}

void spiSoftWrite( uint8_t byte )
{
	uint8_t n = 0;
	
	for (n = 0 ; n < 8 ; n++)
	{
		#if		( CPOL == 1 )
 			PORT(SPI_SOFT_SCK_DDR) |=  (1<<SPI_SOFT_SCK_BP);
		#elif	( CPOL == 0 )
			PORT(SPI_SOFT_SCK_DDR) &= ~(1<<SPI_SOFT_SCK_BP);
		#endif
		
		if (byte & 0x80)
		{
			PORT(SPI_SOFT_MOSI_DDR) |=  (1<<SPI_SOFT_MOSI_BP);
		}
		else
		{
			PORT(SPI_SOFT_MOSI_DDR) &= ~(1<<SPI_SOFT_MOSI_BP);
		}
		byte <<= 1;
		
		#if		( CPOL == 1 )
			PORT(SPI_SOFT_SCK_DDR) &=  ~(1<<SPI_SOFT_SCK_BP);
		#elif	( CPOL == 0 )
			PORT(SPI_SOFT_SCK_DDR) |=   (1<<SPI_SOFT_SCK_BP);
		#endif
				
	}
}

uint8_t spiSoftRead( void )
{
	uint8_t n = 0;
	uint8_t ret = 0;
	
	for( n = 0x80 ; n ; n>>=1 )
	{
		
		#if		( CPHA == 1 )
			PORT(SPI_SOFT_SCK_DDR) |=  (1<<SPI_SOFT_SCK_BP);
			PORT(SPI_SOFT_SCK_DDR) &= ~(1<<SPI_SOFT_SCK_BP);
		#elif	( CPHA == 0 )
			PORT(SPI_SOFT_SCK_DDR) &= ~(1<<SPI_SOFT_SCK_BP);
			PORT(SPI_SOFT_SCK_DDR) |=  (1<<SPI_SOFT_SCK_BP);
		#endif
		
		if( ( SPI_SOFT_MISO_PIN ) & ( 1<<SPI_SOFT_MISO_BP ) )
		{
			ret |= n;
		}
	}
	
	return ret;
}

void spiSoftSlaveSelect( void )
{
	PORT(SPI_SOFT_SCS_DDR) &= ~(1<<SPI_SOFT_SCS_BP);
}

void spiSoftSlaveDeSelect( void )
{
	PORT(SPI_SOFT_SCS_DDR) |= (1<<SPI_SOFT_SCS_BP);
}