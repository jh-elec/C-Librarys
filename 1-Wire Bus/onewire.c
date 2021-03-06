/*************************************************************
*|
*|	\@file  	onewire.c
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	API f�r das "1-Draht BUS System"
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
sOneWire_t sOneWire;
/*****************************************************************/
/*!<-- Interne Variablen // Ende <--*/




/*!<-- Funktionen <--*/
/*****************************************************************/
void OneWireInit( void )
{		
	/*!<-- Port mappen <--*/
	sOneWire.psDDR	= (sOneWireMapping_t*)&DDRD;
	sOneWire.psPORT	= (sOneWireMapping_t*)&PORTD;
	sOneWire.psPIN	= (sOneWireMapping_t*)&PIND;
}

eStatuscode_t OneWireReset( void )
{
	eStatuscode_t eStatus = eSTATUS_ONEWIRE_OK;
	
	sOneWire.psPORT->bDQ	= 0;
	sOneWire.psDDR->bDQ		= 1;
	_delay_us(480);
	sOneWire.psDDR->bDQ		= 0;
	_delay_us(66);
	
	if ( sOneWire.psPIN->bDQ )
	{
		eStatus = eSTATUS_ONEWIRE_NO_PRESENCE;
	}
	
	_delay_us(480);
	
	if ( ! ( sOneWire.psPIN->bDQ ) )
	{
		eStatus = eSTATUS_ONEWIRE_GND_SHORT;
	}
	
	return eStatus;             
}

void OneWireWriteBit( uint8_t uiBit )
{
	if ( ! ( uiBit & 0x01 ) )
	{
		sOneWire.psPORT->bDQ = 0;
		sOneWire.psDDR->bDQ = 1;
		_delay_us(95);
		sOneWire.psDDR->bDQ = 0;
		_delay_us(5);
	}
	else
	{
		sOneWire.psPORT->bDQ = 0;
		sOneWire.psDDR->bDQ = 1;
		_delay_us(10);
		sOneWire.psDDR->bDQ = 0;
		_delay_us(90);
	}
}

uint8_t OneWireReadBit( void )
{
	uint8_t uiBit = 0;
	
	sOneWire.psPORT->bDQ = 0;
	sOneWire.psDDR->bDQ = 1;
	_delay_us(3);
	sOneWire.psDDR->bDQ = 0;
	_delay_us(10);
	uiBit = sOneWire.psPIN->bDQ;
	_delay_us(87);
	
	if ( uiBit )
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
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

void OneWireSearchInit( OneWireROM pROM ) 
{
	memset( pROM , 0 , 8 );
	OneWireSearch( NULL , 0 );
}

uint8_t OneWireSearchROM( OneWireROM pROM )
{
	return OneWireSearch( pROM , eCOMMAND_ONEWIRE_SEARCH_ROM );
}

uint8_t OneWireSearchAlarm( OneWireROM pROM )
{
	return OneWireSearch( pROM , eCOMMAND_ONEWIRE_ALARM_SEARCH );
}

uint8_t OneWireSearch( OneWireROM pROM , enum eCommands eCommand ) 
{
	eStatuscode_t eStatus;
	
	uint8_t mask, i, j, bit, rom_tmp;
	uint8_t max_conf_zero=0;        // last bit conflict that was resolved to zero
	static uint8_t max_conf_old;    // last bit conflict that was resolved to zero in last scan
	uint8_t branch_flag=0;          // indicate new scan branch, new ROM code found

	if ( pROM == NULL ) 
	{ 
		max_conf_old=64;
		return eSTATUS_ONEWIRE_OK;
	}
	
	eStatus = OneWireReset();
	if ( eStatus )
	{
		return eStatus;
	}


	OneWireWriteByte( eCommand );
	rom_tmp  = pROM[0];
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

            case 2: // no bit conflict
            // do nothing, just go on with current bit
            break;

			case 3:
			eStatus = eSTATUS_ONEWIRE_SCAN_ERROR;
			break;
		}// end switch

		if ( bit & 1 )
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
			pROM[i] = rom_tmp;            // update tmp data
			i++;
			if ( i < 8 ) 
			{
				rom_tmp  = pROM[i];       // read new data
			}
		}
	}// end for

	max_conf_old = max_conf_zero;
	
	if ( OneWireCRC( pROM , 8 ) )
	{
		return eSTATUS_ONEWIRE_CRC_ERROR;
	}
	else if ( branch_flag )
	{
		return eSTATUS_ONEWIRE_OK;
	}
	else
	{
		return eSTATUS_ONEWIRE_LAST_CODE;
	}
}

eStatuscode_t OneWireMatchROM( const OneWireROM pROM ) 
{
	eStatuscode_t eStatus = OneWireReset();
	
	if ( eStatus )
	{
		return eStatus;
	}

	OneWireWriteByte( eCOMMAND_ONEWIRE_MATCH_ROM );
	for ( uint8_t x = 0 ; x < 8 ; x++ )
	{
		OneWireWriteByte( pROM[x] );
	}
		
	return eSTATUS_ONEWIRE_OK;
}

eStatuscode_t OneWireSkipROM( void ) 
{
	eStatuscode_t eStatus;
	
	eStatus = OneWireReset();
	if ( eStatus )
	{
		return eStatus;
	}

	OneWireWriteByte( eCOMMAND_ONEWIRE_SKIP_ROM );

	return eSTATUS_ONEWIRE_OK;
}

eStatuscode_t OneWireReadROM( OneWireROM pROM ) 
{
	eStatuscode_t eStatus;
	
	eStatus = OneWireReset();
	if ( eStatus )
	{
		return eStatus;
	}

	OneWireWriteByte( eCOMMAND_ONEWIRE_READ_ROM );
	
	for ( uint8_t x = 0 ; x < 8 ; x++ )
	{
		pROM[x] = OneWireReadByte();
	}

	if ( OneWireCRC( pROM , 8 ) )
	{
		return eSTATUS_ONEWIRE_CRC_ERROR;
	}

	return eSTATUS_ONEWIRE_OK;
}

uint8_t OneWireCRC( const uint8_t *pDataIn , uint8_t uiCnt ) 
{
    static const uint8_t uiCrcTab[]  =
    {
		0x00, 0x9D, 0x23, 0xBE, 0x46, 0xDB, 0x65, 0xF8,
		0x8C, 0x11, 0xAF, 0x32, 0xCA, 0x57, 0xE9, 0x74
	};
    
    uint8_t crc, i, tmp, zerocheck;

    // nibble based CRC calculation,
    // good trade off between speed and memory usage

    // first byte is not changed, since CRC is initialized with 0
    crc = *pDataIn++;
    zerocheck = crc;
    uiCnt--;

    for(; uiCnt>0; uiCnt--) 
	{
	    tmp = *pDataIn++;                        // next byte
	    zerocheck |= tmp;

	    i = crc & 0x0F;
	    crc = (crc >> 4) | (tmp << 4);        // shift in next nibble
	    crc ^= uiCrcTab[i];  // apply polynom

	    i = crc & 0x0F;
	    crc = (crc >> 4) | (tmp & 0xF0);      // shift in next nibble
	    crc ^= uiCrcTab[i];  // apply polynom
    }

    if ( !zerocheck ) 
	{        // all data was zero, this is an error!
	   return 0xFF;
	} 
	else 
	{
	    return crc;
    }
}

uint8_t OneWireSaveROM( OneWireROM ROM , enum eSlavesOnBus eSlave )
{
	if ( eSlave > __MAX_eSLAVE_ENTRYS__ )
	{
		return 1; /*!<-- Kein Speicherplatz <--*/
	}
	
	for ( uint8_t x = 0 ; x < 8 ; x++ )
	{
		OneWireMultiRom[ eSlave ][x] = ROM[x];
	}
	
	return 0;
}

eStatuscode_t OneWireScanBus( OneWireROM pROM ) 
{
	eStatuscode_t eStatus;
	
	OneWireSearchInit( pROM );

	eStatus = eSTATUS_ONEWIRE_OK;
	uint8_t i;
	for ( i = 0 ; ( i < __MAX_eSLAVE_ENTRYS__ ) && ( eStatus == eSTATUS_ONEWIRE_OK ) ; i++ ) 
	{
		eStatus = OneWireSearch( OneWireRom , eCOMMAND_ONEWIRE_SEARCH_ROM );
		
		if ( eStatus == eSTATUS_ONEWIRE_OK || eStatus == eSTATUS_ONEWIRE_LAST_CODE ) 
		{
			OneWireSaveROM( pROM , i );
		} 
	}
	
	sOneWire.uiBusMembers = i; 
	
	return eStatus;
}

/*****************************************************************/
/*!<-- Funktionen // Ende <--*/


// end of file




