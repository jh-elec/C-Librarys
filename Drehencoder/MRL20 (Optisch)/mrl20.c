
/*
 * Copyright:      Jan Homann
 * Author:         Jan Homann
 * Version:        1.0
 * Description:    Funktion zum auslesen eines "Optischen Drehencoder"
 */


/*
	Rechtsdrehung

	00 -> 01 -> 11 -> 10 -> 00

	Linksdrehung

	00 -> 10 -> 11 -> 01 -> 00
*/

#include <inttypes.h>
#include <avr/io.h>
#include "mrl20.h"




void init_Encoder(void)
{
	Encoder_DDR	 &= ~((1<<Encoder_PHASE_A) | (1<<Encoder_PHASE_B));
	Encoder_PORT |=  ((1<<Encoder_PHASE_A) | (1<<Encoder_PHASE_B));
}


volatile uint8_t read_Encoder(uint8_t PinPORT, uint8_t BinaryNumber)
{

	volatile static uint8_t Enc_State_new = 0, Enc_State_old = 0, Enc_Counter = 0;
	

	Enc_State_new = (PinPORT & BinaryNumber);

	if ((Enc_State_new != Enc_State_old))

	{

		if (Enc_State_new == 0x01)
		{

			if (Enc_State_old == 0x00)
			{
				Enc_Counter++;
			}
			else
			{
				
				if ((Enc_Counter & 0xFE))
				{			
					Enc_Counter--;
				}
				

			}
		}

		if (Enc_State_new == 0x03)
		{

			if (Enc_State_old == 0x01)
			{
				Enc_Counter++;
			}
			else
			{
				
				if ((Enc_Counter & 0xFE))
				{
					Enc_Counter--;
				}
			}

		}

		if (Enc_State_new == 0x02)
		{
			if (Enc_State_old == 0x03)
			{
				Enc_Counter++;
			}
			else
			{
				
				if ((Enc_Counter & 0xFE))
				{
					Enc_Counter--;
				}
			}

		}

		if (Enc_State_new == 0x00)
		{
			if (Enc_State_old == 0x02)
			{
				Enc_Counter++;
			}
			else
			{
				
				if ((Enc_Counter & 0xFE))
				{
					Enc_Counter--;
				}
			}

		}

	}

	Enc_State_old = Enc_State_new;
	
	return(Enc_Counter);

}