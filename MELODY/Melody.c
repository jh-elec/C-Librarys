/*
 * Copyright:   Jan Homann
 * Author:      Jan Homann
 * Version:     1.0
 * Date:		14.02.2016
 */

#ifndef F_CPU
#endif	F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include "Melody.h"

// Set overall tempo
long tempo = 20000;
// Set length of pause between notes
int pause = 1000;
// Loop variable to increase Rest length
int rest_count = 100; //<-BLETCHEROUS HACK; See NOTES

// Initialize core variables
int tone_ = 0;
int beat = 0;
long duration  = 0;

int MAX_COUNT = sizeof(tetris_melody) / 2; // Melody length, for looping.


void playTone(void)
{
	long elapsed_time = 0;
	if (tone_ > 0)
	{ // if this isn't a Rest beat, while the tone has
		//  played less long than 'duration', pulse speaker HIGH and LOW
		while (elapsed_time < duration)
		{

			PORTB |= (1<<PB6);
			delay_microsecounds(tone_ / 2);

			// DOWN
			PORTB &= ~(1<<PB6);
			delay_microsecounds(tone_ / 2);

			// Keep track of how long we pulsed
			elapsed_time += (tone_);
		}
	}
	else
	{ // Rest beat; loop times delay
		for (int j = 0; j < rest_count; j++)
		{ // See NOTE on rest_count
			delay_microsecounds(duration);
		}
	}
}

void loop(void)
{
	// Set up a counter to pull from melody[] and beats[]
	for (int i=0; i<MAX_COUNT; i++)
	{
		tone_ = pgm_read_dword(&tetris_melody [i]);
		beat  = pgm_read_dword(&tetris_beats [i]);

		//tone_ = test[i][0];
		//beat  = test[i][1];

		duration = beat * tempo; // Set up timing

		playTone();
		
		// A pause between notes...
		delay_microsecounds(pause);
	}
}

void delay_microsecounds(uint16_t secounds)
{
	for (uint16_t x = 0 ; x < secounds ; x++)
	{
		_delay_us(1);
	}
}