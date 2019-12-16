/*************************************************************
*|
*|	\@file  	onewire.c
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	API für das "1-Draht BUS System"
*|
*|	\@date		16/12/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.161219
*|
*|**************************************************************/

#include "../Headers/onewire.h"


/*!<-- Defines <--*/
/*****************************************************************/

/*****************************************************************/
/*!<-- Defines // Ende <--*/



/*!<-- Interne Variablen <--*/
/*****************************************************************/
static sOneWire_t sOneWire;
/*****************************************************************/
/*!<-- Interne Variablen // Ende <--*/




/*!<-- Funktionen <--*/
/*****************************************************************/
eStatuscode_t OneWireInit( void )
{		
	/*!<-- Port mappen <--*/
	sOneWire.psDDR	= (sOneWireMapping_t *)&_ONEWIRE_GET_DDR(_ONEWIRE_PORT);
	sOneWire.psPORT	= (sOneWireMapping_t *)&_ONEWIRE_PORT;
	sOneWire.psPIN	= (sOneWireMapping_t *)&_ONEWIRE_GET_PIN(_ONEWIRE_PORT);
	
	if ( &sOneWire.psDDR == NULL || &sOneWire.psPIN == NULL || &sOneWire.psPORT == NULL )
	{
		return eSTATUS_INVALID_CONFIG;
	}
	
	return eSTATUS_OK;
}

eStatuscode_t OneWireReset( void )
{
	eStatuscode_t eStatus = eSTATUS_ACK;
	
	sOneWire.psPORT->bData	= 0;
	sOneWire.psDDR->bData		= 1;
	_delay_us(480);
	sOneWire.psDDR->bData		= 0;
	_delay_us(66);
	
	if ( sOneWire.psPIN->bData )
	{
		eStatus = eSTATUS_NO_ACK;
	}
	
	_delay_us(480);
	if ( ! ( sOneWire.psPIN->bData ) )
	{
		eStatus = eSTATUS_BUS_SHORT_TO_GND;
	}
	
	return eStatus;             
}

void OneWireWriteBit( uint8_t uiBit )
{
	if ( uiBit )
	{
		sOneWire.psPORT->bData = 0;
		sOneWire.psDDR->bData = 1;
		_delay_us(95);
		sOneWire.psDDR->bData = 0;
		_delay_us(5);
	}
	else
	{
		sOneWire.psPORT->bData = 0;
		sOneWire.psDDR->bData = 1;
		_delay_us(10);
		sOneWire.psDDR->bData = 0;
		_delay_us(90);
	}
}

uint8_t OneWireReadBit( void )
{
	uint8_t uiBit = 0;
	
	sOneWire.psPORT->bData = 0;
	sOneWire.psDDR->bData = 1;
	_delay_us(3);
	sOneWire.psDDR->bData = 0;
	_delay_us(10);
	uiBit = sOneWire.psPIN->bData;
	_delay_us(87);
	
	return uiBit;
}

uint8_t OneWireReadByte( void )
{
    uint8_t uiData = 0;
    uint8_t i;

    for ( i = 0 ; i < 8 ; i++ ) 
	{
	    uiData >>= 1;         // LSB first on OneWire
	    if ( OneWireReadBit() ) 
		{
		    uiData |= 0x80;
	    }
    }
    return uiData;
}

void OneWireWriteByte( uint8_t uiData )
{
	for ( uint8_t x = 0 ; x < 8 ; x++ )
	{
		OneWireWriteBit( uiData );
		uiData >>= 1;
	}
}

void OneWireSearchInit( uint8_t uiBuffer[8] ) 
{
	memset( uiBuffer , 0 , 8 );
	OneWireSearch( NULL , 0 );
}

uint8_t OneWireSearchROM( uint8_t uiBuffer[8] )
{
	return OneWireSearch( uiBuffer , eCOMMAND_SEARCH_ROM );
}

uint8_t OneWireSearchAlarm( uint8_t uiBuffer[8] )
{
	return OneWireSearch( uiBuffer , eCOMMAND_ALARM_SEARCH );
}

uint8_t OneWireSearch( uint8_t uiBuffer[8] , enum eCommands eCommand ) 
{
	eStatuscode_t eStatus = eSTATUS_OK;
	
	uint8_t mask, i, j, bit, rom_tmp;
	uint8_t max_conf_zero=0;        // last bit conflict that was resolved to zero
	static uint8_t max_conf_old;    // last bit conflict that was resolved to zero in last scan
	uint8_t branch_flag=0;          // indicate new scan branch, new ROM code found

	if ( uiBuffer == NULL ) 
	{    // init search
		max_conf_old=64;
		return 0;
	}

	if ( OneWireReset() ) 
	{
		return eSTATUS_NO_ACK;               // no response
	} 

	OneWireWriteByte( eCommand );
	rom_tmp  = uiBuffer[0];
	i = 0;
	mask = 1;
		
	// scan all 64 ROM bits
	for( j = 0 ; j < 64 ; j++ ) 
	{
		bit  = OneWireReadBit();      // bit
		bit |= OneWireReadBit() << 1; // inverted bit

		switch( bit ) 
		{
			case 0:     // bit conflict, more than one device with different bit
			if ( j < max_conf_old ) 
			{         // below last zero branch conflict level, keep current bit
				if ( rom_tmp & mask ) // last bit was 1
				{       
					bit = 1;
				}
				else // last bit was 0
				{                    
					bit = 0;
					max_conf_zero = j;
					branch_flag = 1;
				}
			} 
			else if ( j == max_conf_old ) // last zero branch conflict, now enter new path
			{ 
				bit = 1;
			}
			else // above last run conflict level
			{                        
				bit = 0;            
				max_conf_zero = j;
				branch_flag = 1;
			}
			break;

			case 3:
			eStatus = eSTATUS_NO_ACK;
			break;
		}// end switch

		if ( bit )
		{
			OneWireWriteBit( 1 );
			rom_tmp |= mask;
		}
		else
		{
			OneWireWriteBit(0);
			rom_tmp &= ~mask;
		}
			
		mask <<= 1;
		if ( mask == 0 ) 
		{
			mask = 1;
			uiBuffer[i] = rom_tmp;            // update tmp data
			i++;
			if ( i < 8 ) 
			{
				rom_tmp  = uiBuffer[i];       // read new data
			}
		}
	}// end for

	max_conf_old = max_conf_zero;
	
	if ( branch_flag ) 
	{
		eStatus = eSTATUS_NEW_ROM_CODE_FOUND;           // new code found
	} 

	return eStatus;
}

eStatuscode_t OneWireMatchROM( const uint8_t uiBufferROM[8] ) 
{
	uint8_t mask, tmp=0, i, j;

	if ( OneWireReset() )
	{
		return eSTATUS_NO_ACK;               // no response
	} 
	else 
	{
		OneWireWriteByte( eCOMMAND_MATCH_ROM );
		mask = 0;
		i = 0;
		for ( j = 0 ; j < 64 ; j++ ) 
		{
			mask <<= 1;
			if (mask == 0) 
			{
				mask = 1;
				tmp = uiBufferROM[i];
				i++;
			}

			if ( tmp & mask ) 
			{
				OneWireWriteBit( 1 );   // branch 1
			} 
			else 
			{
				OneWireWriteBit( 0 );   // branch 0
			}
		}
	}
	
	return eSTATUS_OK;
}

eStatuscode_t OneWireSkipROM( void ) 
{
	if ( OneWireReset() ) 
	{
		return eSTATUS_NO_ACK;
	} 
	else 
	{
		OneWireWriteByte( eCOMMAND_SKIP_ROM );
	}
	
	return eSTATUS_OK;
}

eStatuscode_t OneWireReadROM( uint8_t uiBufferROM[8] ) 
{
	uint8_t mask=1, tmp=0, i=0, j;

	if ( OneWireReset() ) 
	{
		return eSTATUS_NO_ACK;                           
	} 
	else 
	{
		OneWireWriteByte( eCOMMAND_READ_ROM );
		for ( j = 0 ; j < 64 ; j++ ) 
		{
			if ( OneWireReadBit() )  
			{
				tmp |= mask;
			}

			mask <<= 1;
			if ( mask == 0 ) 
			{
				mask = 1;
				uiBufferROM[i]=tmp;
				tmp=0;
				i++;
			}
		}

		if( OneWireCRC( uiBufferROM , 8 ) ) 
		{
			return eSTATUS_CRC_ERROR; 
		}
	}
	
	return eSTATUS_OK;
}

uint8_t OneWireCRC( const uint8_t *pDataIn , uint8_t uiCnt ) 
{
	static const uint8_t uiCrcTab[] =
	{
		0x00, 0x9D, 0x23, 0xBE, 0x46, 0xDB, 0x65, 0xF8,
		0x8C, 0x11, 0xAF, 0x32, 0xCA, 0x57, 0xE9, 0x74
	};
	
	uint8_t crc, i, tmp;

	// nibble based CRC calculation,
	// good trade off between speed and memory usage

	// first byte is not changed, since CRC is initialized with 0
	crc = *pDataIn++;
	uiCnt--;

	for( ; uiCnt > 0 ; uiCnt-- ) 
	{
		tmp = *pDataIn++;                        // next byte

		i = crc & 0x0F;
		crc = (crc >> 4) | (tmp << 4);        // shift in next nibble
		crc ^= uiCrcTab[i];  // apply polynom

		i = crc & 0x0F;
		crc = (crc >> 4) | (tmp & 0xF0);      // shift in next nibble
		crc ^= uiCrcTab[i];  // apply polynom
	}

	return crc;
}
/*****************************************************************/
/*!<-- Funktionen // Ende <--*/


// end of file




