/*************************************************************
*|
*|	\@file  	timer.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Atmega Timer konfigurieren
*|
*|	\@date		22/10/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.211019
*|
*|**************************************************************/

#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>



/*!<-- defines -- >*/
/*****************************************************************/

/**< CPU Takt */
#ifndef F_CPU
    #define F_CPU   16000000 // Hz
#endif

/**< Waveform Generation Mode Bits */
#define _WGM00_bp           1<<6
#define _WGM01_bp           1<<3

#define _TMR0_MODE0         ( ! ( _WGM00_bp | _WGM01_bp ) ) // Normal
#define _TMR0_MODE1         ( _WGM00_bp ) // PWM, Phase Correct
#define _TMR0_MODE2         ( _WGM01_bp ) // CTC
#define _TMR0_MODE3         ( _WGM00_bp | _WGM01_bp ) // Fast PWM

/**< Prescaler Mode Group Position */
#define _PRESCALER_0       ( 0b001 << 0 )
#define _PRESCALER_8       ( 0b010 << 0 )
#define _PRESCALER_64      ( 0b011 << 0 )
#define _PRESCALER_256     ( 0b100 << 0 )
#define _PRESCALER_1024    ( 0b101 << 0 )

/**< TIMSK Interrupt Configuration */
#define _ENABLE_COMP_INT	TIMSK |= ( 1 << OCIE0 )
#define _ENABLE_OVF_INT		TIMSK |= ( 1 << TOIE0 )

typedef struct
{
    uint8_t uiOCR0;
    uint8_t uiCSxx;
    uint8_t uiWGMxx;
}sTimer0CompConfig_t;// Compare Match Interrupt Einstellungen

typedef struct
{
	uint8_t uiTCNT0;
	uint8_t uiCSxx;
	uint8_t uiWGMxx;
}sTimer0OvfConfig_t;// Overflow Interrupt Einstellungen

void (*Timer0CompCallback)(void) = NULL;
void (*Timer0OvfCallback)(void) = NULL;

/*****************************************************************/


/*!<-- global variables -- >*/
/*****************************************************************/

const sTimer0CompConfig_t sTimer0CompSettings[] =
{
		/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		+ Compare Match		 |	Divisor der F_CPU        | Waveform	Generating    +
		+ Vergleichswert     |  (Prescaler)              | Modus ( CTC! )		  +
		++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	#if (F_CPU == 1000000)
		{ .uiOCR0 = 9		 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE2 }, // 10µS  @1MHz
		{ .uiOCR0 = 99		 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE2 }, // 100µS @1MHz
		{ .uiOCR0 = 124		 , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE2 }, // 1ms   @1MHz
	#endif

	#if (F_CPU == 8000000)
		{ .uiOCR0 = 79		 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE2 }, // 10µS  @8MHz
		{ .uiOCR0 = 99		 , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE2 }, // 100µS @8MHz
		{ .uiOCR0 = 124		 , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR0_MODE2 }, // 1ms   @8MHz
	#endif

	#if (F_CPU == 16000000)
		{ .uiOCR0 = 159		 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE2 }, // 10µS  @16MHz
		{ .uiOCR0 = 199		 , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE2 }, // 100µS @16MHz
		{ .uiOCR0 = 249		 , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR0_MODE2 }, // 1ms   @16MHz
	#endif
};

const sTimer0OvfConfig_t sTimer0OvfSettings[] =
{
		/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		+ Overflow			 |	Divisor der F_CPU        | Waveform	Generating	      +
		+ Schwellenwert		 |  (Prescaler)              | Modus ( CTC! )		      +
		++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	#if (F_CPU == 1000000)
		{ .uiTCNT0 = 246	 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE0 }, // 10µS  @1MHz
		{ .uiTCNT0 = 156	 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE0 }, // 100µS @1MHz
		{ .uiTCNT0 = 131	 , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE0 }, // 1ms   @1MHz
	#endif

	#if (F_CPU == 8000000)
		{ .uiTCNT0 = 176	 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE0 }, // 10µS  @8MHz
		{ .uiTCNT0 = 156	 , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE0 }, // 100µS @8MHz
		{ .uiTCNT0 = 131	 , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR0_MODE0 }, // 1ms   @8MHz
	#endif

	#if (F_CPU == 16000000)
		{ .uiTCNT0 = 96		 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE0 }, // 10µS  @16MHz
		{ .uiTCNT0 = 56		 , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE0 }, // 100µS @16MHz
		{ .uiTCNT0 = 6		 , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR0_MODE0 }, // 1ms   @16MHz
	#endif
};

/*****************************************************************/


/*!<-- functions -- >*/
/*****************************************************************/

extern inline uint8_t Timer0CompInit( const sTimer0CompConfig_t *psTimer0CnfgTab , void (*pFnc)(void) )
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */

	TCCR0	= ( psTimer0CnfgTab->uiCSxx | psTimer0CnfgTab->uiWGMxx );
	OCR0	= psTimer0CnfgTab->uiOCR0;
	
	if ( pFnc == NULL )
	{
		return 1; // Falsche oder keine "Callback Adresse"
	}else
	{
		Timer0CompCallback = pFnc;
	}
	
	_ENABLE_COMP_INT;

	sei();
	
	return 0;
}

extern inline uint8_t Timer0OvfInit( const sTimer0OvfConfig_t *psTimer0CnfgTab , void (*pFnc)(void) )
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */

	TCCR0	= ( psTimer0CnfgTab->uiCSxx | psTimer0CnfgTab->uiWGMxx );
	TCNT0	= psTimer0CnfgTab->uiTCNT0;
	
	if ( pFnc == NULL )
	{
		return 1; // Falsche oder keine "Callback Adresse"
	}else
	{
		Timer0OvfCallback = pFnc;
	}
	
	_ENABLE_OVF_INT;

	sei();
	
	return 0;
}

ISR ( TIMER0_COMP_vect )
{
	Timer0CompCallback();
}

ISR ( TIMER0_OVF_vect )
{
	Timer0OvfCallback();
}

/*****************************************************************/


// end of file


#endif // __TIMER_H__
