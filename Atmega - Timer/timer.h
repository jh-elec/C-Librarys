/*************************************************************
*|
*|	\@file  	timer.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Atmega Timer Treiber
*|
*|	\@date		22/10/2019 - first implementation
*|
*|	\@todo 		Timer 1 : Es müssen noch die Overflow Interrupts behandelt werden!
*|              Timer 0 : Der Overflow Interrupt benötigt noch einen Reload Wert?
*|		
*|	\@test		Timer 0 : Es wurde erfolgreich der Compare Match Interrupt in betrieb genommen.
*|				Timer 1 : Es wurde erfolgreich der Compare Match A + B Interrupt in betrieb genommen. 
*|
*|
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
#define _TMR0_MODE0					( ! ( 1 << WGM00 | 1 << WGM01 ) ) // Normal
#define _TMR0_MODE2					( 1 << WGM01 ) // CTC

#define _TMR1_MODE0					( ! ( 1 << WGM13 | 1 << WGM12 | 1 << WGM11 | 1 << WGM10 ) ) // Normal
#define _TMR1_MODE4					( 1 << WGM12 ) // CTC

#define _TMR2_MODE0					( ! ( 1 << WGM21 | 1 << WGM20 ) ) // Normal
#define _TMR2_MODE2					( 1 << WGM21 ) // CTC

/**< Prescaler Mode Group Position */
#define _PRESCALER_0				( 0b001 << 0 )
#define _PRESCALER_8				( 0b010 << 0 )
#define _PRESCALER_64				( 0b011 << 0 )
#define _PRESCALER_256				( 0b100 << 0 )
#define _PRESCALER_1024				( 0b101 << 0 ) // Sind für alle Timer (0..2) gleich

/**< TIMSK Interrupt Configuration */
#define _ENABLE_TOIE0				TIMSK |= ( 1 << TOIE0  )
#define _ENABLE_OCIE0				TIMSK |= ( 1 << OCIE0  )
#define _ENABLE_TOIE1				TIMSK |= ( 1 << TOIE1  )
#define _ENABLE_OCIE1B				TIMSK |= ( 1 << OCIE1B )
#define _ENABLE_OCIE1A				TIMSK |= ( 1 << OCIE1A )
#define _ENABLE_TOIE2				TIMSK |= ( 1 << TOIE2  )
#define _ENABLE_OCIE2				TIMSK |= ( 1 << OCIE2  )

enum __attribute__( ( packed ) ) eTimer { TIMER0 , TIMER1 , TIMER2 };

typedef struct
{
    uint8_t uiCnt;
    uint8_t uiCSxx;
    uint8_t uiWGMxx;
}sTimer8Config_t;// 8 Bit Timer Einstellungen

typedef struct
{
	uint16_t uiCnt;
	uint8_t uiCSxx;
	uint8_t uiWGMxx;
}sTimer16Config_t;// 16 Bit Timer Einstellungen

typedef struct  
{
	uint8_t uiLoad8;
	uint16_t uiLoad16;
}sTimerReload_t;

sTimerReload_t sReload;

enum eTimerCallback
{
	CALLBACK_TIMER0_OVF,
	CALLBACK_TIMER0_COMP,
	CALLBACK_TIMER1_OVF,
	CALLBACK_TIMER1_COMPB,
	CALLBACK_TIMER1_COMPA,
	CALLBACK_TIMER2_OVF,
	CALLBACK_TIMER2_COMP,
	
	__CALLBACK_TIMER_MEMBERS__
};



void (*pvTimerCallback[__CALLBACK_TIMER_MEMBERS__])(void) =
{
		// Callback Adressen
	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	+ TIMER0_OVF | TIMER0_COMP | TIMER1_OVF | TIMER1_COMPB | TIMER1_COMPA | TIMER2_OVF | TIMER2_COMP  +
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
      NULL       , NULL       , NULL        , NULL         , NULL         , NULL       , NULL 
};

/*****************************************************************/


/*!<-- global variables -- >*/
/*****************************************************************/

const sTimer8Config_t  sTimer0OcieSettings[] =
{
		/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		+ Compare Match		 |	Divisor der F_CPU        | Waveform	Generating    +
		+ Vergleichswert     |  (Prescaler)              | Modus ( CTC! )		  +
		++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	#if (F_CPU == 1000000)
		{ .uiCnt = 9		 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE2 }, // 10µS  @1MHz
		{ .uiCnt = 99		 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE2 }, // 100µS @1MHz
		{ .uiCnt = 124		 , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE2 }, // 1ms   @1MHz
	
	#elif (F_CPU == 8000000)
		{ .uiCnt = 79		 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE2 }, // 10µS  @8MHz
		{ .uiCnt = 99		 , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE2 }, // 100µS @8MHz
		{ .uiCnt = 124		 , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR0_MODE2 }, // 1ms   @8MHz

	#elif (F_CPU == 16000000)
		{ .uiCnt = 159		 , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE2 }, // 10µS  @16MHz
		{ .uiCnt = 199		 , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE2 }, // 100µS @16MHz
		{ .uiCnt = 249		 , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR0_MODE2 }, // 1ms   @16MHz
	#else
		#warning F_CPU not declared!
	#endif
};


const sTimer8Config_t  sTimer0TovSettings[]  =
{
	#if (F_CPU == 1000000)
		{ .uiCnt = 246     , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE0 }, // 10µS  @1MHz
		{ .uiCnt = 156     , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE0 }, // 100µS @1MHz
		{ .uiCnt = 131     , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE0 }, // 1ms   @1MHz
	#endif

	#if (F_CPU == 8000000)
		{ .uiCnt = 176     , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE0 }, // 10µS  @8MHz
		{ .uiCnt = 156     , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE0 }, // 100µS @8MHz
		{ .uiCnt = 131     , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR0_MODE0 }, // 1ms   @8MHz
	#endif

	#if (F_CPU == 16000000)
		{ .uiCnt = 96      , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR0_MODE0 }, // 10µS  @16MHz
		{ .uiCnt = 56      , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR0_MODE0 }, // 100µS @16MHz
		{ .uiCnt = 6       , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR0_MODE0 }, // 1ms   @16MHz
	#endif
};


const sTimer16Config_t sTimer1OcieSettings[] = 
{
	#if (F_CPU == 1000000)
		{ .uiCnt = 9       , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE4 }, // 10µS  @1MHz
		{ .uiCnt = 99      , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE4 }, // 100µS @1MHz
		{ .uiCnt = 124     , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR1_MODE4 }, // 1ms   @1MHz
	#endif

	#if (F_CPU == 8000000)
		{ .uiCnt = 79	   , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE4 }, // 10µS  @8MHz
		{ .uiCnt = 99	   , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR1_MODE4 }, // 100µS @8MHz
		{ .uiCnt = 124     , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR1_MODE4 }, // 1ms   @8MHz
	#endif

	#if (F_CPU == 16000000)
		{ .uiCnt = 159      , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE4 }, // 10µS  @16MHz
		{ .uiCnt = 199      , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR1_MODE4 }, // 100µS @16MHz
		{ .uiCnt = 249      , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR1_MODE4 }, // 1ms   @16MHz
	#endif	
};


const sTimer16Config_t sTimer1TovSettings[] = 
{
	#if (F_CPU == 1000000)
		{ .uiCnt = 246     , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE0 }, // 10µS  @1MHz
		{ .uiCnt = 156     , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE0 }, // 100µS @1MHz
		{ .uiCnt = 131     , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR1_MODE0 }, // 1ms   @1MHz
	#endif

	#if (F_CPU == 8000000)
		{ .uiCnt = 176     , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE0 }, // 10µS  @8MHz
		{ .uiCnt = 156     , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR1_MODE0 }, // 100µS @8MHz
		{ .uiCnt = 131     , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR1_MODE0 }, // 1ms   @8MHz
	#endif

	#if (F_CPU == 16000000)
		{ .uiCnt = 96      , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE0 }, // 10µS  @16MHz
		{ .uiCnt = 56      , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR1_MODE0 }, // 100µS @16MHz
		{ .uiCnt = 6       , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR1_MODE0 }, // 1ms   @16MHz
	#endif	
};


/*****************************************************************/


/*!<-- functions -- >*/
/*****************************************************************/

extern inline uint8_t Timer0CompInit( const sTimer8Config_t *psTab , void (*pFnc)(void) )
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */

	TCCR0	= ( psTab->uiCSxx | psTab->uiWGMxx );
	OCR0	= psTab->uiCnt;
	
	if ( pFnc == NULL )
	{
		return 1; // keine gültige "Callback Adresse"
	}else
	{
		pvTimerCallback[CALLBACK_TIMER0_COMP] = pFnc;
	}
	
	_ENABLE_OCIE0;

	sei();
	
	return 0;
}

extern inline uint8_t Timer0OvfInit( const sTimer8Config_t *psTab , void (*pFnc)(void) )
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */

	TCCR0	= ( psTab->uiCSxx | psTab->uiWGMxx );
	TCNT0	= psTab->uiCnt;
	
	if ( pFnc == NULL )
	{
		return 1; // keine gültige "Callback Adresse"
	}else
	{
		pvTimerCallback[CALLBACK_TIMER0_OVF] = pFnc;
	}
	
	sReload.uiLoad8 = TCNT0;
	
	_ENABLE_TOIE0;

	sei();
	
	return 0;
}

extern inline uint8_t Timer1CompAInit( const sTimer16Config_t *psTab , void (*pFnc)(void) )
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */

	TCCR1A	= psTab->uiWGMxx;
	TCCR1B	= psTab->uiWGMxx;
	TCCR1B  |= psTab->uiCSxx;
	OCR1A	= psTab->uiCnt;
	
	if ( pFnc == NULL )
	{
		return 1; // keine gültige "Callback Adresse"
	}else
	{
		pvTimerCallback[CALLBACK_TIMER1_COMPA] = pFnc;
	}
	
	_ENABLE_OCIE1A;

	sei();
	
	return 0;
}

extern inline uint8_t Timer1CompBInit( const sTimer16Config_t *psTab , void (*pFnc)(void) )
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */

	TCCR1A	= psTab->uiWGMxx;
	TCCR1B	= psTab->uiWGMxx;
	TCCR1B  |= psTab->uiCSxx;
	OCR1B	= psTab->uiCnt;
	
	if ( pFnc == NULL )
	{
		return 1; // keine gültige "Callback Adresse"
	}else
	{
		pvTimerCallback[CALLBACK_TIMER1_COMPB] = pFnc;
	}
	
	_ENABLE_OCIE1B;

	sei();
	
	return 0;
}

ISR ( TIMER0_COMP_vect )	
{
	pvTimerCallback[CALLBACK_TIMER0_COMP]();
}

ISR ( TIMER0_OVF_vect )		
{
	pvTimerCallback[CALLBACK_TIMER0_OVF]();
	TCNT0 = sReload.uiLoad8;
}

ISR ( TIMER1_COMPA_vect )	
{
	pvTimerCallback[CALLBACK_TIMER1_COMPA]();
}

ISR ( TIMER1_COMPB_vect )	
{
	pvTimerCallback[CALLBACK_TIMER1_COMPB]();
}

/*****************************************************************/


// end of file


#endif // __TIMER_H__
