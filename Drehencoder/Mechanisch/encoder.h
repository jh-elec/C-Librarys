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


#ifndef __ENCODER_H__
#define __ENCODER_H__



#define PORT_GET_DDR( _PORT ) 		( * (_PORT - 1 ) )
#define PORT_GET_PIN( _PORT )		( * (_PORT - 2 ) )

typedef volatile uint8_t* pPort_t;

enum
{
	PHASE_A,
	PHASE_B,
	
	__PHASE_MAX_ENTRYS__
}ePhase;

typedef enum
{
	ENC_ROTATION_LEFT = 1,
	ENC_ROTATION_RIGHT,
}eEncRotation_t;

void EncoderInit( pPort_t  pPort , uint8_t uiPhaseABp , uint8_t uiPhaseBBp , uint8_t uiPullUpMask );

int8_t EncoderGet( void );

eEncRotation_t EncoderGetRotation( void );

void EncoderRead( void );



#endif // __ENCODER_H__