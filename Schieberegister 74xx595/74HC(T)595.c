#include <avr/io.h>
#include <stdlib.h>
#include "com74hc595.h"

// initialize the array
unsigned char com74hc595[ com74hc595_SIZE ];

/*
	Author       : Frank Link
	Date         : 22.05.2008
	Description  : calculate the port number and set the bit into this port
*/
void com74hc595_setBit( unsigned char BitNumber)
{
	unsigned char Port = BitNumber >> 3;
	unsigned char Bit = BitNumber & 7;
	com74hc595[ Port ] |= (1 << ( Bit ) );
}

/*
	Author       : Frank Link
	Date         : 22.05.2008
	Description  : calculate the port number and unset the bit into this port
*/
void com74hc595_unsetBit( unsigned char BitNumber)
{
	unsigned char Port = BitNumber >> 3;
	unsigned char Bit = BitNumber & 7;
	com74hc595[ Port ] &= ~(1 << ( Bit ) );
}

/*
	Author       : Frank Link
	Date         : 22.05.2008
	Description  : set all bit to high = 1
*/
void com74hc595_setall()
{
	
	for ( int i = 0; i < com74hc595_SIZE; i++ )
	{
		com74hc595_setPort( i, 0xFF );
	}
}

/*
	Author       : Frank Link
	Date         : 22.05.2008
	Description  : set all bit to low = 0
*/
void com74hc595_unsetall()
{
	for ( int i = 0; i < com74hc595_SIZE; i++ )
	{
		com74hc595_setPort( i, 0x00 );
	}
}

/*
	Author       : Frank Link
	Date         : 22.05.2008
	Description  : set all bit of the port to the defined value
*/
void com74hc595_setPort( unsigned char Port, unsigned char Bits )
{
	com74hc595[ Port ] = Bits;
}

/*
	Author       : Frank Link
	Date         : 22.05.2008
	Description  : set all bit of the port to low = 0
*/
void com74hc595_unsetPort( unsigned char Port )
{
	com74hc595[ Port ] = 0x00;
}


/*
	Author       : Frank Link
	Date         : 22.05.2008
	Description  : Initializing
*/
void com74hc595_init()
{
    /* set Ports to OUT */
	DDR_com74hc595 |= ( 1 << DDR_SER);
	DDR_com74hc595 |= ( 1 << DDR_SCK);
	DDR_com74hc595 |= ( 1 << DDR_RCK);

#ifdef WITH_OE
	DDR_com74hc595 |= ( 1 << DDR_OE);
#endif

    /* set SER to LOW */
	PORT_com74hc595 &= ~(1<<PORT_SER);

    /* set SCR und RCK to HIGH */
	PORT_com74hc595 |= (1<<PORT_SCK);
	PORT_com74hc595 |= (1<<PORT_RCK);
#ifdef WITH_OE
	PORT_com74hc595 |= (1<<PORT_OE);
#endif

	com74hc595_unsetall();
	com74hc595_out();     

#ifdef WITH_OE
	PORT_com74hc595 &= ~(1<<PORT_OE);
#endif
}

/*
	Author       : Frank Link
	Date         : 22.05.2008
	Description  : output to 74HC595
*/
void com74hc595_out()
{
    unsigned char anz = com74hc595_SIZE;
    unsigned char* serp = com74hc595 + com74hc595_SIZE;

    do
    {
        unsigned char bits;
        unsigned char data = *--serp;

        /* 8 Bits per Byte to 74HC595 */
        for (bits = 8; bits > 0; bits--)
        {
			PORT_com74hc595 &= ~(1<<PORT_SER);
            if (data & 0x80)
            {
				PORT_com74hc595 |= (1<<PORT_SER);
            };

            data <<= 1;
            /* Strobe on SCK to shift the bit into 74HC595  */
			PORT_com74hc595 &= ~(1<<PORT_SCK);
			PORT_com74hc595 |= (1<<PORT_SCK);
        }
    }
    while (--anz > 0);

    /* Strobe on RCK for out on 74HC595 */
	PORT_com74hc595 &= ~(1<<PORT_RCK);
	PORT_com74hc595 |= (1<<PORT_RCK);
}
