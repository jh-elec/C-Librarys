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
#if defined( __AVR_ATmega328P__ )
  
	 /*!<-- ATmega328P <--*/

	  /*!<-- TCCR0A – Timer/Counter0 Control Register A [7..0] <--*/
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + COM0A1 | COM0A0 | COM0B1 | COM0B0 | – | – | WGM01 | WGM00        +
	  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
		
		#define TCCR0A_WGM( _WGMxx )					( TCCR0A |= ( _WGMxx & ( ( 1 << WGM01 ) | ( 1 << WGM00 ) ) )  ) 

	  /*!<-- TCCR0B – Timer/Counter0 Control Register A [7..0] <--*/
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + FOC0A | FOC0B | – | – | WGM02 | CS02 | CS01 | CS00               +
	  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	
		#define TCCR0B_WGM_CS( _WGMxx , _CSxx )			( TCCR0B |= ( _WGMxx & ( 1 << WGM02 ) ) |  ( _CSxx & (( 1 << CS02 ) | ( 1 << CS01 ) | ( 1 << CS00 ) ) ) )

	  /*!<-- TCCR1A – Timer/Counter1 Control Register A [7..0] <--*/
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + COM1A1 | COM1A0 | COM1B1 | COM1B0 | – | – | WGM11 | WGM10        +
	  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
 
		#define TCCR1A_WGM( _WGMxx )					( TCCR1A |= ( _WGMxx & ( ( 1 << WGM11 ) | ( 1 << WGM10 ) ) ) )
 
	  /*!<-- TCCR1B – Timer/Counter1 Control Register B [7..0] <--*/
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + ICNC1 | ICES1 | – | WGM13 | WGM12 | CS12 | CS11 | CS10           +
	  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/ 

		#define TCCR1B_WGM_CS( _WGMxx , _CSxx )				TCCR1B |= ( _WGMxx & ( 1 << WGM13 | 1 << WGM12 ) ) | ( _CSxx & ( ( 1 << CS12 ) | ( 1 << CS11 ) | ( 1 << CS10 ) ) )
 
	  /*!<-- TIMSK1 – Timer/Counter Interrupt Mask Register [7..0] <--*/
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + – | – | ICIE1 | – | – | OCIE1B | OCIE1A | TOIE1                  +
	  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/   

		#define TIMSK1_OCIE1A_EN						TIMSK1 |= ( 1 << OCIE1A ) 
		#define TIMSK1_OCIE1B_EN						TIMSK1 |= ( 1 << OCIE1B )
		#define TIMSK1_TOIE1_EN							TIMSK1 |= ( 1 << TOIE1  )


	  /*!<-- TIMSK0 – Timer/Counter Interrupt Mask Register [7..0] <--*/
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + - | - | - | - | - | OCIE0B | OCIE0A | TOIE0                      +
	  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/   

		#define TIMSK0_TOIE0_EN							TIMSK0 |= ( 1 << TOIE0  ) 
		#define TIMSK0_OCIE0A_EN						TIMSK0 |= ( 1 << OCIE0A )
		#define TIMSK0_OCIE0B_EN						TIMSK0 |= ( 1 << OCIE0B )
		

	  /*!<-- TCCR2A – Timer/Counter Control Register A [7..0] <--*/
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + COM2A1 | COM2A0 | COM2B1 | COM2B0 | – | – | WGM21 | WGM20        +
	  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/  
		
		#define TCCR2A_WGM( _WGMxx )					( TCCR2A |= ( _WGMxx & (( 1 << WGM21 ) | ( 1 << WGM20 )) ) )

	  /*!<-- TCCR2B – Timer/Counter Control Register B [7..0] <--*/
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + FOC2A | FOC2B | – | – | WGM22 | CS22 | CS21 | CS20               +
	  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/  

		#define TCCR2B_WGM_CS( _WGMxx , _CSxx )			( TCCR2B |= ( _WGMxx & 1 << WGM22 ) | ( _CSxx & ( ( 1 << CS22 ) | ( 1 << CS21 ) | ( 1 << CS20 ) ) ) )			
		

	  /*!<-- TIMSK2 – Timer/Counter2 Interrupt Mask Register [7..0] <--*/
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + – | – | – | – | – | OCIE2B | OCIE2A | TOIE2                      +
	  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/ 
  
		#define TIMSK2_TOIE2_EN							TIMSK2 |= ( 1 << TOIE2  )
		#define TIMSK2_OCIE2A_EN						TIMSK2 |= ( 1 << OCIE2A )
		#define TIMSK2_OCIE2B_EN						TIMSK2 |= ( 1 << OCIE2B )
  
	 /*!<-- ATmega328P // End <--*/

	
#elif defined  ( __AVR_ATmega32__ )	

	/*!<-- ATmega32 <--*/

	  /*!<-- TCCR0 – Timer/Counter Control Register [7..0] <--*/
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + FOC0 | WGM00 | COM01 | COM00 | WGM01 | CS02 | CS01 |  CS00       +
	  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    
		#define TCCR0_WGM_CS( _WGMxx , _CSxx )			( TCCR0 |= ( _WGMxx  & (( 1 << WGM00 ) | ( 1 << WGM01 ) ) | ( _CSxx & ( ( 1 << CS02 ) | ( 1 << CS01 ) | ( 1 << CS00 ) ) ) ) )	
	
	  /*!<-- TIMSK – Timer/Counter Interrupt Mask Register [7..0] <--*/
	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + OCIE2 | TOIE2 | TICIE1 | OCIE1A | OCIE1B | TOIE1 | OCIE0 | TOIE0 +
	  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/   
  
		#define TIMSK_OCIE2_EN				TIMSK |= ( 1 << OCIE2  )
		#define TIMSK_TOIE2_EN				TIMSK |= ( 1 << TOIE2  )
		#define TIMSK_OCIE1A_EN				TIMSK |= ( 1 << OCIE1A )
		#define TIMSK_OCIE1B_EN				TIMSK |= ( 1 << OCIE1B )
		#define TIMSK_TOIE1_EN				TIMSK |= ( 1 << TOIE1  )
		#define TIMSK_OCIE0_EN				TIMSK |= ( 1 << OCIE0  )
		#define TIMSK_TOIE0_EN				TIMSK |= ( 1 << TOIE0  )
		
	  /*!<-- TCCR1A – Timer/Counter1 Control Register A [7..0] <--*/
	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + COM1A1 | COM1A0 | COM1B1 | COM1B0 | FOC1A | FOC1B | WGM11 | WGM10 +
	  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/  
  
		#define TCCR1A_WGM( _WGMxx )		( TCCR1A |= ( _WGMxx & ( ( 1 << WGM11 ) | ( 1 << WGM10 ) ) ) )
  
	  /*!<-- TCCR1B – Timer/Counter1 Control Register B [7..0] <--*/
	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + ICNC1 | ICES1 | – | WGM13 | WGM12 | CS12 | CS11 | CS10            +
	  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/  
  
		#define TCCR1B_WGM_CS( _WGMxx , _CSxx )		( TCCR1B |= ( _WGMxx & (( 1 << WGM13) | ( 1 << WGM12 ) ) ) | ( _CSxx & ( ( 1 << CS12 ) | ( 1 << CS11 ) | ( 1 << CS10 ) ) ) )
	
  
	  /*!<-- TCCR2 – Timer/Counter Control Register [7..0] <--*/
	/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	  + FOC2 | WGM20 | COM21 | COM20 | WGM21 | CS22 | CS21 | CS20         +
	  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/  
  
		#define TCCR2_WGM_CS( _WGMxx , _CSxx )			( TCCR2 |= ( _WGMxx & ( ( 1 << WGM20 ) | ( 1 << WGM21 ) ) ) | ( _CSxx & ( ( 1 << CS22 ) | ( 1 << CS21 ) | ( 1 << CS20 ) ) ) )		  
  
	 /*!<-- ATmega32 // End <--*/

#else
	#error Konfiguration nicht vorhande!
#endif


enum __attribute__( ( packed ) ) eTimer { TIMER0 , TIMER1 , TIMER2 , __MAX_TIMER_ENTRYS__ };

enum __attribute__( ( packed ) ) eTimerCallback
{
	CALLBACK_TIMER0_OVF,
	CALLBACK_TIMER0_COMP,
	CALLBACK_TIMER0_COMPA,
	CALLBACK_TIMER0_COMPB,
	CALLBACK_TIMER1_OVF,
	CALLBACK_TIMER1_COMPB,
	CALLBACK_TIMER1_COMPA,
	CALLBACK_TIMER2_OVF,
	CALLBACK_TIMER2_COMP,
	CALLBACK_TIMER2_COMPA,
	CALLBACK_TIMER2_COMPB,
	
	__CALLBACK_TIMER_MEMBERS__
};

enum __attribute__( ( packed ) ) eTimerError
{
	ERROR_TIMER_OK,
	ERROR_TIMER_NO_ADDRESS,
};


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



sTimerReload_t sReload[__MAX_TIMER_ENTRYS__];


// pv = PointerVector
void (*pvTimerCallback[__CALLBACK_TIMER_MEMBERS__])(void) =
{
		// Callback Adressen! Werden in den entsprechenden Initalisierungen zugewiesen.
	[CALLBACK_TIMER0_OVF   ] = NULL,
	[CALLBACK_TIMER0_COMP  ] = NULL,
	[CALLBACK_TIMER0_COMPA ] = NULL,
	[CALLBACK_TIMER0_COMPB ] = NULL,
	[CALLBACK_TIMER1_OVF   ] = NULL,
	[CALLBACK_TIMER1_COMPB ] = NULL,
	[CALLBACK_TIMER1_COMPA ] = NULL,
	[CALLBACK_TIMER2_OVF   ] = NULL,
	[CALLBACK_TIMER2_COMP  ] = NULL,
    [CALLBACK_TIMER2_COMPA ] = NULL,
	[CALLBACK_TIMER2_COMPB ] = NULL,
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
		{ .uiCnt = 7	   , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE4 }, // 1µS   @8MHz
		{ .uiCnt = 79	   , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE4 }, // 10µS  @8MHz
		{ .uiCnt = 99	   , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR1_MODE4 }, // 100µS @8MHz
		{ .uiCnt = 124     , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR1_MODE4 }, // 1ms   @8MHz
	#endif

	#if (F_CPU == 16000000)
		{ .uiCnt = 15       , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE4 }, // 1µS   @16MHz	
		{ .uiCnt = 159      , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE4 }, // 10µS  @16MHz
		{ .uiCnt = 199      , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR1_MODE4 }, // 100µS @16MHz
		{ .uiCnt = 249      , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR1_MODE4 }, // 1ms   @16MHz
	#endif	
};


const sTimer16Config_t sTimer1TovSettings[] = 
{
	#if (F_CPU == 1000000)
		{ .uiCnt = 65535   , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE0 }, // 1µS   @1MHz	
		{ .uiCnt = 65526   , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE0 }, // 10µS  @1MHz
		{ .uiCnt = 65436   , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE0 }, // 100µS @1MHz
		{ .uiCnt = 65411   , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR1_MODE0 }, // 1ms   @1MHz
	#endif

	#if (F_CPU == 8000000)
		{ .uiCnt = 65528   , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE0 }, // 1µS   @8MHz
		{ .uiCnt = 65456   , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE0 }, // 10µS  @8MHz
		{ .uiCnt = 65436   , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR1_MODE0 }, // 100µS @8MHz
		{ .uiCnt = 65411   , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR1_MODE0 }, // 1ms   @8MHz
	#endif

	#if (F_CPU == 16000000)
		{ .uiCnt = 65520  , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE0 }, // 1µS   @16MHz
		{ .uiCnt = 65376  , .uiCSxx = _PRESCALER_0    , .uiWGMxx = _TMR1_MODE0 }, // 10µS  @16MHz
		{ .uiCnt = 65336  , .uiCSxx = _PRESCALER_8    , .uiWGMxx = _TMR1_MODE0 }, // 100µS @16MHz
		{ .uiCnt = 65286  , .uiCSxx = _PRESCALER_64   , .uiWGMxx = _TMR1_MODE0 }, // 1ms   @16MHz
	#endif	
};


/*****************************************************************/


/*!<-- functions -- >*/
/*****************************************************************/

extern inline enum eTimerError Timer0CompInit( const sTimer8Config_t *psTab , void (*pFncCallback)(void) )	
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */
	
	if ( pFncCallback == NULL )
	{
		sei();
		return ERROR_TIMER_NO_ADDRESS;
	}else
	{
		pvTimerCallback[CALLBACK_TIMER0_COMP] = pFncCallback;
	}
	
	#if defined  ( __AVR_ATmega32__ )	
		
		TCCR0_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		OCR0 = psTab->uiCnt;
		TIMSK_OCIE0_EN;	
	#else
		#warning Konfiguration fuer aktuellen Controller fehlt!
	#endif
	
	sei();
	
	return ERROR_TIMER_OK;
}

extern inline enum eTimerError Timer0CompAInit( const sTimer8Config_t *psTab , void (*pFncCallback)(void) )	
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */
	
	if ( pFncCallback == NULL )
	{
		sei();
		return ERROR_TIMER_NO_ADDRESS;
	}else
	{
		pvTimerCallback[CALLBACK_TIMER0_COMPA] = pFncCallback;
	}
	
	#if defined  ( __AVR_ATmega328P__ )
		
		TCCR1A_WGM( psTab->uiWGMxx );
		TCCR1B_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		OCR0A = psTab->uiCnt;

		TIMSK0_OCIE0A_EN;
	
	#else
		#warning Konfiguration fuer aktuellen Controller fehlt!
	#endif
	
	sei();
	
	return ERROR_TIMER_OK;
}

extern inline enum eTimerError Timer0CompBInit( const sTimer8Config_t *psTab , void (*pFncCallback)(void) )	
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */
	
	if ( pFncCallback == NULL )
	{
		sei();
		return ERROR_TIMER_NO_ADDRESS;
	}else
	{
		pvTimerCallback[CALLBACK_TIMER0_COMPB] = pFncCallback;
	}
	
	#if defined  ( __AVR_ATmega328P__ )
	
		TCCR1A_WGM( psTab->uiWGMxx );
		TCCR1B_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		OCR0B = psTab->uiCnt;

		TIMSK0_OCIE0B_EN;
	
	#else
		#warning Konfiguration fuer aktuellen Controller fehlt!
	#endif
	
	sei();
	
	return ERROR_TIMER_OK;
}

extern inline enum eTimerError Timer0OvfInit( const sTimer8Config_t *psTab , void (*pFncCallback)(void) )	
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */
	
	if ( pFncCallback == NULL )
	{
		sei();
		return ERROR_TIMER_NO_ADDRESS;
	}else
	{
		pvTimerCallback[CALLBACK_TIMER0_OVF] = pFncCallback;
	}
	
	sReload[TIMER0].uiLoad8 = TCNT0;
	
	#if defined  ( __AVR_ATmega32__ )
	
		TCCR0_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		TCNT0 = psTab->uiCnt;
					
		TIMSK_TOIE0_EN;
	
	#elif defined ( __AVR_ATmega328P__ )
	
		TCCR0B_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		TCCR0A_WGM( psTab->uiWGMxx );
		TCNT0 = psTab->uiCnt;
		
		TIMSK0_TOIE0_EN;

	#else
		#warning Konfiguration fuer aktuellen Controller fehlt!
	#endif
	
	sei();
	
	return ERROR_TIMER_OK;
}


extern inline enum eTimerError Timer1CompAInit( const sTimer16Config_t *psTab , void (*pFncCallback)(void) )
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */
	
	if ( pFncCallback == NULL )
	{
		sei();
		return ERROR_TIMER_NO_ADDRESS;
	}else
	{
		pvTimerCallback[CALLBACK_TIMER1_COMPA] = pFncCallback;
	}
	
	#if defined  ( __AVR_ATmega32__ )

		TCCR1A_WGM( psTab->uiWGMxx );
		TCCR1B_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		OCR1A = psTab->uiCnt;
		
		TIMSK_OCIE1A_EN;

	#elif defined ( __AVR_ATmega328P__ )
	
		TCCR1A_WGM( psTab->uiWGMxx );
		TCCR1B_WGM_CS( psTab->uiWGMxx  , psTab->uiCSxx );
		OCR1A = psTab->uiCnt;
		
		TIMSK1_OCIE1A_EN;
		
	#else
		#warning Konfiguration fuer aktuellen Controller fehlt!
	#endif

	sei();
	
	return ERROR_TIMER_OK;
}

extern inline enum eTimerError Timer1CompBInit( const sTimer16Config_t *psTab , void (*pFncCallback)(void) )
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */
	
	if ( pFncCallback == NULL )
	{
		sei();
		return ERROR_TIMER_NO_ADDRESS;
	}else
	{
		pvTimerCallback[CALLBACK_TIMER1_COMPB] = pFncCallback;
	}
	
	#if defined  ( __AVR_ATmega32__ )
		
		TCCR1A_WGM( psTab->uiWGMxx );
		TCCR1B_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		OCR1B = psTab->uiCnt;
		
		TIMSK_OCIE1B_EN;
	
	#elif defined ( __AVR_ATmega328P__ )
		
		TCCR1A_WGM( psTab->uiWGMxx );
		TCCR1B_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		OCR1B = psTab->uiCnt;
		
		TIMSK1_OCIE1B_EN;
		
	#else
		#warning Konfiguration fuer aktuellen Controller fehlt!
	#endif

	sei();
	
	return ERROR_TIMER_OK;
}

extern inline enum eTimerError Timer1OvfInit( const sTimer16Config_t *psTab , void (*pFncCallback)(void) )	
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */

	if ( pFncCallback == NULL )
	{
		sei();
		return ERROR_TIMER_NO_ADDRESS;
	}else
	{
		pvTimerCallback[CALLBACK_TIMER1_OVF] = pFncCallback;
	}
	
	
	sReload[TIMER1].uiLoad16 = TCNT1;
	
	
	#if defined  ( __AVR_ATmega32__ )

		TCCR1A_WGM( psTab->uiWGMxx );
		TCCR1B_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );

		TIMSK_TOIE1_EN;

	#elif defined ( __AVR_ATmega328P__ )
		
		TCCR1A_WGM( psTab->uiWGMxx );
		TCCR1B_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		
		TIMSK1_TOIE1_EN;
		
	#else
		#warning Konfiguration fuer aktuellen Controller fehlt!
	#endif

	sei();
	
	return ERROR_TIMER_OK;	
}


extern inline enum eTimerError Timer2CompInit( const sTimer8Config_t *psTab  , void (*pFncCallback)(void) )	
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */

	if ( pFncCallback == NULL )
	{
		sei();
		return ERROR_TIMER_NO_ADDRESS;
	}else
	{
		pvTimerCallback[CALLBACK_TIMER2_COMP] = pFncCallback;
	}
	
	#if defined  ( __AVR_ATmega32__ )

		TCCR2_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		OCR2 = psTab->uiCnt;
		TIMSK_OCIE2_EN;
		
	#else
		#warning Konfiguration fuer aktuellen Controller fehlt!
	#endif

	sei();
	
	return ERROR_TIMER_OK;	
};

extern inline enum eTimerError Timer2CompAInit( const sTimer8Config_t *psTab , void (*pFncCallback)(void) )	
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */

	if ( pFncCallback == NULL )
	{
		sei();
		return ERROR_TIMER_NO_ADDRESS;
	}else
	{
		pvTimerCallback[CALLBACK_TIMER2_COMPA] = pFncCallback;
	}
	
	#if defined  ( __AVR_ATmega328P__ )

	TCCR2A_WGM( psTab->uiWGMxx );
	TCCR2B_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
	
	TCCR2A = psTab->uiCnt;
	
	TIMSK2_OCIE2A_EN;
	
	#else
		#warning Konfiguration fuer aktuellen Controller fehlt!
	#endif

	sei();
	
	return ERROR_TIMER_OK;	
}

extern inline enum eTimerError Timer2CompBInit( const sTimer8Config_t *psTab , void (*pFncCallback)(void) )	
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */

	if ( pFncCallback == NULL )
	{
		sei();
		return ERROR_TIMER_NO_ADDRESS;
	}else
	{
		pvTimerCallback[CALLBACK_TIMER2_COMPB] = pFncCallback;
	}
	
	#if defined  ( __AVR_ATmega328P__ )

	TCCR2A_WGM( psTab->uiWGMxx );
	TCCR2B_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
	OCR2B = psTab->uiCnt;
	
	TIMSK2_OCIE2B_EN;
	
	#else
		#warning Konfiguration fuer aktuellen Controller fehlt!
	#endif

	sei();
	
	return ERROR_TIMER_OK;
}

extern inline enum eTimerError Timer2OvfInit( const sTimer8Config_t *psTab  , void (*pFncCallback)(void) )	
{
	cli(); /**< Vorsichtshalber Interrupts global sperren */

	if ( pFncCallback == NULL )
	{
		sei();
		return ERROR_TIMER_NO_ADDRESS;
	}else
	{
		pvTimerCallback[CALLBACK_TIMER2_OVF] = pFncCallback;
	}
	
	sReload[TIMER2].uiLoad8 = psTab->uiCnt;
	
	#if defined  ( __AVR_ATmega32__ )

		TCCR2_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		TCNT2 = psTab->uiCnt;
		TIMSK_TOIE2_EN;

	#elif defined ( __AVR_ATmega328P__ )

		TCCR2A_WGM( psTab->uiWGMxx );
		TCCR2B_WGM_CS( psTab->uiWGMxx , psTab->uiCSxx );
		TCNT2 = psTab->uiCnt;
		TIMSK2_TOIE2_EN;

	#else
		#warning Konfiguration fuer aktuellen Controller fehlt!
	#endif

	sei();
	
	return ERROR_TIMER_OK;
};




#ifdef TIMER0_COMPA_vect	
ISR ( TIMER0_COMPA_vect )	
{
	pvTimerCallback[CALLBACK_TIMER0_COMPA]();
}
#endif

#ifdef TIMER0_COMPB_vect	
ISR ( TIMER0_COMPB_vect )
{
	pvTimerCallback[CALLBACK_TIMER0_COMPB]();
}
#endif

#ifdef TIMER0_OVF_vect		
ISR ( TIMER0_OVF_vect )		
{
	pvTimerCallback[CALLBACK_TIMER0_OVF]();
	TCNT0 = sReload[TIMER0].uiLoad8;
}
#endif 

#ifdef TIMER1_COMPA_vect	
ISR ( TIMER1_COMPA_vect )	
{
	pvTimerCallback[CALLBACK_TIMER1_COMPA]();
}
#endif 

#ifdef TIMER1_COMPB_vect	
ISR ( TIMER1_COMPB_vect )	
{
	pvTimerCallback[CALLBACK_TIMER1_COMPB]();
}
#endif 

#ifdef TIMER1_OVF_vect		
ISR ( TIMER1_OVF_vect )
{
	pvTimerCallback[CALLBACK_TIMER1_OVF]();
	TCNT1 = sReload[TIMER1].uiLoad16;	
};
#endif

#ifdef TIMER2_COMP_vect		
ISR ( TIMER2_COMP_vect )
{
	pvTimerCallback[CALLBACK_TIMER2_COMP]();
}
#endif

#ifdef TIMER2_OVF_vect		
ISR ( TIMER2_OVF_vect )
{
	pvTimerCallback[CALLBACK_TIMER2_OVF]();
	TCNT2 = sReload[TIMER2].uiLoad16;
}
#endif

/*****************************************************************/


// end of file


#endif // __TIMER_H__
