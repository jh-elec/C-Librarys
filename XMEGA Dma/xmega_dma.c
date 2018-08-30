/*
 * DMAController.cpp
 *
 *  Created on: 16.01.2013
 *      Author: Nicolas Göddel
 *
 *	https://www.mikrocontroller.net/articles/Speicherdirektzugriff_(DMA)_mit_dem_ATxmega#DMAController.cpp
 */

#include "xmega_dma.h"


#define DOUBLE_BUFFERED(channel)			( dma.doubleBuffered & _BV ( ( channel ) >> 1 ) )
#define CHANNEL(channel)					( ( ( DMA_CH_t * ) &DMA.CH0 )[channel])



dma_t dma;


static void dmaUseDoubleBuffering			( uint8_t channel , bool use )				
{
	channel >>= 1;

	if ( use ) 
	{
		DMA.CTRL |= (channel + 1) << 2;	//DMA_DBUFMODE_CH01_gc oder DMA_DBUFMODE_CH23_gc
		dma.usedChannels |= _BV(2 * channel) | _BV(2 * channel + 1);
		dma.doubleBuffered = channel;
	} 
	else 
	{
		DMA.CTRL &= ~((channel + 1) << 2);
		dma.doubleBuffered &= ~_BV(channel);
	}
}

uint8_t*	dmaUpdateAutoBuffer				( uint8_t channel , uint16_t blockSize )	
{
	/*
	 * Falls bei diesem Kanal der dma.buffer nicht automatisiert alloziert werden soll
	 * oder blockSize = 0 ist, lösche ihn.
	 */

	if ( ! ( dma.autoDestBuffer & _BV(channel ) ) || blockSize == 0) 
	{
		if ( dma.buffer[channel] ) 
		{
			free( (uint8_t*)dma.buffer[channel] );
			dma.buffer[channel] = 0;
		}
	} else 
	{
		/*
		 * Ist schon ein DMA Buffer mit anderer Größe alloziert, lösche den alten
		 * und alloziere einen neuen.
		 */
		if ( ( dma.buffer[channel] ) && ( dma.blockSizes[channel] != blockSize ) ) 
		{
			free( (uint8_t*)dma.buffer[channel] );
			
			dma.blockSizes[channel]	= blockSize;
			dma.buffer[channel]		= (uint8_t*) malloc(blockSize);
			
			if ( dma.buffer[channel] == 0 ) 
			{
				return 0;
			}
		}
	}

	return dma.buffer[channel];
}

void		dmaSetBuffer					( uint8_t channel )							
{
	if ( dma.autoDestBuffer & _BV( channel ) ) 
	{
		CHANNEL(channel).DESTADDR0 = (uintptr_t)dma.buffer[ channel     ];
 		CHANNEL(channel).DESTADDR1 = (uintptr_t)dma.buffer[ channel + 1 ];
 		CHANNEL(channel).DESTADDR2 = (uintptr_t)dma.buffer[ channel + 2 ];
	}
}

void		dmaReset										( void )									
{
	// reset DMA controller
	DMA.CTRL = 0;
	DMA.CTRL = DMA_RESET_bm;
	
	while ( ( DMA.CTRL & DMA_RESET_bm ) != 0 );
	
	dma.usedChannels = 0;
	dma.doubleBuffered = 0;
	dma.autoDestBuffer = 0;
	
	for ( uint8_t i = 0 ; i < 3 ; i++ ) 
	{
		dma.blockSizes[i] = 0;
		
		if (dma.buffer[i])
		{
			free( &dma.buffer[i] );
		}
			
		dma.buffer[i] = 0;
	}
}

void		dmaSetBlockSize									( uint8_t channel , uint16_t blockSize )	
{
	if ( DOUBLE_BUFFERED( channel ) ) 
	{
		channel &= ~1;
		dmaUpdateAutoBuffer( channel, blockSize );
		dmaUpdateAutoBuffer( channel + 1, blockSize );
		dmaSetBuffer( channel );
		dmaSetBuffer( channel + 1 );
		CHANNEL( channel ).TRFCNT = blockSize;
		CHANNEL( channel + 1 ).TRFCNT = blockSize;
	} 
	else 
	{
		dmaUpdateAutoBuffer( channel , blockSize );
		dmaSetBuffer(channel);
		CHANNEL(channel).TRFCNT = blockSize;
	}
}

void		dmaUseChannel									( uint8_t channel , uint8_t use )			
{
	dma.usedChannels = ( dma.usedChannels & ~_BV( channel ) ) | ( use << channel );
	if ( !use ) 
	{
		dmaDisableChannel( channel );
		if (DOUBLE_BUFFERED( channel ) ) 
		{
			dma.doubleBuffered &= ~_BV( channel >> 1 );
			dmaUseDoubleBuffering( channel , false );
		}
	}
}

void		dmsSetSourceChannel								( uint8_t channel , void *source )			
{	
	dmaaddr_t addr = CPTR2DMA( source );
	
	CHANNEL(channel).SRCADDR0 = addr;
	CHANNEL(channel).SRCADDR1 = addr >> 8;
	CHANNEL(channel).SRCADDR2 = addr >> 16;
}

void		dmaSetSourceChannelDirection					( uint8_t channel , void *source , enum SrcDirection_enum direction , enum SrcReload_enum mode ) 
{
	dmaSetSourceChannelDirectionDoublePuffering(channel, source, source, direction, mode);
}

void		dmaSetSourceChannelDirectionDoublePuffering		( uint8_t channel , void *source1 , void *source2 , enum SrcDirection_enum direction , enum SrcReload_enum mode ) 
{
	dmaaddr_t addr[2];
	addr[0] = CPTR2DMA( source1 );
	addr[1] = CPTR2DMA( source2 );
	
	if ( DOUBLE_BUFFERED( channel ) ) 
	{
		channel &= ~1;

		CHANNEL(channel).SRCADDR0		= addr[0];
		CHANNEL(channel).SRCADDR1		= addr[0] >> 8;
		CHANNEL(channel).SRCADDR2		= addr[0] >> 16;
		
		CHANNEL(channel + 1).SRCADDR0	= addr[1];
		CHANNEL(channel + 1).SRCADDR1	= addr[1] >> 8;
		CHANNEL(channel + 1).SRCADDR2	= addr[1] >> 16;

		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcDirection_bm) | direction;
		CHANNEL(channel + 1).ADDRCTRL = (CHANNEL(channel + 1).ADDRCTRL & ~SrcDirection_bm) | direction;

		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcReload_bm) | mode;
		CHANNEL(channel + 1).ADDRCTRL = (CHANNEL(channel + 1).ADDRCTRL & ~SrcReload_bm) | mode;

	} 
	else 
	{
		CHANNEL(channel).SRCADDR0 = addr[0];
		CHANNEL(channel).SRCADDR1 = addr[1] >> 8;
		CHANNEL(channel).SRCADDR2 = 0;
		
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcReload_bm) | mode;
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcDirection_bm) | direction;
	}
}


void		dmaSetSourceDirection							( uint8_t channel , enum SrcDirection_enum direction )	
{
	if ( DOUBLE_BUFFERED( channel ) ) 
	{
		channel &= ~1;
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcDirection_bm) | direction;
		CHANNEL(channel + 1).ADDRCTRL = (CHANNEL(channel + 1).ADDRCTRL & ~SrcDirection_bm) | direction;
	} 
	else 
	{
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcDirection_bm) | direction;
	}
}

void		dmaSetSourceReload								( uint8_t channel , enum SrcReload_enum mode )			
{
	if ( DOUBLE_BUFFERED( channel ) ) 
	{
		channel &= ~1;
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcReload_bm) | mode;
		CHANNEL(channel + 1).ADDRCTRL = (CHANNEL(channel + 1).ADDRCTRL & ~SrcReload_bm) | mode;
	} 
	else 
	{
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcReload_bm) | mode;
	}
}

void		*dmaSetManualDestination						( uint8_t channel , void *destination )					
{
	dmaaddr_t addr = CPTR2DMA( destination );
	
	if ( destination )
	{
		dma.autoDestBuffer &= ~_BV(channel);
		dmaUpdateAutoBuffer( channel , dma.blockSizes[channel] );
		CHANNEL(channel).DESTADDR0 = addr;
		CHANNEL(channel).DESTADDR1 = addr >> 8;
		CHANNEL(channel).DESTADDR2 = addr >> 16;
		return destination;

	} 
	else 
	{
		dma.autoDestBuffer |= _BV(channel);
		
		if ( dmaUpdateAutoBuffer( channel , dma.blockSizes[channel] ) )
		{
			dmaSetBuffer( channel );
			return &dma.buffer[channel];
		}
		
		return 0;
	}
}

void		*dmaSetDestination2								( uint8_t channel , void *destination, enum DestDirection_enum direction, enum DestReload_enum mode)						
{
	dmaSetDestination3(channel, destination, destination, direction, mode);
	
	if ( dma.autoDestBuffer & _BV( channel ) ) 
	{
		return &dma.buffer[channel];
	}
	
	return destination;
}

void		dmaSetDestination3				( uint8_t channel , void *destination1, void *destination2, enum DestDirection_enum direction, enum DestReload_enum mode) 
{
	dmaaddr_t addr[2];
	addr[0] = CPTR2DMA( destination1 );
	addr[1] = CPTR2DMA( destination2 );
	
	if ( DOUBLE_BUFFERED( channel ) ) 
	{
		channel &= ~1;
		CHANNEL(channel).ADDRCTRL		= ( CHANNEL( channel ).ADDRCTRL & ~DestDirection_bm) | direction;
		CHANNEL(channel + 1).ADDRCTRL	= ( CHANNEL( channel + 1 ).ADDRCTRL & ~DestDirection_bm) | direction;

		CHANNEL(channel).ADDRCTRL		= ( CHANNEL( channel).ADDRCTRL & ~DestReload_bm) | mode;
		CHANNEL(channel + 1).ADDRCTRL	= ( CHANNEL( channel + 1 ).ADDRCTRL & ~DestReload_bm) | mode;

		if ( destination1 ) 
		{
			dma.autoDestBuffer &= ~_BV(channel);
			dmaUpdateAutoBuffer( channel , dma.blockSizes[channel] );
			CHANNEL(channel).DESTADDR0 = addr[0];
			CHANNEL(channel).DESTADDR1 = addr[0] >> 8;
			CHANNEL(channel).DESTADDR2 = addr[0] >> 16;
		} 
		else 
		{
			dma.autoDestBuffer |= _BV(channel);
			if ( dmaUpdateAutoBuffer( channel , dma.blockSizes[channel] ) ) 
			{
				dmaSetBuffer( channel );
			}
		}

		if ( destination2 ) 
		{
			dma.autoDestBuffer &= ~_BV( channel + 1 );
			
			dmaUpdateAutoBuffer( channel + 1 , dma.blockSizes[channel + 1] );
			
			CHANNEL(channel + 1).DESTADDR0 = addr[1];
			CHANNEL(channel + 1).DESTADDR1 = addr[1] >> 8;
			CHANNEL(channel + 1).DESTADDR2 = addr[1] >> 16;
		} 
		else 
		{
			dma.autoDestBuffer |= _BV( channel + 1 );
			if ( dmaUpdateAutoBuffer( channel + 1 , dma.blockSizes[channel + 1] ) ) 
			{
				dmaSetBuffer( channel + 1 );
			}
		}
	} 
	else 
	{	
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~DestDirection_bm) | direction;
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~DestReload_bm) | mode;

		if ( destination1 ) 
		{
			dma.autoDestBuffer &= ~_BV(channel);
			dmaUpdateAutoBuffer( channel, dma.blockSizes[channel] );
			CHANNEL(channel).DESTADDR0 = addr[0];
			CHANNEL(channel).DESTADDR1 = addr[0] >> 8;
			CHANNEL(channel).DESTADDR2 = addr[0] >> 16;
		} 
		else 
		{
			dma.autoDestBuffer |= _BV( channel );
			if ( dmaUpdateAutoBuffer( channel , dma.blockSizes[channel] ) ) 
			{
				dmaSetBuffer( channel );
			}
		}
	}
}

void		dmaSetDestDirection				( uint8_t channel , enum DestDirection_enum direction)		
{
	if ( DOUBLE_BUFFERED( channel ) ) 
	{
		channel &= ~1;
		CHANNEL(channel).ADDRCTRL		= (CHANNEL(channel).ADDRCTRL & ~DestDirection_bm) | direction;
		CHANNEL(channel + 1).ADDRCTRL	= (CHANNEL(channel + 1).ADDRCTRL & ~DestDirection_bm) | direction;
	} 
	else 
	{
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~DestDirection_bm) | direction;
	}
}

void		dmaSetDestReload				( uint8_t channel , enum DestReload_enum mode )				
{
	if ( DOUBLE_BUFFERED( channel ) ) 
	{
		channel &= ~1;
		CHANNEL( channel ).ADDRCTRL		= ( CHANNEL( channel ).ADDRCTRL & ~DestReload_bm ) | mode;
		CHANNEL( channel + 1 ).ADDRCTRL = ( CHANNEL( channel + 1 ).ADDRCTRL & ~DestReload_bm ) | mode;
	} 
	else 
	{
		CHANNEL( channel ).ADDRCTRL = ( CHANNEL( channel ).ADDRCTRL & ~DestReload_bm ) | mode;
	}
}

void		dmaSetBurstLength				( uint8_t channel , enum BurstLength_enum burstLength )			
{
	if ( DOUBLE_BUFFERED( channel ) ) 
	{
		channel &= ~1;
		CHANNEL( channel ).CTRLA		= ( CHANNEL( channel ).CTRLA & ~BurstLength_bm ) | burstLength;
		CHANNEL( channel + 1 ).CTRLA	= ( CHANNEL( channel + 1 ).CTRLA & ~BurstLength_bm ) | burstLength;
	} 
	else 
	{
		CHANNEL(channel).CTRLA = ( CHANNEL( channel ).CTRLA & ~BurstLength_bm) | burstLength;
	}
}

void		dmaEnableController				( void )
{
	DMA.CTRL |= DMA_CH_ENABLE_bm;
}

void		dmaDisableController			( void )
{
	DMA.CTRL &= ~DMA_CH_ENABLE_bm;
}

void		dmaEnableChannel				( uint8_t channel )			
{
	if ( DOUBLE_BUFFERED( channel ) ) 
	{
		channel &= ~1;
		CHANNEL( channel ).CTRLA		|= DMA_CH_ENABLE_bm;
		CHANNEL( channel + 1 ).CTRLA  &= ~DMA_CH_ENABLE_bm;
	} 
	else 
	{
		CHANNEL( channel ).CTRLA |= DMA_CH_ENABLE_bm;
	}
}

void		dmaDisableChannel				( uint8_t channel )			
{	
	if ( DOUBLE_BUFFERED( channel ) ) 
	{
		channel &= ~1;
		CHANNEL( channel ).CTRLA		&= ~DMA_CH_ENABLE_bm;
		CHANNEL( channel + 1 ).CTRLA	&= ~DMA_CH_ENABLE_bm;
	} 
	else 
	{
		CHANNEL( channel ).CTRLA		&= ~DMA_CH_ENABLE_bm;
	}
}

bool		dmaIsTransactionComplete		( uint8_t channel )			
{
	bool result = ( DMA.INTFLAGS & _BV( channel ) ) == _BV( channel );
	
	if ( result )
	{
		DMA.INTFLAGS |= _BV( channel );
	}
	
	return result && ! dmaIsBlockTransferBusy( channel );
}

bool		dmaIsBlockTransferBusy			( uint8_t channel )			
{
	return ( CHANNEL( channel ).CTRLB & ( DMA_CH_CHBUSY_bm | DMA_CH_CHPEND_bm ) );
}

void		dmaTrigger						( uint8_t channel )			
{
	while ( CHANNEL( channel ).CTRLA & DMA_CH_TRFREQ_bm );
	CHANNEL( channel ).CTRLA |= DMA_CH_TRFREQ_bm;
}

bool		dmaGetFreeChannel				( uint8_t channel )			
{
	uint8_t c = 0;
	while ( c < 4 )
	{
		if ( dma.usedChannels & _BV( c ) ) 
		{
			break;
		}
		
		c++;
	}
	
	if ( c == 4 )
	{ 
		return false;
	}
	
	channel = c;
	
	return true;
}

bool		dmaCopySyncron					( void *source , void *destination , uint16_t size )					
{
	uint8_t channel = 0;
	if ( ! dmaGetFreeChannel ( channel ) )
	{
		return false;
	} 

	if ( ! dmaCopyAsychron( channel , source , destination , size ) )
	{
		return false;
	}
		

	while ( ! dmaIsTransactionComplete( channel ) );
	dmaUseChannel( channel , false );

	return true;
}

bool		dmaCopyAsychron					( uint8_t channel , void *source , void *destination , uint16_t size )	
{
	if ( dma.usedChannels & _BV( channel ) )
	{
		return false;
	} 
	
	dmaDisableChannel		( channel			);
	dmaSetChannel			( channel			);
	dmaUseDoubleBuffering	( channel , false	);
	dmaSetBlockSize			( channel , size	);
	dmaSetSourceChannelDirection			( channel , source , SrcDirectionInc , SrcReloadTransaction );
	dmaSetDestination2		( channel , destination , DestDirectionInc , DestReloadTransaction );
	dmaSetBurstLength		( channel , BurstLength1Byte );
	dmaSetRepeatCount		( channel , 1 );
	dmaSetSingleShot		( channel , false );
	dmaSetTriggerSource		( channel , DMA_CH_TRIGSRC_OFF_gc );
	dmaEnableChannel		( channel );
	dmaTrigger				( channel );

	return true;
}

bool		dmaFillMemory					( void *destination , uint16_t size , uint8_t byte )					
{
	uint8_t channel = 0;

	if ( ! dmaGetFreeChannel( channel ) )
	{
		return false;
	} 

	dmaDisableChannel		( channel );
	dmaSetChannel			( channel );
	dmaUseDoubleBuffering	( channel , false );
	dmaSetBlockSize			( channel , size );
	dmsSetSourceChannel		( channel , (uint32_t *) &byte );
	dmaSetSourceDirection	( channel , SrcDirectionFixed );
	dmaSetSourceReload		( channel , SrcReloadBurst );
	dmaSetManualDestination	( channel , destination );
	dmaSetDestDirection		( channel , DestDirectionInc );
	dmaSetDestReload		( channel , DestReloadTransaction );
	dmaSetBurstLength		( channel , BurstLength1Byte );
	dmaSetRepeatCount		( channel , 1 );
	dmaSetSingleShot		( channel , false );
	dmaSetTriggerSource		( channel , DMA_CH_TRIGSRC_OFF_gc );
	dmaEnableChannel		( channel );
	dmaTrigger				( channel );

	while ( ! dmaIsTransactionComplete( channel ) ){};
	dmaUseChannel( channel , false );

	return true;
}

void		dmaSetRepeatCount				( uint8_t channel , uint8_t repeatCount )		
{
	if ( repeatCount != 1 )
	{
		if ( DOUBLE_BUFFERED( channel ) )
		{
			channel &= ~1;
			CHANNEL( channel ).REPCNT		= repeatCount;
			CHANNEL( channel + 1 ).REPCNT = repeatCount;
			CHANNEL( channel).CTRLA		|= DMA_CH_REPEAT_bm;
			CHANNEL( channel + 1 ).CTRLA	|= DMA_CH_REPEAT_bm;
		} 
		else 
		{
			CHANNEL( channel ).REPCNT = repeatCount;
			CHANNEL( channel ).CTRLA |= DMA_CH_REPEAT_bm;
		}
	} 
	else 
	{
		if ( DOUBLE_BUFFERED( channel ) ) 
		{
			channel &= ~1;
			CHANNEL( channel ).CTRLA		&= ~DMA_CH_REPEAT_bm;
			CHANNEL( channel + 1 ).CTRLA	&= ~DMA_CH_REPEAT_bm;
		} 
		else 
		{
			CHANNEL( channel ).CTRLA		&= ~DMA_CH_REPEAT_bm;
		}
	}
}

void		dmaSetSingleShot				( uint8_t channel , bool use )					
{
	uint8_t iUse = (use) ? 1 : 0;
	if  (DOUBLE_BUFFERED( channel ) )
	{
		channel &= ~1;
		CHANNEL( channel ).CTRLA		= ( CHANNEL( channel ).CTRLA & ~DMA_CH_SINGLE_bm ) | ( iUse << DMA_CH_SINGLE_bp );
		CHANNEL( channel + 1 ).CTRLA	= ( CHANNEL( channel + 1 ).CTRLA & ~DMA_CH_SINGLE_bm ) | ( iUse << DMA_CH_SINGLE_bp );
	}
	else 
	{
		CHANNEL( channel ).CTRLA		= ( CHANNEL( channel ).CTRLA & ~DMA_CH_SINGLE_bm ) | ( iUse << DMA_CH_SINGLE_bp );
	}
}

void		dmaSetTriggerSource				( uint8_t channel , uint8_t triggerSource )		
{
	if ( DOUBLE_BUFFERED( channel ) )
	{
		channel &= ~1;
		CHANNEL( channel ).TRIGSRC		= triggerSource;
		CHANNEL( channel + 1 ).TRIGSRC	= triggerSource;
	} 
	else 
	{
		CHANNEL( channel ).TRIGSRC		= triggerSource;
	}
}


#undef CHANNEL
#undef DOUBLE_BUFFERED