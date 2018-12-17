
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#include "RingBuffer.h"


static uint8_t Length = 0;


static void RingBufferAdd( RingBuff_t *RingBuff , uint8_t *Data , uint8_t Offset , uint8_t Length )
{
	uint16_t i = 0;
	uint8_t *tmpPtr = Data;
	
	if ( RingBuff->WritePos + Length > RingBuff->BufferSize )
	{
		return; // no free space
	}
	
	for ( ; i < Length ; i++ )
	{
		RingBuff->Buffer[Offset+i] = *tmpPtr++;
	}
}

RingBuff_t RingBufferCreate( uint8_t *Buffer , uint16_t Capacity )
{	
	RingBuff_t RingBuffer;

	RingBuffer.ReadPos = 0;
	RingBuffer.WritePos = 0;
	RingBuffer.IsFull = false;
	
	RingBuffer.Buffer = Buffer;
	
	RingBuffer.BufferSize = Capacity;

	return RingBuffer;
}

uint8_t RingBufferGetLength( RingBuff_t *RingBuff )
{
	if ( RingBuff->IsFull == true )
	{
		return RingBuff->BufferSize;
	}
	else
	{
		if ( RingBuff->ReadPos == RingBuff->WritePos )
		{
			Length = 0;
		}
		else if ( RingBuff->ReadPos < RingBuff->WritePos )
		{
			Length = RingBuff->WritePos -RingBuff->ReadPos;
		}
		else
		{
			Length = (RingBuff->BufferSize - RingBuff->ReadPos) + RingBuff->WritePos;
		}
	}
	
	return Length;
}

void RingBufferClear( RingBuff_t *RingBuff )
{
	RingBuff->ReadPos = 0;
	RingBuff->WritePos = 0;
}

uint8_t RingBufferPush( RingBuff_t *RingBuff , uint8_t *Data , uint8_t Length )
{
	if ( Data == NULL || Length == 0 || RingBuff->Buffer == NULL )
	{
		return 1;
	}

	if ( RingBuff->WritePos + Length <= RingBuff->BufferSize )
	{
		RingBufferAdd( RingBuff , Data , RingBuff->WritePos , Length );
		
		RingBuff->WritePos += Length;
	}
	else
	{
		int16_t i = (RingBuff->WritePos + Length) - RingBuff->BufferSize;
			
		RingBufferAdd( RingBuff , Data , RingBuff->WritePos , (RingBuff->BufferSize - RingBuff->WritePos) );
		RingBufferAdd( RingBuff , Data+(Length-i) , 0 , i );
		
		RingBuff->WritePos = i;
	}
	
	if ( RingBuff->ReadPos == RingBuff->WritePos )
	{
		RingBuff->IsFull = true;
	}
	
	return 0;
}

uint8_t RingBufferPull( RingBuff_t *RingBuffer , uint8_t *Buff , uint8_t Length )
{
	if ( Length <= 0 )
	{
		#ifdef RINGBUFFER_DEBUG_ENABLE
			printf("RingBufferPull(): Length <= 0\r\n");
		#endif
		return 1;
	}
	
	if ( Length > RingBuffer->BufferSize )
	{
		#ifdef RINGBUFFER_DEBUG_ENABLE
			printf("RingBufferPull(): Length > RingBuffer->BufferSize\r\n");
		#endif
		return 2;
	}
	
	
	if ( (RingBuffer->ReadPos + Length) <= RingBuffer->BufferSize )
	{
		int16_t x = 0;
		for ( ; x < Length ; x++ )
		{
			Buff[x] = RingBuffer->Buffer[RingBuffer->ReadPos+x];
		}RingBuffer->ReadPos += Length;
	}
	else
	{
		int16_t l = (RingBuffer->ReadPos + Length) - RingBuffer->BufferSize;
		if ( RingBuffer->ReadPos < l )
		{
			#ifdef RINGBUFFER_DEBUG_ENABLE
				printf("RingBufferPull(): RingBuffer->ReadPos < l\r\n");
			#endif
			return 3; // Requested data length larger than stored data. Buffer underrun!
		}
		
		int16_t x = 0;
		for ( ; x < Length - l ; x++ )
		{
			Buff[x] = RingBuffer->Buffer[RingBuffer->ReadPos+x];
		}
			
		for ( x = 0 ; x < l ; x++ )
		{
			Buff[(Length-l)+x] = RingBuffer->Buffer[x];
		}
					
		RingBuffer->ReadPos = l;
	}
	
	return 0;
}
