#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>



void 		ringBufferInit				( void )
{
	ringBuffer.in		= 0;
	ringBuffer.out 		= 0;
	ringBuffer.size 	= RBUFsize;
	ringBuffer.count 	= 0;
}

uint16_t 	ringBufferGetCount			( void )
{
	return ringBuffer.count;	
}

uint16_t 	ringBufferGetFreeCount		( void )
{
	return ( ringBuffer.size - ringBufferGetCount() );
}

bool 		ringBufferIsEmpty			( void )
{
	return ( ringBufferGetCount() == 0 );
}

bool 		ringBufferIsFull			( void )
{
	return ( ringBufferGetCount() == ringBuffer.size );	
}

void 		ringBufferInsert			( char data )
{
	ringBuffer.buff[ringBuffer.in++] = data;
	
	if (ringBuffer.in == ringBuffer.size)
	{
		ringBuffer.in = 0;
	}
	
	ringBuffer.count++;
}

char 		ringBufferGet				( void )
{
	uint8_t ret = ringBuffer.buff[ringBuffer.out];

	ringBuffer.out++;
	if ( ringBuffer.out == ringBuffer.size )
	{
		ringBuffer.out = 0;
	}
	
	ringBuffer.count--;
	
	return ret;
}

uint8_t 	ringBufferPeek				( void )
{
	return ( ringBuffer.buff[ringBuffer.out] );
}
