
/* md09a direction */
#define RIGHT				0x01
#define LEFT			0x00

/* md09a sleep : high = work, low = sleep */
#define STEP_SLEEP		0x04
#define IGNORE			0x02
#define ENABLE			0x01
#define DISABLE			0x00

/* step setting */
#define STEP_FULL		0x00
#define STEP_HALF		0x01
#define STEP_QUAD		0x02
#define STEP_EIGH		0x03
#define STEP_SIXT		0x04

/* sleep pin */
#define STEP_SLEEP_DDR		DDRD
#define STEP_SLEEP_PORT		PORTD
#define STEP_SLEEP_PIN		6

/* enable pin */
#define STEP_EN_DDR			DDRC
#define STEP_EN_PORT		PORTC
#define STEP_EN_PIN			2

/* puls pin */
#define STEP_PULS_DDR		DDRC
#define STEP_PULS_PORT		PORTC
#define STEP_PULS_PIN		4

/* direction pin */
#define STEP_DIR_DDR		DDRC
#define STEP_DIR_PORT		PORTC
#define STEP_DIR_PIN		3

/* step "MS1" pin */
#define STEP_MS1_DDR		DDRD
#define STEP_MS1_PORT		PORTD
#define STEP_MS1_PIN		3

/* step "MS2" pin */		
#define STEP_MS2_DDR		DDRD
#define STEP_MS2_PORT		PORTD
#define STEP_MS2_PIN		4

/* step "MS3" pin */
#define STEP_MS3_DDR		DDRD
#define STEP_MS3_PORT		PORTD
#define STEP_MS3_PIN		5



/* initialize the md09a module */
/* Parameter	: none
*  Returns		: none
*/
void md09a_init(void);


/* handle the md09a module */
/* Parameter	:
*					steps		= steps for RIGHT or LEFT
*					direction	= RIGHT or LEFT
*					step_type	= STEP_FULL, STEP_HALF, STEP_QUAD, STEP_EIGH, STEP_SIXT
*
*	Returns		:	
*					0x00		= command successfully 
*					0x01		= direction error
*					0x02		= step type error
*					0x03		= sleep error
*/
uint8_t md09a_command(uint16_t steps, uint8_t direction , uint8_t step_type , uint8_t sleep);
