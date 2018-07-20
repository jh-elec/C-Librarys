#include <avr/interrupt.h>
#include <stdbool.h>

#include "RingBuffer.h"


void RingBuffer_InitBuffer(void)
{
	uint8_t int_reg = SREG;
	cli();
	
	RingBufferInfo.In  = 0;
	RingBufferInfo.Out = 0;
	RingBufferInfo.Size = RBUFSIZE;
	RingBufferInfo.Count = 0;
	
	SREG = int_reg;	
}

uint16_t RingBuffer_GetCount(void)
{
	uint8_t Count;
	
	uint8_t int_reg = SREG;
	cli();
	Count = RingBufferInfo.Count;
	SREG = int_reg;
	
	return Count;
}

uint16_t RingBuffer_GetFreeCount(void)
{
	
	return (RingBufferInfo.Size - RingBuffer_GetCount());
	
}

bool RingBuffer_IsEmpty(void)
{
	return (RingBuffer_GetCount() == 0);
}

bool RingBuffer_IsFull(void)
{
	return (RingBuffer_GetCount() == RingBufferInfo.Size);	
}

void RingBuffer_Insert(CANMSG_t CAN_MESSAGE)
{
	RingBuffer[RingBufferInfo.In].Message_ID = CAN_MESSAGE.Message_ID; // contains actual id from message x
	RingBuffer[RingBufferInfo.In].DataLength = CAN_MESSAGE.DataLength; // contains actual data length from message x
	
	/* store actual data from message x */
	for (uint8_t x = 0 ; x < 8 ; x++)
	{
		RingBuffer[RingBufferInfo.In].Data[x] = CAN_MESSAGE.Data[x];
	}
	
	RingBufferInfo.In++;
	
	if (RingBufferInfo.In == RingBufferInfo.Size)
	RingBufferInfo.In = 0;
	
	uint8_t int_reg = SREG;
	cli();
	
	RingBufferInfo.Count++;
	
	SREG = int_reg;
}

void RingBuffer_Remove(CANMSG_t *CAN_STRUCT)
{
	uint8_t int_reg = SREG;
	cli();
	
	// Diesen Block hier anpassen
	(*CAN_STRUCT).Message_ID = RingBuffer[RingBufferInfo.Out].Message_ID;
	(*CAN_STRUCT).DataLength = RingBuffer[RingBufferInfo.Out].DataLength;
	
	for (uint8_t i = 0 ; i < 8 ; i++)
	{
		(*CAN_STRUCT).Data[i] = RingBuffer[RingBufferInfo.Out].Data[i];
	} 
	// Block Ende
	
	RingBufferInfo.Out++;
	if (RingBufferInfo.Out == RingBufferInfo.Size)
	RingBufferInfo.Out = 0;
	
	RingBufferInfo.Count--;
	SREG = int_reg;
	
}

uint8_t RingBuffer_Peek(void)
{
	return (RingBuffer[RingBufferInfo.Out].Data[0]);
}