#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define RBUFsize		256

/* RinG BUFFER */
typedef struct
{
	uint8_t		in;  			/**< Current storage location in the circular buffer. */
	uint8_t		out;  			/**< Current retrieval location in the circular buffer. */
	uint16_t	size;  			/**< size of the buffer's underlying storage array. */
	uint16_t	count;  		/**< Number of bytes currently stored in the buffer. */
	char 		buff[RBUFsize];
	
}ringBuffer_t;
ringBuffer_t ringBuffer;


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


int main(int argc, char *argv[]) 
{
	ringBufferInit();
	
	char stream[] = "Serial Input"; // Eingangspuffer
	char *ptr = stream; // Zeiger auf den Eingangspuffer
	
	
	while( *ptr ) // Ringpuffer befüllen..
	{
		ringBufferInsert( *ptr++ );
	}
		
	printf( "\r\n" );
	printf( "Buffer Get Count[VORHER] : %d\r\n" , ringBufferGetCount() ); // Anzahl der Bytes im Speicher
	printf( "Buffer Free Size[VORHER] : %d\r\n" , ringBufferGetFreeCount( ) ); // Anzahl freier Speicherplätze

	uint8_t x;
	for ( x = 0 ; !ringBufferIsEmpty() ; x++ ) // Solange Daten aus dem Ringpuffer holen bis leer
	{
		printf( "%c" , ringBufferGet() );	
	}

	printf( "\r\n" );
	printf( "Buffer Get Count[NACHER] : %d\r\n" , ringBufferGetCount() );
	printf( "Buffer Free Size[NACHER] : %d\r\n" , ringBufferGetFreeCount( ) );	
	printf( "Buffer Get Peek          : %c\r\n" , ringBufferPeek() );
	
	return 0;
}
