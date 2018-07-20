/*
 * Copyright:   Jan Homann
 * Author:      Jan Homann
 * Version:     1.0
 * Date:		23.06.2015
 */


/* load parallel bits into 165A register (PL) */
#define LD_DDR		DDRB
#define LD_PORT		PORTB
#define LD_PIN		PB2

/* shift clock (CP) */
#define CLK_DDR		DDRB
#define CLK_PORT	PORTB
#define CLK_PIN		PB6

/* shift clock */
//#define with_clk_inh

#ifdef with_clk_inh
	#define INH_DDR		
	#define INH_PORT
	#define INH_PIN
#endif

/* serial out (Q7) */
#define INPUT_DDR	DDRB
#define INPUT_PORT	PINB
#define INPUT_PIN	PB3



/* init the pins for 74xx165A */
/* 
* parameter : none
* return : none
*/
void init_165A(void);

/* read pin state @ 74xx165A*/
/*
* parameter : none
* return : state from the pins @ 74xx165A
*/
uint8_t read_SR_165A(void);
