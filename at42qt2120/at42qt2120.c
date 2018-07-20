/* Generated by CodeDescriptor 1.0.0.0 */
/*
* Project Name      -> AT42QT2120
* Version           -> 1.0
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 16.05.2017 08:39:28
* Description       -> Library / Handling Routines
*
*
*
*/

#include <avr/io.h>
#include "at42qt2120.h"
#include "xmega_twi.h"



void qt2120_get_info(TWI_t *twi, at42qt_t *at42qtxx){
	
	uint8_t tmp[2]={0,0};
	
	twi_reads(twi,CHIP_ID,2,tmp);
	
	at42qtxx->ID = tmp[0];
	at42qtxx->FirmwareVersion = tmp[1];
}

void qt2120_get_rks(TWI_t *twi, at42qt_t *at42qtxx, enum keys_for_signal keyx){
	
	uint8_t KeyArray = 0;
	uint8_t Byte[2] = {0,0};
			
	twi_reads(twi,keyx,2,Byte);
	
	switch(keyx)
	{
		case KEY_SIGNAL_0_BYTE_1:		{KeyArray = 0; }break;
		case KEY_SIGNAL_1_BYTE_1:		{KeyArray = 1; }break;
		case KEY_SIGNAL_2_BYTE_1:		{KeyArray = 2; }break;
		case KEY_SIGNAL_3_BYTE_1:		{KeyArray = 3; }break;
		case KEY_SIGNAL_4_BYTE_1:		{KeyArray = 4; }break;
		case KEY_SIGNAL_5_BYTE_1:		{KeyArray = 5; }break;
		case KEY_SIGNAL_6_BYTE_1:		{KeyArray = 6; }break;
		case KEY_SIGNAL_7_BYTE_1:		{KeyArray = 7; }break;
		case KEY_SIGNAL_8_BYTE_1:		{KeyArray = 8; }break;
		case KEY_SIGNAL_9_BYTE_1:		{KeyArray = 9; }break;
		case KEY_SIGNAL_10_BYTE_1:		{KeyArray = 10;}break;
		case KEY_SIGNAL_11_BYTE_1:		{KeyArray = 11;}break;
	}
	at42qtxx->KeySignal[KeyArray] = ((uint16_t)((uint8_t)(Byte[0])<<8UL) | (uint8_t)Byte[1]);
}

void qt2120_get_rfd(TWI_t *twi, at42qt_t *at42qtxx){
	
	uint8_t ReadedBytes = 0;
	uint8_t Byte[2]	= {0,0};
	
	for (uint8_t KeySignalCnt = REFERENCE_DATA_0_BYTE_1 ; KeySignalCnt < REFERENCE_DATA_11_BYTE_2 ; KeySignalCnt++)
	{
		Byte[0] = twi_read(twi,KeySignalCnt);
		Byte[1] = twi_read(twi,KeySignalCnt+1);
		at42qtxx->ReferenceData[KeySignalCnt] = ((uint16_t)((uint8_t)(Byte[0])<<8UL) | (uint8_t)Byte[1]);
		ReadedBytes++;
	}
}

void qt2120_get_state(TWI_t *twi, at42qt_t *at42qtxx){
	
	at42qtxx->DetectionStatus = twi_read(twi,DETECTION_STATUS);
}

void qt2120_get_ks(TWI_t *twi, at42qt_t *at42qtxx){
	
	uint8_t Byte[2]={0,0};
	twi_reads(twi,KEY_STATUS_BYTE_1,2,Byte);
	at42qtxx->KeyStatus=((uint16_t)((uint8_t)(Byte[1])<<8UL) | (uint8_t)Byte[0]);
}


void qt2120_set_gpio(TWI_t *twi, at42qt_t *at42qtxx, uint8_t key, enum gpio_state state){
		
	twi_write(twi, (28+key), (state<<1) | 0x01);
}

void qt2120_cmd_cal(TWI_t *twi, at42qt_t *at42qtxx){
	
	twi_write(twi,CALIBRATE,0xFF); // Calibrate
	while(at42qtxx->DetectionStatus & 1<<7){
		qt2120_get_state(twi,at42qtxx);
	}
}

void qt2120_set_dthr(TWI_t *twi, enum keys_for_struct keyx, uint8_t dthr){
	
	twi_write(twi,keyx+KEY_0_DETECT_THRESHOLD,dthr); // keyx + Offset = Register Address..
}