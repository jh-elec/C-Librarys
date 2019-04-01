#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "C:\Users\Jan\Desktop\C-Librarys\trunk\RingBuffer\RingBuffer.h"
#include "C:\Users\Jan\Desktop\C-Librarys\trunk\RingBuffer\RingBuffer.c"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) 
{
	uint8_t Buffer[8]="";
	RingBuffer_t RingBuffer;
	
	RingBufferInit( &RingBuffer, Buffer , sizeof(Buffer));
	
	uint8_t Entry[] = {1,2,3,4,5,6};
	
	RingBufferWriteBurst(&RingBuffer, Entry , sizeof(Entry));
	
	Burst_Info_t BurstInfo = RingBufferReadBurst(&RingBuffer,5);
		
	
	printf("%d", BurstInfo.Status );
	
	return 0;
}
