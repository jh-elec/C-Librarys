/*************************************************************
*|
*|	\@file  	a4988.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Schrittmotor Treiber - A4988
*|
*|	\@date		14/11/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.141119
*|
*|**************************************************************/

#ifndef __A4988_H__
#define __A4988_H__

#include <stdio.h>
#include <avr/io.h>


/*!<-- defines -- >*/
/*****************************************************************/
#define STEPPER_STEP_PORT		PORTD
#define STEPPER_STEP_BP			PD7

#define STEPPER_RESET_PORT		PORTD
#define STEPPER_RESET_BP		PD6

#define STEPPER_DIR_PORT		PORTD
#define STEPPER_DIR_BP			PD5

#define STEPPER_MS_PORT			PORTD
#define STEPPER_MS1_BP			PD2
#define STEPPER_MS2_BP			PD3
#define STEPPER_MS3_BP			PD4

enum eMSx        { MS1 , MS2 , MS3 , __MSx_MAX_ENTRYS__ };

enum eStep
{
	STEP_FULL,
	STEP_HALF,		
	STEP_QUARTER,		
	STEP_EIGHTH,	
	STEP_SIXTEENTH,
	
	__STEP_MAX_ENTRYS__
};

/*****************************************************************/


/*!<-- global variables -- >*/
/*****************************************************************/
typedef volatile uint8_t *pPort_t;

typedef struct  
{
	struct
	{
		pPort_t pPort;
		uint8_t uiBp;	
	}sDir;
	
	struct
	{
		pPort_t pPort;
		uint8_t uiBp;
	}sRst;
	
	struct  
	{
		pPort_t pPort;
		uint8_t uiMsxBp;		
	}sMsx;
	
	struct
	{
		pPort_t pPort;
		uint8_t uiBp;
	}sStep;

	uint8_t uiMode[__STEP_MAX_ENTRYS__];

}sA4988_t;

sA4988_t sStepper =
{
	.sMsx =
	{
		&STEPPER_MS_PORT  , ( (1 << STEPPER_MS1_BP) | (1 << STEPPER_MS2_BP) | (1 << STEPPER_MS3_BP) )
	},
	
	.sDir =
	{
		&STEPPER_DIR_PORT  , STEPPER_DIR_BP
	},
	
	.sRst =
	{
		&STEPPER_RESET_PORT  , STEPPER_RESET_BP
	},
	
	.sStep =
	{
		&STEPPER_STEP_PORT , STEPPER_STEP_BP
	}

};

typedef struct
{
	pPort_t pPort;
	uint8_t uiBp;
}sStepperDrive_t;

static volatile sStepperDrive_t sStepperDrive;

/*****************************************************************/


/*!<-- functions -- >*/
/*****************************************************************/

static inline void StepperInit( sA4988_t *sObj )
{
	/*!<-- Daten Richtungs Register konfiguieren <--*/
	*( sObj->sMsx.pPort - 1  )  = sObj->sMsx.uiMsxBp; // Step Bits
	*( sObj->sDir.pPort - 1  ) |= ( 1 << sObj->sDir.uiBp ); // Richtungs Ausgang
	*( sObj->sRst.pPort - 1  ) |= ( 1 << sObj->sRst.uiBp ); // Reset Ausgang
	*( sObj->sStep.pPort - 1 ) |= ( 1 << sObj->sStep.uiBp ); // Pulse Ausgang
	
	/*!<-- Ausgänge auf default "low" setzen <--*/
	*( sObj->sMsx.pPort ) &= ~( sObj->sMsx.uiMsxBp );

	sObj->uiMode[STEP_FULL]			= 0x00;
	sObj->uiMode[STEP_HALF]			= 1 << STEPPER_MS1_BP;
	sObj->uiMode[STEP_QUARTER]		= 1 << STEPPER_MS2_BP;
	sObj->uiMode[STEP_EIGHTH]		= ( (1 << STEPPER_MS1_BP) | (1 << STEPPER_MS2_BP) );
	sObj->uiMode[STEP_SIXTEENTH]	= ( (1 << STEPPER_MS1_BP) | (1 << STEPPER_MS2_BP) | (1 << STEPPER_MS3_BP) );
	
	/*!<-- Nur für den internen gebrauch gedacht! <--*/
 	sStepperDrive.pPort = sObj->sStep.pPort;
 	sStepperDrive.uiBp = sObj->sStep.uiBp;
}

static inline void StepperReset( sA4988_t *sObj )
{
	*( sObj->sRst.pPort ) &= ~( 1 << sObj->sRst.uiBp );
	for ( uint8_t x = 0 ; x < 10 ; x++){};
	*( sObj->sRst.pPort ) |= ( 1 << sObj->sRst.uiBp );	
}

static inline uint8_t StepperSetMode( sA4988_t *sObj , enum eStep Step )
{
	if ( Step > STEP_SIXTEENTH )
	{
		return 1; // Modus nicht vorhanden
	}
	
	if ( !(sObj->sMsx.pPort) || !(sObj->sDir.pPort) || !(sObj->sRst.pPort) )
	{
		return 2; // kein gültiger Port
	}
	
	*sObj->sMsx.pPort = ( *sObj->sMsx.pPort & 0x00 ) ^ sObj->uiMode[Step];
	
	return 0;
}

static inline void StepperPulse( void )
{
 	*( sStepperDrive.pPort ) &= ~( 1 << sStepperDrive.uiBp );
 	*( sStepperDrive.pPort ) |= ( 1 << sStepperDrive.uiBp );
}

/*****************************************************************/



// end of file



#endif // __A4988_H__
