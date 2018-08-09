
#include <stdbool.h>
#include "ringbuff.h"


void 		ringBufferinit				( void )
{
	ringBuffer.in		= 0;
	ringBuffer.out 		= 0;
	ringBuffer.size 	= RBUFsize;
	ringBuffer.count 	= 0;
}

uint16_t 	ringBufferGetcount			( void )
{
	return ringBuffer.count;	
}

uint16_t 	ringBufferGetFreecount		( void )
{
	return (ringBuffer.size - ringBufferGetcount());
}

bool 		ringBufferIsEmpty			( void )
{
	return ( ringBufferGetcount() == 0 );
}

bool 		ringBufferIsFull			( void )
{
	return ( ringBufferGetcount() == ringBuffer.size );	
}

void 		ringBufferinsert			( char data )
{
	ringBuffer.buff[ringBuffer.in++];
	
	if (ringBuffer.in == ringBuffer.size)
	{
		ringBuffer.in = 0;
	}
	
	ringBuffer.count++;
}

void 		ringBufferRemove			( char data )
{
	ringBuffer.out++;
	if ( ringBuffer.out == ringBuffer.size )
	ringBuffer.out = 0;
	
	ringBuffer.count--;
}

uint8_t 	ringBufferPeek				( void )
{
	return ( ringBuff.buff[ringBuffer.out] );
}
