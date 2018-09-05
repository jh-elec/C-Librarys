/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> WSQ 3000
* Version           -> 1.0.0.0813
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 13.08.2018 10:10:01
* Description       -> Timer
*
*
*
*	------------------
*	|Port	|	Timer|
*	------------------
*	PORTC		TCCxC
*	PORTD		TCCxD
*	PORTE		TCCxE
*
*/


#ifndef __XMEGA_TIMER_H__
#define __XMEGA_TIMER_H__

#define F_CPU					2e6

#define TIMER_HZ				10000
#define TIMER_PRESCALER			1

#if		TIMER_PRESCALER == 1
#undef TIMER_PRESCALER_gc
#define TIMER_PRESCALER_gc	TC_CLKSEL_DIV1_gc
#elif	TIMER_PRESCALER == 2
#undef TIMER_PRESCALER_gc
#define TIMER_PRESCALER_gc	TC_CLKSEL_DIV2_gc
#elif	TIMER_PRESCALER == 4
#undef TIMER_PRESCALER_gc
#define TIMER_PRESCALER_gc	TC_CLKSEL_DIV4_gc
#elif	TIMER_PRESCALER == 8
#undef TIMER_PRESCALER_gc
#define TIMER_PRESCALER_gc	TC_CLKSEL_DIV8_gc
#elif	TIMER_PRESCALER == 64
#undef TIMER_PRESCALER_gc
#define TIMER_PRESCALER_gc	TC_CLKSEL_DIV64_gc
#elif	TIMER_PRESCALER == 256
#undef TIMER_PRESCALER_gc
#define TIMER_PRESCALER_gc	TC_CLKSEL_DIV256_gc
#elif	TIMER_PRESCALER == 1024
#undef TIMER_PRESCALER_gc
#define TIMER_PRESCALER_gc	TC_CLKSEL_DIV1024_gc
#endif


/*
*	Einige XMEGA�s haben mehrere Timer Compare Matches
*/
#define TIMER_CALC					(uint16_t)( ( F_CPU / ( 2 * TIMER_PRESCALER ) * ( TIMER_HZ + 1 ) ) )

#define TIMER_CCx( _timer , _ccx)	( ( & ( _timer->CCA ) )[_ccx] )

enum tc0_ccx_enum
{
	TC0_CCA,
	TC0_CCB,
	TC0_CCC,
	TC0_CCD,
};

enum tc1_ccx_enum
{
	TC1_CCA,
	TC1_CCB,
};


void		timerOVFInit	( TC0_t *tim0 , TC1_t *tim1 );

void		timerCMPInit	( TC0_t *tim0 , enum tc0_ccx_enum channel0 , TC1_t *tim1 , enum tc1_ccx_enum channel1 );

void		timerPWMInit	( TC0_t *tim0 , enum tc0_ccx_enum channel0 , TC1_t *tim1 , enum tc1_ccx_enum channel1 , uint16_t period , uint16_t compare )	;

void		timerStart		( TC0_t *tim0 , TC1_t *tim1 );

void		timerStop		( TC0_t *tim0 , TC1_t *tim1 );



#endif  // __XMEGA_TIMER_H__
