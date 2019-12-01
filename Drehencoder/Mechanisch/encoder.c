/*************************************************************
*|
*|	\@file  	encoder.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Routinen zum auslesen eines Mechanischen Encoders
*|
*|	\@date		23/11/2019 - first implementation
*|
*|	\@todo
*|
*|
*|	\@test
*|
*|
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.231119
*|
*|**************************************************************/


#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "../Includes/encoder.h"




typedef struct
{
	pPort_t pPin;
	uint8_t uiPhase[__PHASE_MAX_ENTRYS__];
	int8_t uiDeltaCnt;
	uint8_t uiRotation;
}sEnc_t;

volatile sEnc_t sEnc;

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



void EncoderInit( pPort_t  pPort , uint8_t uiPhaseABp , uint8_t uiPhaseBBp , uint8_t uiPullUpMask )
{
	if ( pPort == NULL )
	{
		return;
	}
	
	sEnc.pPin = ( pPort - 2 );
	sEnc.uiPhase[PHASE_B] = 1 << uiPhaseABp;
	sEnc.uiPhase[PHASE_A] = 1 << uiPhaseBBp;
	sEnc.uiDeltaCnt = 0;
	
	PORT_GET_DDR( pPort ) &= ~( sEnc.uiPhase[PHASE_A] | sEnc.uiPhase[PHASE_B] ); // Eingänge konfigurieren
	*pPort |= ( uiPullUpMask ); // ggf. PullUps setzen

}

int8_t EncoderGet( void )        
{
	int8_t val;
	// atomarer Variablenzugriff
	cli();
	val = sEnc.uiDeltaCnt;
	sEnc.uiDeltaCnt = 0;
	sei();
	return val;
}

eEncRotation_t EncoderGetRotation( void )
{
	eEncRotation_t eEnc;
	eEnc = sEnc.uiRotation;
	sEnc.uiRotation = 0;
	
	return eEnc;
}

void EncoderRead( void )
{
	static int8_t Last = 0;           // alten Wert speichern
	
	Last = (Last << 2)  & 0x0F;
	if (*(sEnc.pPin) & sEnc.uiPhase[PHASE_A]) Last |=2;
	if (*(sEnc.pPin) & sEnc.uiPhase[PHASE_B]) Last |=1;
	
	int8_t iTableRead = pgm_read_byte( &table[Last] );
	sEnc.uiDeltaCnt += iTableRead;
	
	switch ( iTableRead )
	{
		case 1	:{ sEnc.uiRotation = ENC_ROTATION_LEFT;  }break;
		case -1 :{ sEnc.uiRotation = ENC_ROTATION_RIGHT; }break;
	}
}



