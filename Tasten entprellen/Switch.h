/*************************************************************
*|
*|	\@file  	Switch.h
*|	\@brief
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Taster einlesen und verarbeiten ( entprellen )
*|
*|	\@date		28/03/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.280319
*|
*|**************************************************************/

#ifndef __SWITCH_H__
#define __SWITCH_H__

#include <avr/interrupt.h>

#ifndef NULL
#define NULL	(void*)0
#endif

/*	Macro 
*	- Calculate Address of Data Direction Register
*	from
*/
#define PORT_DDR_ADDR( PORT )		(*( (PORT-1) ))

/*	Macro 
*	- Calculate Address of PIN Register
*	from
*/
#define PORT_PIN_ADDR( PORT )		(*( (PORT-2) ))

enum __attribute__((packed)) eSwitchError
{
	ERROR_SWITCH_OK,
	ERROR_SWITCH_NO_PORT,	
};

#define _SWITCH_REPEAT_START		100
#define _SWITCH_REPEAT_NEXT			5

typedef volatile uint8_t			*pRegister_t;

typedef struct
{
	pRegister_t pPort;
	uint8_t     uiNew;
	uint8_t     uiOld;
	uint8_t     uiInfo;
	uint8_t     uiMask;
	
	uint8_t		uiRepeat;
	uint8_t		uiRepeatKeyMask;
}sSwitch_t;


enum eSwitchError SwitchInit( sSwitch_t *Switch , pRegister_t pPort , uint8_t uiKeyMask , uint8_t uiPullUpMask , uint8_t uiRepeatKeyMask );

uint8_t SwitchRead( sSwitch_t *psSwitch );

uint8_t SwitchGet( sSwitch_t *psSwitch , uint8_t uiKeyMask );

void SwitchClear( sSwitch_t *Switch );


/*!<-- BETA <--*/
uint8_t SwitchGetRpt( sSwitch_t *psSwitch , uint8_t uiKeyMask );
/*!<-- BETA ENDE <--*/

#endif