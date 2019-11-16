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

typedef volatile uint8_t			*pRegister_t;

typedef struct
{
	pRegister_t pPort;
	uint8_t     New;
	uint8_t     Old;
	uint8_t     Info;
	uint8_t     Mask;
}sSwitch_t;


enum eSwitchError SwitchInit( sSwitch_t *Switch , pRegister_t pPort , uint8_t uiSwitchMask , uint8_t uiPullUpMask );

uint8_t SwitchGet( sSwitch_t *psSwitch , uint8_t uiMask );

void SwitchClear( sSwitch_t *Switch );

#endif