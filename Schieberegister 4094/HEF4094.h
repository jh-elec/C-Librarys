/*
*	made by		: Jan Homann (C)
*	Copyright	: Jan Homann (C)
*	Date		: 20.05.2015
*/


#define HEF4094_DATA_DDR			DDRF
#define HEF4094_DATA_PORT			PORTF
#define HEF4094_DATA_PIN			PF2

#define HEF4094_CLOCK_DDR			DDRD
#define HEF4094_CLOCK_PORT			PORTD
#define HEF4094_CLOCK_PIN			PD1

#define HEF4094_STROBE_DDR			DDRF
#define HEF4094_STROBE_PORT			PORTF
#define HEF4094_STROBE_PIN			PF2



/***Prototype functions***/

/* init the Pins */
/* 
*	parameter	: none
*	return		: none
*/
void HEF4094_init(void);

/* shift data serial out */
/* 
*	parameter	: mask = z.B 0b11110000(BIN) or 0x1A(HEX)  or 12(Dec)
*	return		: none
*	++INFO++
*	sends the same command only once!
*/
void HEF4094_send_Data(uint8_t mask);
