
#define RBUFSIZE		256

/* RING BUFFER */
typedef struct
{
	uint8_t		In;  /**< Current storage location in the circular buffer. */
	uint8_t		Out;  /**< Current retrieval location in the circular buffer. */
	uint16_t	Size;  /**< Size of the buffer's underlying storage array. */
	uint16_t	Count;  /**< Number of bytes currently stored in the buffer. */
} RingBuffer_t;
RingBuffer_t RingBufferInfo;

/* CAN RING BUFFER */
typedef struct {
	uint16_t	Message_ID;	// Identifier of Message
	uint8_t		DataLength; // Data Bytes of Message
	uint8_t		Data[8];	// Data 0 - 8 max.
}CANMSG_t;

CANMSG_t RingBuffer[RBUFSIZE];
 CANMSG_t CAN_RingBuffer;



void RingBuffer_InitBuffer(void);

uint16_t RingBuffer_GetCount(void);

uint16_t RingBuffer_GetFreeCount(void);

bool RingBuffer_IsEmpty(void);

bool RingBuffer_IsFull(void);

void RingBuffer_Insert(CANMSG_t CAN_MESSAGE);

void RingBuffer_Remove(CANMSG_t *CAN_STRUCT);

uint8_t RingBuffer_Peek(void);