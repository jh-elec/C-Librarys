/*
 * crc16.c
 *
 * Created: 08.07.2015 12:57:05
 *  Author: Hm
 */ 


#include <avr/io.h>
#include "CRC.h"

int main(void)
{
	int *intptr = 0x00;
	int a = 0xBBBB;
	intptr = &a;
	
	
    while(1)
    {
		calculate_crc16(intptr,2);
    }
}