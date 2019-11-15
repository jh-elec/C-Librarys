/*************************************************************
*|
*|	\@file  	stepper.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Schrittmotor Treiber - A4988 / DRV8825
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


/*++++++++++++++++++++++++++++++++++++++
+	Treiber | ATmega328
+
+	DIR     = A7 (PC.7)
+	STP     = A6 (PC.6)
+	SLP     = A5 (PC.5)
+	RST     = A4 (PC.4)
+	M2      = A3 (PC.3)
+	M1      = A2 (PC.2)
+	M0      = A1 (PC.1)
+	EN      = A0 (PC.0)
+++++++++++++++++++++++++++++++++++++++*/

/*!<-- defines -- >*/
/*****************************************************************/
#define STEPPER_STEP_PORT		PORTC
#define STEPPER_STEP_BP			6

#define STEPPER_RESET_PORT		PORTC
#define STEPPER_RESET_BP		4

#define STEPPER_DIR_PORT		PORTC
#define STEPPER_DIR_BP			7

#define STEPPER_MS_PORT			PORTC
#define STEPPER_MS1_BP			1
#define STEPPER_MS2_BP			2
#define STEPPER_MS3_BP			3

#define STEPPER_EN_PORT			PORTC
#define STEPPER_EN_BP			0

#define STEPPER_SLP_PORT		PORTC
#define STEPPER_SLP_BP			5

enum __attribute__((packed)) eMSx        { MS1 , MS2 , MS3 , __MSx_MAX_ENTRYS__ };

enum __attribute__((packed)) eStep
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
		uint8_t uiMsxGp;		
	}sMsx;
	
	struct
	{
		pPort_t pPort;
		uint8_t uiBp;
	}sStep;
	
	struct  
	{
		pPort_t pPort;
		uint8_t uiBp;
	}sEn;

	struct
	{
		pPort_t pPort;
		uint8_t uiBp;
	}sSlp;

	uint8_t uiMode[__STEP_MAX_ENTRYS__];

}sStepper_t;

sStepper_t sStepper =	
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
	},
	
	.sEn =
	{
		&STEPPER_EN_PORT , STEPPER_EN_BP
	},
	
	.sSlp =
	{
		&STEPPER_SLP_PORT , STEPPER_SLP_BP
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

static inline void StepperInit( sStepper_t *sObj )
{
	/*!<-- Daten Richtungs Register konfiguieren <--*/
	*( sObj->sMsx.pPort - 1  ) |= ( sObj->sMsx.uiMsxGp    );
	*( sObj->sDir.pPort - 1  ) |= ( 1 << sObj->sDir.uiBp  );
	*( sObj->sRst.pPort - 1  ) |= ( 1 << sObj->sRst.uiBp  );
	*( sObj->sStep.pPort - 1 ) |= ( 1 << sObj->sStep.uiBp );
	*( sObj->sEn.pPort - 1   ) |= ( 1 << sObj->sEn.uiBp   );
	*( sObj->sSlp.pPort - 1  ) |= ( 1 << sObj->sSlp.uiBp  );
	
	/*!<-- Default Zustände setzen <--*/
	*( sObj->sDir.pPort  ) |=  ( 1 << sObj->sDir.uiBp  );
	*( sObj->sRst.pPort  ) |=  ( 1 << sObj->sRst.uiBp  );
	*( sObj->sSlp.pPort  ) |=  ( 1 << sObj->sSlp.uiBp  );
	*( sObj->sStep.pPort ) &= ~( 1 << sObj->sStep.uiBp );
	*( sObj->sEn.pPort   ) &= ~( 1 << sObj->sEn.uiBp   );

	/*!<-- Schrittmodis aus den angeschlossenen MSx Bits zusammen bauen <--*/
	sObj->uiMode[STEP_FULL]			= ( 0x00 );
	sObj->uiMode[STEP_HALF]			= ( 1 << STEPPER_MS1_BP );
	sObj->uiMode[STEP_QUARTER]		= ( 1 << STEPPER_MS2_BP );
	sObj->uiMode[STEP_EIGHTH]		= ( (1 << STEPPER_MS1_BP) | (1 << STEPPER_MS2_BP) );
	sObj->uiMode[STEP_SIXTEENTH]	= ( (1 << STEPPER_MS1_BP) | (1 << STEPPER_MS2_BP) | (1 << STEPPER_MS3_BP) );
	
	/*!<-- Nur für den internen gebrauch gedacht! <--*/
 	sStepperDrive.pPort = sObj->sStep.pPort;
 	sStepperDrive.uiBp = sObj->sStep.uiBp;
}

static inline void StepperReset( sStepper_t *sObj )
{
	*( sObj->sRst.pPort ) &= ~( 1 << sObj->sRst.uiBp );
	for ( uint8_t x = 0 ; x < 10 ; x++){};
	*( sObj->sRst.pPort ) |= ( 1 << sObj->sRst.uiBp );	
}

static inline uint8_t StepperSetMode( sStepper_t *sObj , enum eStep Step )
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

static inline void StepperChangeRotation( sStepper_t *sObj )
{
	*( sObj->sDir.pPort ) ^= 1 << sObj->sDir.uiBp;	
};

static inline void StepperEnable( sStepper_t *sObj )
{
	*( sObj->sEn.pPort ) &= ~( 1 << sObj->sEn.uiBp );
}

static inline void StepperDisable( sStepper_t *sObj )
{
	*( sObj->sEn.pPort ) |= ( 1 << sObj->sEn.uiBp );
}

static inline void StepperSleep( sStepper_t *sObj )
{
	*( sObj->sSlp.pPort ) &= ~( 1 << sObj->sSlp.uiBp );
}

static inline void StepperActivate( sStepper_t *sObj )
{
	*( sObj->sSlp.pPort ) |= ( 1 << sObj->sSlp.uiBp );
}

static inline void StepperPulse( void )
{
 	*( sStepperDrive.pPort ) ^= ( 1 << sStepperDrive.uiBp );
}

/*****************************************************************/



// end of file



#endif // __STEPPER_H__
