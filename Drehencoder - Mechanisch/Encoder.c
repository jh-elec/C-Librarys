/*
	Routine for read Mechanical Encoder
*/

/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#define PHASE_A     (PINB & 1<<PB2)     // an Pinbelegung anpassen
#define PHASE_B     (PINB & 1<<PB1)     // an Pinbelegung anpassen

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Encoder.h"


volatile int8_t enc_delta;              // Drehgeberbewegung zwischen

// zwei Auslesungen im Hauptprogramm

// Dekodertabelle für wackeligen Rastpunkt  ( es wird +2 gezählt )
// halbe Auflösung
//const int8_t table[16] PROGMEM = {0,0,-1,0,0,0,0,1,1,0,0,0,0,-1,0,0};

// Dekodertabelle für normale Drehgeber
// volle Auflösung
//const int8_t table[16] PROGMEM = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};

// Dekodertabelle für wackeligen Rastpunkt ( es wird nur +1 gezählt )
// volle Auflösung
  const int8_t table[16] PROGMEM = {0,0,-1,0,0,0,0,1,0,0,0,0,0,0,0,0};


int8_t encode_read( void )         // Encoder auslesen
{
	int8_t val;
	// atomarer Variablenzugriff
	cli();
	val = enc_delta;
	enc_delta = 0;
	sei();
	return val;
}

ISR(TIMER0_COMPA_vect)             // 1ms fuer manuelle Eingabe
{
	static int8_t last=0;           // alten Wert speichern
	
	last = (last << 2)  & 0x0F;
	if (PHASE_A) last |=2;
	if (PHASE_B) last |=1;
	enc_delta += pgm_read_byte(&table[last]);
}

#if defined(__AVR_AT328P__)
	void encode_init( void )            // nur Timer 0 initialisieren
	{
		TCCR0A = (1<<WGM01);// CTC
		TCCR0B = ((1<<CS01) | (1<<CS00));// F_CPU/64
		OCR0A = (uint8_t)(F_CPU / 64.0 * 1e-3 - 0.5);       // 1ms
		TIMSK0 |= 1<<OCIE1A;
	}
#endif


