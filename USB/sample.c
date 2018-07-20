/*
 * FSD_Schrittmotormodul.c
 *
 * Created: 21.03.2016 12:18:21
 * Author : Hm
 */ 

#define F_CPU	16000000

#define STATE_LED_ON		PORTD |= (1<<PD5)
#define STATE_LED_OFF		PORTD &= ~(1<<PD5)
#define STATE_LED_TOGGLE	PORTD ^= (1<<PD5)

#define LED_LEFT_TOGGLE		PORTB ^= (1<<PB6)
#define LED_RIGHT_TOOGLE	PORTB ^= (1<<PB5)

#define LED_LEFT_ON			PORTB |= (1<<PB6)
#define LED_LEFT_OFF		PORTB &= ~(1<<PB6)

#define LED_RIGHT_ON		PORTB |= (1<<PB5)
#define LED_RIGHT_OFF		PORTB &= ~(1<<PB5)

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))


#define LEFT		(1<<6)
#define RIGHT		(1<<7)
#define MEASURE		(1<<0)



#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>


#include "Encoder.h"
#include "uart.h"
#include "lcd.h"
#include "usb_serial.h"
#include "pwm.h"

struct {
			 int8_t  step;
			 int8_t  tmp;
			 int8_t  old;
			 int8_t  new;
			 uint8_t state;
			 uint8_t rotation;
			 float time;
	}Encoder;

void read_enc(void);
void send_str(const char *s);
void hardware_init(void);

/* contains character */
char Buffer[10];

int main(void)
{
	/* configure µC */
	hardware_init();
	
	/* configure usb unit */
	CPU_PRESCALE(0);
	
	usb_init();	
	
	init_pwm(0x00,OCIE1A, 21);
	
	init_enc();	
	
	
    while (1) 
    {

		read_enc();
		
		if (Encoder.state & RIGHT) // right
		{
			Encoder.state &= ~RIGHT; // right delete
			
			Encoder.step++; // for time measure
			
			if (Encoder.step >= 2) // new time available (ms)
			{
				Encoder.step = 0;
				Encoder.time = Time.ms;
				Time.ms = 0;
				
				itoa(Encoder.time,Buffer,10);
				send_str(Buffer);
				usb_serial_putchar('r'); // rotate right
				usb_serial_putchar('\n');
				usb_serial_putchar('\r');
				
				Time.ms = 0;
				Encoder.time = 0;					
			}
			
			LED_RIGHT_ON; // ->
			LED_LEFT_OFF; // show way
		}
		
		if (Encoder.state & LEFT) // right
		{
			Encoder.state &= ~LEFT; // right delete
			
			Encoder.step++; // for time measure
			
			if (Encoder.step >= 2) // new time available (ms)
			{
				Encoder.step = 0;
				Encoder.time = Time.ms;
				Time.ms = 0;
			
				itoa(Encoder.time,Buffer,10);
				send_str(Buffer);
				usb_serial_putchar('l'); // rotate left
				usb_serial_putchar('\n');
				usb_serial_putchar('\r');	
				
				Time.ms = 0;
				Encoder.time = 0;				
			}
			
			LED_LEFT_ON; // ->
			LED_RIGHT_OFF; // show way
		}
			
    }// end while
}// end main

void send_str(const char *s)
{
	char c;
	while (1) {
		c = (*s++);
		if (!c) break;
		usb_serial_putchar(c);
	}
}

void read_enc(void)
{		
	Encoder.new += encode_read();
	
	if (Encoder.new != Encoder.old)
	{
		Encoder.tmp = Encoder.new;
		
		if (Encoder.old > Encoder.tmp)// right
		{
			Encoder.state |=  RIGHT; // right detect
			Encoder.state &= ~LEFT; // left delete
		}
		if (Encoder.old < Encoder.tmp)// left
		{
			Encoder.state |=  LEFT; // left detect
			Encoder.state &= ~RIGHT; // right delete
		}
		Encoder.old = Encoder.new;
			
		Encoder.state ^= 1<<0; // start measure
	}// end if	
}

void hardware_init(void)
{
	DDRD |= (1<<PD5); // state led on board
	DDRB |= ((1<<PB5) | (1<<PB6)); // bi color led
	
	PORTC |= (1<<PC6);
	PORTD |= (1<<PD0);
	
	for (uint8_t x = 0 ; x < 10 ; x++)
	{
		LED_RIGHT_ON;
		_delay_ms(40);
		LED_RIGHT_OFF;
		_delay_ms(40);
		LED_LEFT_ON;
		_delay_ms(40);
		LED_LEFT_OFF;
	}
	LED_LEFT_OFF;
	LED_RIGHT_OFF;	
}