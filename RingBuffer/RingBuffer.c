
#include "RingBuffer.h"


void		RingBufferInit( RingBuffer_t *RingBuff , void *Buff , uint16_t BufferSize )
{
	RingBuff->BufferPtr = Buff;
	RingBuff->BufferSize = (BufferSize - 1);
	RingBuff->NewestIndex = 0;
	RingBuff->OldestIndex = 0;
}

void		RingBufferClear( RingBuffer_t *RingBuff )
{
	RingBuff->Entrys = 0;
	RingBuff->NewestIndex = 0;
	RingBuff->OldestIndex = 0;
}

size_t		RingBufferLength( RingBuffer_t *RingBuff )
{
	return (RingBuff->NewestIndex-RingBuff->OldestIndex);
}


enum RingBufferStatus RingBufferWrite(volatile RingBuffer_t *RingBuff , uint8_t Byte )
{
	uint8_t NextIndex = (((RingBuff->NewestIndex)+1) % RingBuff->BufferSize);
	
	if (NextIndex == RingBuff->OldestIndex)
	{
		return BUFFER_FULL;
	}
	
	RingBuff->BufferPtr[RingBuff->NewestIndex] = Byte;
	
	RingBuff->NewestIndex = NextIndex;
	
	return BUFFER_OK;
}

enum RingBufferStatus RingBufferRead(volatile RingBuffer_t *RingBuff , uint8_t *Byte)
{
	if (RingBuff->NewestIndex == RingBuff->OldestIndex)
	{
		return BUFFER_EMPTY;
	}
	
	*Byte = RingBuff->BufferPtr[RingBuff->OldestIndex];
	
	RingBuff->OldestIndex = ((RingBuff->OldestIndex+1) % RingBuff->BufferSize);
	
	return BUFFER_OK;
}

enum RingBufferStatus RingBufferPeek(volatile RingBuffer_t *RingBuff , uint8_t *Byte)
{
	uint8_t LastIndex = ((RingBuff->BufferSize + (RingBuff->NewestIndex) - 1) % RingBuff->BufferSize);
	
	if (RingBuff->NewestIndex == RingBuff->OldestIndex)
	{
		return BUFFER_EMPTY;
	}
	
	*Byte = RingBuff->BufferPtr[LastIndex];
	
	return BUFFER_OK;
}


uint16_t RingBufferReadByte( volatile RingBuffer_t *RingBuff )
{
	uint8_t ReadedByte;
	
	enum RingBufferStatus BufferStatus;
	BufferStatus = RingBufferRead( RingBuff , &ReadedByte );
	
	return ((uint16_t)(BufferStatus << 8) + (uint8_t)ReadedByte);
}


enum RingBufferStatus RingBufferWriteBurst(volatile RingBuffer_t *RingBuff , uint8_t *Source , uint16_t Length )
{
	enum RingBufferStatus BufferState;
	for ( uint16_t i = 0 ; i < Length ; i++ )
	{
		BufferState = RingBufferWrite( RingBuff , *Source+i );
		if ( BufferState != BUFFER_OK )
		{
			return BufferState;
		}
	}
	
	return BUFFER_OK;
}

Burst_Info_t RingBufferReadBurst(volatile RingBuffer_t *RingBuff , uint16_t Length )
{
	Burst_Info_t BurstInfo;
	
	BurstInfo.Status = 0;
	BurstInfo.DataPtr = NULL;
	
	BurstInfo.Status = RingBufferRead( RingBuff, BurstInfo.DataPtr );
	if ( BurstInfo.Status != BUFFER_OK )
	{
		return BurstInfo;
	}
	
	uint8_t tmp;
	for ( uint16_t i = 0 ; i < (Length-1) ; i-- ) // -1 da wir schon ein Byte gelesen haben
	{
		BurstInfo.Status = RingBufferRead( RingBuff , &tmp );
		if ( BurstInfo.Status != BUFFER_OK )
		{
			return BurstInfo;
		}
	}
	
	BurstInfo.Status = BUFFER_OK;
	
	return BurstInfo;
}