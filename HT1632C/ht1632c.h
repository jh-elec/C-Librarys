// ht1632c
// custom HT1632C driver library for
// JY-MCU 3208 Display
// Rick Shear

#ifndef _HT1632C_H
#define _HT1632C_H

#ifndef ROWS
  #define ROWS 8
#endif

#ifndef COLS
  #define COLS 32
#endif


#define HT1632_CS_DDR  DDRA
#define HT1632_WR_DDR  DDRA
#define HT1632_DT_DDR  DDRA

#define HT1632_CS_PORT PORTA
#define HT1632_WR_PORT PORTA
#define HT1632_DT_PORT PORTA

#define HT1632_CS_BP  PA5
#define HT1632_WR_BP  PA3
#define HT1632_DT_BP  PA2


enum Ht1632_Commandos_Enum
{
	HT1632_SYS_DIS					= 0x00,
	HT1632_SYS_EN					= 0x01,
	HT1632_LED_OFF					= 0x02,
	HT1632_LED_ON					= 0x03,
	HT1632_BLINK_OFF				= 0x08,
	HT1632_BLINK_ON					= 0x09,
	HT1632_SLAVE_MODE				= 0x10,
	HT1632_RC_MASTER_MODE			= 0x18,
	HT1632_EXT_CLK_MASTER_MODE		= 0x1C,
	HT1632_COM_OPTION				= 0x20,
	HT1632_PWM_DUTY					= 0xA0,
	
	__HT1632_COMMANDOS_MAX__
};

enum Ht1632_Com_Option_Enum
{
	HT1632_COM_OPTION_N_MOS_8_COM	= 0x00 << 2,	
	HT1632_COM_OPTION_N_MOS_16_COM	= 0x01 << 2,
	HT1632_COM_OPTION_P_MOS_8_COM	= 0x02 << 2,
	HT1632_COM_OPTION_P_MOS_16_COM	= 0x03 << 2,
	
	__HT1632_COM_OPTION_MAX__
};

enum Ht1632_Pwm_Duty_Commandos_Enum
{
	HT1632_PWM_DUTY_1_16,
	HT1632_PWM_DUTY_2_16,
	HT1632_PWM_DUTY_3_16,
	HT1632_PWM_DUTY_4_16,
	HT1632_PWM_DUTY_5_16,
	HT1632_PWM_DUTY_6_16,
	HT1632_PWM_DUTY_7_16,
	HT1632_PWM_DUTY_8_16,
	HT1632_PWM_DUTY_9_16,
	HT1632_PWM_DUTY_10_16,
	HT1632_PWM_DUTY_11_16,
	HT1632_PWM_DUTY_12_16,
	HT1632_PWM_DUTY_13_16,
	HT1632_PWM_DUTY_14_16,
	HT1632_PWM_DUTY_15_16,
	HT1632_PWM_DUTY_16_16,
	
	__HT1632_PWM_DUTY_MAX__
};


volatile uint8_t ht1632_shadowram[ ( COLS * ROWS ) / 4 ];

void ht1632c_send_page ();
void ht1632cSendCmd (uint8_t command);
void ht1632c_send_data (uint8_t address, uint8_t data);
void ht1632c_init (uint8_t bright);
void ht1632c_chipselect();
void ht1632c_chipfree();
void ht1632c_send_to_display (uint8_t data_to_send, uint8_t dtlen);
void ht1632c_set (uint8_t x, uint8_t y, uint8_t val);
uint8_t ht1632c_get (uint8_t x, uint8_t y);

#endif