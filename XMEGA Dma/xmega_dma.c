/*
 * DMAController.cpp
 *
 *  Created on: 16.01.2013
 *      Author: Nicolas Göddel
 *
 *	https://www.mikrocontroller.net/articles/Speicherdirektzugriff_(DMA)_mit_dem_ATxmega#DMAController.cpp
 */

#include "DMAController.h"

#define DOUBLE_BUFFERED(channel)			( dma.doubleBuffered & _BV ( ( channel ) >> 1 ) )
#define CHANNEL(channel)					( ( ( DMA_CH_t * ) &DMA.CH0 )[channel])


typedef struct  
{
	uint8_t		usedChannels;
	uint8_t		doubleBuffered;
	uint8_t		autoDestBuffer;
	uint16_t	blockSizes[4];
	uint8_t	   *buffer[4];	
}dma_t;
dma_t dma;



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
			free( dma.buffer[channel] );
			dma.buffer[channel] = 0;
		}
	} else 
	{
		/*
		 * Ist schon ein dma.buffer mit anderer Größe alloziert, lösche den alten
		 * und alloziere einen neuen.
		 */
		if ( ( dma.buffer[channel] ) && ( dma.blockSizes[channel] != blockSize ) ) 
		{
			free( dma.buffer[channel] );
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
		CHANNEL(channel).DESTADDR0 = ((uintptr_t) dma.buffer[channel] & 0xff) ;
		CHANNEL(channel).DESTADDR1 = ((uintptr_t) dma.buffer[channel] >> 8) & 0xff;
		CHANNEL(channel).DESTADDR2 = ((uintptr_t) dma.buffer[channel] >> 16) & 0xff;
	}
}

void		dmaReset						( void )									
{
	// reset DMA controller
	DMA.CTRL = 0;
	DMA.CTRL = DMA_RESET_bm;
	
	while ( ( DMA.CTRL & DMA_RESET_bm ) != 0 );
	
	dma.UsedChannels = 0;
	dma.doubleBuffered = 0;
	dma.autoDestBuffer = 0;
	
	for ( uint8_t i = 0 ; i < 3 ; i++ ) 
	{
		dma.blockSizes[i] = 0;
		
		if (dma.buffer[i])
		{
			free(dma.buffer[i]);
		}
			
		dma.buffer[i] = 0;
	}
}

void		dmaSetBlockSize					( uint8_t channel , uint16_t blockSize )	
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

void		dmaUseChannel					( uint8_t channel , uint8_t use )			
{
	dma.UsedChannels = ( dma.UsedChannels & ~_BV( channel ) ) | ( use << channel );
	if ( !use ) 
	{
		disable( channel );
		if (DOUBLE_BUFFERED( channel ) ) 
		{
			dma.doubleBuffered &= ~_BV( channel >> 1 );
			dma.doubleBuffered( channel , false );
		}
	}
}

void		dmaSetSource					( uint8_t channel , void* source )			
{
	CHANNEL(channel).SRCADDR0 = ( uintptr_t )	source & 0xff;
	CHANNEL(channel).SRCADDR1 = ( ( uintptr_t )	source >> 8 ) & 0xff;
	CHANNEL(channel).SRCADDR2 = ( ( uintptr_t ) source >> 16 ) & 0xff;
}

void		dmaSetSource					( uint8_t channel , void* source , SrcDirection_enum direction , SrcReload_enum mode ) 
{
	setSource(channel, source, source, direction, mode);
}

void		dmaSetSource					( uint8_t channel , void* source1 , void* source2 , SrcDirection_enum direction , SrcReload_enum mode ) {
	if (DOUBLE_BUFFERED(channel)) {
		channel &= ~1;

		CHANNEL(channel).SRCADDR0 = (uintptr_t) source1 & 0xff;
		CHANNEL(channel).SRCADDR1 = ((uintptr_t) source1 >> 8) & 0xff;
		CHANNEL(channel).SRCADDR2 = ((uintptr_t) source1 >> 16) & 0xff;
		CHANNEL(channel + 1).SRCADDR0 = (uintptr_t) source2 & 0xff;
		CHANNEL(channel + 1).SRCADDR1 = ((uintptr_t) source2 >> 8) & 0xff;
		CHANNEL(channel + 1).SRCADDR2 = ((uintptr_t) source2 >> 16) & 0xff;

		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcDirection_bm) | direction;
		CHANNEL(channel + 1).ADDRCTRL = (CHANNEL(channel + 1).ADDRCTRL & ~SrcDirection_bm) | direction;

		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcReload_bm) | mode;
		CHANNEL(channel + 1).ADDRCTRL = (CHANNEL(channel + 1).ADDRCTRL & ~SrcReload_bm) | mode;

	} else {
		CHANNEL(channel).SRCADDR0 = (uintptr_t) source1 & 0xff;
		CHANNEL(channel).SRCADDR1 = ((uintptr_t) source1 >> 8) & 0xff;
		CHANNEL(channel).SRCADDR2 = ((uintptr_t) source1 >> 16) & 0xff;
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcReload_bm) | mode;

		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcDirection_bm) | direction;
	}
}


void		dmaSetSourceDirection			( uint8_t channel , SrcDirection_enum direction )	{
	if (DOUBLE_BUFFERED(channel)) 
	{
		channel &= ~1;
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcDirection_bm) | direction;
		CHANNEL(channel + 1).ADDRCTRL = (CHANNEL(channel + 1).ADDRCTRL & ~SrcDirection_bm) | direction;
	} else {
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcDirection_bm) | direction;
	}
}

void		dmaSetSourceReload				( uint8_t channel , SrcReload_enum mode )			{
	if (DOUBLE_BUFFERED(channel)) {
		channel &= ~1;
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcReload_bm) | mode;
		CHANNEL(channel + 1).ADDRCTRL = (CHANNEL(channel + 1).ADDRCTRL & ~SrcReload_bm) | mode;
	} else {
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~SrcReload_bm) | mode;
	}
}

void		*dmaSetDestination				( uint8_t channel , void* destination )				{
	if (destination) {
		dma.autoDestBuffer &= ~_BV(channel);
		updateAutodma.buffer(channel, dma.blockSizes[channel]);
		CHANNEL(channel).DESTADDR0 = (uintptr_t) destination & 0xff;
		CHANNEL(channel).DESTADDR1 = ((uintptr_t) destination >> 8) & 0xff;
		CHANNEL(channel).DESTADDR2 = ((uintptr_t) destination >> 16) & 0xff;
		return destination;

	} else {
		dma.autoDestBuffer |= _BV(channel);
		if (updateAutodma.buffer(channel, dma.blockSizes[channel])) {
			setdma.buffer(channel);
			return dma.buffer[channel];
		}
		return 0;
	}
}

void		*dmaSetDestination				( uint8_t channel , void* destination, DestDirection_enum direction, DestReload_enum mode)						{
	setDestination(channel, destination, destination, direction, mode);
	if (dma.autoDestBuffer & _BV(channel)) {
		return dma.buffer[channel];
	}
	return destination;
}

void		dmaSetDestination				( uint8_t channel , void* destination1, void* destination2, DestDirection_enum direction, DestReload_enum mode) {
	if (DOUBLE_BUFFERED(channel)) {
		channel &= ~1;
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~DestDirection_bm) | direction;
		CHANNEL(channel + 1).ADDRCTRL = (CHANNEL(channel + 1).ADDRCTRL & ~DestDirection_bm) | direction;

		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~DestReload_bm) | mode;
		CHANNEL(channel + 1).ADDRCTRL = (CHANNEL(channel + 1).ADDRCTRL & ~DestReload_bm) | mode;

		if (destination1) {
			dma.autoDestBuffer &= ~_BV(channel);
			updateAutodma.buffer(channel, dma.blockSizes[channel]);
			CHANNEL(channel).DESTADDR0 = (uintptr_t) destination1 & 0xff;
			CHANNEL(channel).DESTADDR1 = ((uintptr_t) destination1 >> 8) & 0xff;
			CHANNEL(channel).DESTADDR2 = ((uintptr_t) destination1 >> 16) & 0xff;
		} else {
			dma.autoDestBuffer |= _BV(channel);
			if (updateAutodma.buffer(channel, dma.blockSizes[channel])) {
				setdma.buffer(channel);
			}
		}

		if (destination2) {
			dma.autoDestBuffer &= ~_BV(channel + 1);
			updateAutodma.buffer(channel + 1, dma.blockSizes[channel + 1]);
			CHANNEL(channel + 1).DESTADDR0 = (uintptr_t) destination2 & 0xff;
			CHANNEL(channel + 1).DESTADDR1 = ((uintptr_t) destination2 >> 8) & 0xff;
			CHANNEL(channel + 1).DESTADDR2 = ((uintptr_t) destination2 >> 16) & 0xff;
		} else {
			dma.autoDestBuffer |= _BV(channel + 1);
			if (updateAutodma.buffer(channel + 1, dma.blockSizes[channel + 1])) {
				setdma.buffer(channel + 1);
			}
		}


	} else {
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~DestDirection_bm) | direction;

		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~DestReload_bm) | mode;

		if (destination1) {
			dma.autoDestBuffer &= ~_BV(channel);
			updateAutodma.buffer(channel, dma.blockSizes[channel]);
			CHANNEL(channel).DESTADDR0 = (uintptr_t) destination1 & 0xff;
			CHANNEL(channel).DESTADDR1 = ((uintptr_t) destination1 >> 8) & 0xff;
			CHANNEL(channel).DESTADDR2 = ((uintptr_t) destination1 >> 16) & 0xff;
		} else {
			dma.autoDestBuffer |= _BV(channel);
			if (updateAutodma.buffer(channel, dma.blockSizes[channel])) {
				setdma.buffer(channel);
			}
		}
	}
}

void		dmaSetDestDirection				( uint8_t channel , DestDirection_enum direction)		{
	if (DOUBLE_BUFFERED(channel)) {
		channel &= ~1;
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~DestDirection_bm) | direction;
		CHANNEL(channel + 1).ADDRCTRL = (CHANNEL(channel + 1).ADDRCTRL & ~DestDirection_bm) | direction;
	} else {
		CHANNEL(channel).ADDRCTRL = (CHANNEL(channel).ADDRCTRL & ~DestDirection_bm) | direction;
	}
}

void		dmaSetDestReload				( uint8_t channel , DestReload_enum mode )				
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

void		dmaSetBurstLength				( uint8_t channel , BurstLength_enum burstLength )		
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

void		dmaEnable						( uint8_t channel )			
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

void		dmaDisable						( uint8_t channel )			
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
		if ( dma.UsedChannels & _BV( c ) ) 
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

bool		dmaCopyMemory					( void* source , void* destination , uint16_t size )					
{
	uint8_t channel;
	if ( ! dmaGetFreeChannel ( channel ) )
	{
		return false;
	} 

	if ( ! dmaCopyMemory( channel , source , destination , size ) )
	{
		return false;
	}
		

	while ( ! dmaIsTransactionComplete( channel ) );
	dmaUseChannel( channel , false );

	return true;
}

bool		dmaCopyMemory					( uint8_t channel , void* source , void* destination , uint16_t size )	
{
	if ( dma.UsedChannels & _BV( channel ) )
	{
		return false;
	} 
	
	dmaDisable				( channel			);
	dmaUseChannel			( channel			);
	dmaUseDoubleBuffering	( channel , false	);
	dmaSetBlockSize			( channel , size	);
	dmaSetSource			( channel , source , SrcDirectionInc , SrcReloadTransaction );
	dmaSetDestination		( channel , destination , DestDirectionInc , DestReloadTransaction );
	dmaSetBurstLength		( channel , BurstLength1Byte );
	dmaSetRepeatCount		( channel , 1 );
	dmaUseSingleShot		( channel , false );
	dmaSetTriggerSource		( channel , DMA_CH_TRIGSRC_OFF_gc );
	dmaEnable				( channel );
	dmaTrigger				( channel );

	return true;
}

bool		dmaFillMemory					( void* destination , uint16_t size , uint8_t byte )					
{
	uint8_t channel;

	if ( ! dmaGetFreeChannel( channel ) )
	{
		return false;
	} 

	dmaDisable				( channel );
	dmaUseChannel			( channel );
	dmaUseDoubleBuffering	( channel , false );
	dmaSetBlockSize			( channel , size );
	dmaSetSource			( channel , &byte );
	dmaSetSourceDirection	( channel , SrcDirectionFixed );
	dmaSetSourceReload		( channel , SrcReloadBurst );
	dmaSetDestination		( channel , destination );
	dmaSetDestDirection		( channel , DestDirectionInc );
	dmaSetDestReload		( channel , DestReloadTransaction );
	dmaSetBurstLength		( channel , BurstLength1Byte );
	dmaSetRepeatCount		( channel , 1 );
	dmaUseSingleShot		( channel , false );
	dmaSetTriggerSource		( channel , DMA_CH_TRIGSRC_OFF_gc );
	dmaEnable				( channel );
	dmaTrigger				( channel );

	while ( ! dmaIsTransactionComplete( channel ) );
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

void		dmaUseSingleShot				( uint8_t channel , bool use )					
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