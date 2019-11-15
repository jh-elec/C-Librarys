/*
 * Teigausroller.c
 *
 * Created: 15.11.2019 06:05:47
 * Author : Hm
 */ 
#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>
#include "Includes/stepper.h"


int main(void)
{
	StepperReset( &sStepper );
	StepperInit( &sStepper );
   	StepperSetMode( &sStepper , STEP_FULL );
		
	StepperEnable( &sStepper );
	StepperActivate( &sStepper );
	
    while (1) 
    {
		StepperPulse();
    }
}

