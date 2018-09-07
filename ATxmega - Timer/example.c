/*
 * GccApplication1.c
 *
 * Created: 05.09.2018 08:19:03
 * Author : Hm
 */ 

#define F_CPU	2e6

#include <avr/io.h>
#include <util/delay.h>
#include "C:\Users\Hm\Documents\Atmel Studio\7.0\C-Librarys\trunk\ATxmega - A3BU Board\xmega_a3bu_board.h"

#include "C:\Users\Hm\Documents\Atmel Studio\7.0\C-Librarys\trunk\ATxmega - Timer\xmega_timer.h"
#include "C:\Users\Hm\Documents\Atmel Studio\7.0\C-Librarys\trunk\ATxmega - Timer\xmega_timer.c"

int main(void)
{
    PORTE.DIRSET	= 1<<4; 
	PORTD.DIRSET	= 1<<0;
	PORTD.DIRSET	= 1<<1;
	PORTD.DIRSET	= 1<<2;
	PORTD.DIRSET	= 1<<3;
	
	tcxInit_t pwmOc1a;
	pwmOc1a.tim1 = &TCE1;
	pwmOc1a.tim1Cnfg.compareMatchChannel = TC1_CCA;
	pwmOc1a.tim1Cnfg.preVal = TIMER_PRESCALER_256;
	pwmOc1a.tim1Cnfg.perVal = 100;
	pwmOc1a.tim1Cnfg.cmpVal = 10;
	timerPWMInit( &pwmOc1a );

	tcxInit_t cmpMI;
	
	cmpMI.tim0							= &TCD0; 	
 	cmpMI.tim0Cnfg.preVal				= TIMER_PRESCALER_1024;
	cmpMI.tim0Cnfg.perVal				= 400;
 	
	cmpMI.tim0Cnfg.compareMatchChannel	= TC0_CCA;
	cmpMI.tim0Cnfg.cmpVal				= 100;
	timerCMPInit( &cmpMI );

	cmpMI.tim0Cnfg.compareMatchChannel	= TC0_CCB;
	cmpMI.tim0Cnfg.cmpVal				= 200;
	timerCMPInit( &cmpMI );
	
	cmpMI.tim0Cnfg.compareMatchChannel	= TC0_CCC;
	cmpMI.tim0Cnfg.cmpVal				= 300;
	timerCMPInit( &cmpMI );

	cmpMI.tim0Cnfg.compareMatchChannel	= TC0_CCD;
	cmpMI.tim0Cnfg.cmpVal				= 400;
	timerCMPInit( &cmpMI );
		
    while (1) 
    {

    }
}

ISR( TCD0_CCA_vect )
{
	PORTD.OUTTGL = 1<<3;
}

ISR( TCD0_CCB_vect )
{
	PORTD.OUTTGL = 1<<2;
}

ISR( TCD0_CCC_vect )
{
	PORTD.OUTTGL = 1<<1;
}

ISR( TCD0_CCD_vect )
{
	PORTD.OUTTGL = 1<<0;
}