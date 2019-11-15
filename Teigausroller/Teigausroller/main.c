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

int main(void)
{
    
	StepperInit( &sStepper );
   	//StepperSetMode( &sStepper , STEP_SIXTEENTH );
   	StepperEnable( &sStepper );
   	StepperActivate( &sStepper );
	
    while (1) 
    {
		StepperPulse();
    }
}

