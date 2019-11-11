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


typedef volatile uint8_t			*pRegister_t;

typedef struct
{
	pRegister_t pPort;
	uint8_t     New;
	uint8_t     Old;
	uint8_t     Info;
	uint8_t     Mask;
}Switch_t;


void SwitchInit( Switch_t *Switch );

uint8_t SwitchRead( Switch_t *Switch );

void SwitchClear( Switch_t *Switch );

#endif