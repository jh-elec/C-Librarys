/*
 * Copyright:   Jan Homann
 * Author:      Jan Homann
 * Version:     1.0
 * Date:		14.02.2016
 */

#ifndef __MELODY_H__
#define __MELODY_H__

#include <avr/pgmspace.h>

/**********************************************************
*
* TONES
* Start by defining the relationship between
* note, period, &  frequency.
*
**********************************************************/

#define		a_4     2272    // 440 Hz
#define		b_4     2028    // 493 Hz
#define		c_4     3830    // 261 Hz
#define		d_4     3400    // 294 Hz
#define		e_4     3038    // 329 Hz
#define		f_4     2864    // 349 Hz
#define		g_4     2550    // 392 Hz

#define		a_3		4545
#define		b_3		4050
#define		c_3		7645
#define		d_3		6811
#define		e_3		6067
#define		f_3		5727
#define		g_3		5104
#define		gis_3	4816

#define		e_2		12135


#define		gis_4	2407



#define		c_5     1912    // 523 Hz



const uint16_t tetris_melody[]	 PROGMEM = {  e_4,b_3,c_4,d_4,c_4,b_3,a_3,a_3,c_4,e_4,d_4,c_4,b_3,c_4,d_4,e_4,c_4,a_3,a_3,/*R*/d_4,f_4,a_4,g_4,f_4,e_4,c_4,e_4,d_4,c_4,b_3,b_3,c_4,d_4,e_4,c_4,a_3,a_3,e_4,a_3,e_4,a_3,c_4,a_3,c_4,a_3,d_4,b_3,d_4,b_3,b_3,gis_3,b_3,gis_3,c_4,e_3,c_4,e_3,a_3,e_3,a_3,e_3,gis_3,e_3,gis_3,e_3,b_3,e_4,a_3,e_4,a_3,c_4,a_3,c_4,a_3,d_4,b_3,d_4,b_3,b_3,gis_3,b_3,gis_3,c_4,e_4,a_4,gis_4};
const uint16_t tetris_beats[]    PROGMEM = {  16,8,8,16,8,8,16,8,8,16,8,8,24,8,16,16,16,16,32,/*8*/16,8,16,8,8,24,8,16,8,8,16,8,8,16,16,16,16,32,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,32,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,16,16,32,64};



#endif 