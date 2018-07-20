/*
 * self_init.c
 *
 * Created: 09.03.2016 12:39:34
 * Author : Hm
 */ 

#define F_CPU 16000000

#define LCD_DATA0_PORT   PORTE     /**< port for 4bit data bit 0 */
#define LCD_DATA1_PORT   PORTD     /**< port for 4bit data bit 1 */
#define LCD_DATA2_PORT   PORTD     /**< port for 4bit data bit 2 */
#define LCD_DATA3_PORT   PORTC     /**< port for 4bit data bit 3 */
#define LCD_DATA0_PIN    6            /**< pin for 4bit data bit 0  */
#define LCD_DATA1_PIN    4            /**< pin for 4bit data bit 1  */
#define LCD_DATA2_PIN    7            /**< pin for 4bit data bit 2  */
#define LCD_DATA3_PIN    6            /**< pin for 4bit data bit 3  */
#define LCD_RS_PORT      PORTD     /**< port for RS line         */
#define LCD_RS_PIN       0            /**< pin  for RS line         */

#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
/* on ATmega64/128 PINF is on port 0x00 and not 0x60 */
#define PIN(x) ( &PORTF==&(x) ? _SFR_IO8(0x00) : (*(&x - 2)) )
#else
#define PIN(x) (*(&x - 2))    /* address of input register of port x          */
#endif

//#define  LCD_READ_REQUIRED

// #if LCD_READ_REQUIRED
// #define LCD_RW_PORT      PORTA     /**< port for RW line         */
// #define LCD_RW_PIN       6            /**< pin  for RW line         */
// #endif

#define LCD_E_PORT       	PORTD     /**< port for Enable line     */
#define LCD_E_PIN        	1            /**< pin  for Enable line     */


#define LCD_ENTRY_MODE      2      /* DB2: set entry mode                 */
#define LCD_ENTRY_INC       1      /*   DB1: 1=increment, 0=decrement     */
#define LCD_MODE_DEFAULT    ((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC) )

#define DATA 				1	
#define COMMAND 			0


#include <avr/io.h>
#include <util/delay.h>


void lcd_command(unsigned char c);
void lcd_e_toggle(void);
void lcd_command(unsigned char c unsigned char dataORcommand);

int main(void)
{

	lcd_init();

    while (1) 
    {
    }
}

void lcd_command(unsigned char c unsigned char dataORcommand)// send command or data to lcd
{	
	if(dataORcommand)
	{
		LCD_RS_PORT |= (1<<LCD_RS_PIN); // write data
	}
	else
	{
		LCD_RS_PORT &= ~(1<<LCD_RS_PIN); // write the command
	}
	
	/* set all data pins @ low */
    LCD_DATA3_PORT &= ~_BV(LCD_DATA3_PIN);
    LCD_DATA2_PORT &= ~_BV(LCD_DATA2_PIN);
    LCD_DATA1_PORT &= ~_BV(LCD_DATA1_PIN);
    LCD_DATA0_PORT &= ~_BV(LCD_DATA0_PIN);	
	
	if (c & 0x80)
	{
		LCD_DATA3_PORT |= (1<<LCD_DATA3_PIN);
	}	
	if (c & 0x40)
	{
		LCD_DATA2_PORT |= (1<<LCD_DATA2_PIN);
	}
	if (c & 0x20)
	{
		LCD_DATA1_PORT |= (1<<LCD_DATA1_PIN);
	}		
	if (c & 0x10)
	{
		LCD_DATA0_PORT |= (1<<LCD_DATA0_PIN);
	}

	/* adoption high nibble */
	lcd_e_toggle();
	
	/* set all data pins @ low */
    LCD_DATA3_PORT &= ~_BV(LCD_DATA3_PIN);
    LCD_DATA2_PORT &= ~_BV(LCD_DATA2_PIN);
    LCD_DATA1_PORT &= ~_BV(LCD_DATA1_PIN);
    LCD_DATA0_PORT &= ~_BV(LCD_DATA0_PIN);	
	
	if (c & 0x01)
	{
		LCD_DATA0_PORT |= (1<<LCD_DATA0_PIN);
	}
	
	if (c & 0x02)
	{
		LCD_DATA1_PORT |= (1<<LCD_DATA1_PIN);
	}
	
	if (c & 0x04)
	{
		LCD_DATA2_PORT |= (1<<LCD_DATA2_PIN);
	}

	if (c & 0x08)
	{
		LCD_DATA3_PORT |= (1<<LCD_DATA3_PIN);
	}
	
	/* adoption low nibble */
	lcd_e_toggle();
	
	/* set all data pins @ high */
    LCD_DATA0_PORT |= _BV(LCD_DATA0_PIN);
    LCD_DATA1_PORT |= _BV(LCD_DATA1_PIN);
    LCD_DATA2_PORT |= _BV(LCD_DATA2_PIN);
    LCD_DATA3_PORT |= _BV(LCD_DATA3_PIN);
}

void lcd_init(void)
{
	
    DDR(LCD_RS_PORT)    |= (1<< LCD_RS_PIN);
    DDR(LCD_E_PORT)     |= (1<< LCD_E_PIN);
    DDR(LCD_DATA0_PORT) |= (1<< LCD_DATA0_PIN);
    DDR(LCD_DATA1_PORT) |= (1<< LCD_DATA1_PIN);
    DDR(LCD_DATA2_PORT) |= (1<< LCD_DATA2_PIN);
    DDR(LCD_DATA3_PORT) |= (1<< LCD_DATA3_PIN);
	
    /* initial write to lcd is 8bit */
    LCD_DATA1_PORT |= _BV(LCD_DATA1_PIN);
    LCD_DATA0_PORT |= _BV(LCD_DATA0_PIN);
    lcd_e_toggle();
    _delay_ms(5);
    
    lcd_e_toggle();
    _delay_us(70);
    
    lcd_e_toggle();
    _delay_us(70);

    /* now configure for 4bit mode */
    LCD_DATA0_PORT &= ~_BV(LCD_DATA0_PIN);
    lcd_e_toggle();
    _delay_us(70);
	

    lcd_command(0x28); // LCD_FUNCTION_4BIT_2LINES
    lcd_command(0x08); // LCD_DISP_OFF 
	lcd_command(0x00); // LCD_CLR
    lcd_command(0x0F); // LCD_DISP_ON_CURSOR_BLINK
    lcd_command(0x0E); // LCD_DISP_ON_CURSOR
	
	lcd_command(0x14); // CURSOR RIGHT ++
}


void lcd_e_toggle(void)
{
	LCD_E_PORT |= (1<<LCD_E_PIN);
	_delay_ms(5);
	LCD_E_PORT &= ~(1<<LCD_E_PIN);	
}