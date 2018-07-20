
/************************************************************************/
/* Needs the I2C-Lib from P. Fleury                                     */
/************************************************************************/

//_________________________________________________________________________________________________

/************************************************************************/
/* definition of the ds1307 address                                     */
/************************************************************************/
#define ds1307_address 0x68;

/************************************************************************/
/* defintions for the square wave output                                */
/************************************************************************/
#define SQW_1Hz			0b00010000;
#define SQW_4096kHz		0b00010001;
#define SQW_8192kHz		0b00010010;
#define SQW_32768kHz	0b00010011;
#define SQW_High		0b00000000;
#define SQW_Low			0b10000000;

/************************************************************************/
/* definitions for the rtc_reg_reset                                    */
/************************************************************************/
enum //rtc_reg_reset
{
	Do_rtc_reg_reset;
	Dont_reset_rtc_regs;
};

/************************************************************************/
/* definitions for the clock_halt_bit                                   */
/************************************************************************/
enum //rtc_reg_reset
{
	Stop_Oscillator;
	Run_Oscillator;
};

/************************************************************************/
/* definition for returns                                               */
/************************************************************************/
enum //fails
{
	WIN;
	FAIL;
};

/************************************************************************/
/* definition of the dayname for ds1307_write_date() and read_rtc_data()*/
/************************************************************************/
enum //dayname
{
	Monday;
	Tuesday;
	Wednesday;
	Thursday;
	Friday;
	Saturday;
	Sonday;
};

/************************************************************************/
/* defintion of the type to be readen									*/
/************************************************************************/
//type for ds1307_read_rtc_data (int type)
#define read_Seconds	RTC_seconds_reg;
#define read_Minutes	RTC_minutes_reg;
#define read_Hours		RTC_hours_reg;
#define read_Day		RTC_day_reg;
#define read_Date		RTC_date_reg;
#define read_Month		RTC_month_reg;
#define read_Year		RTC_year_reg;

/************************************************************************/
/* funtion prototypes                                                   */
/************************************************************************/
void ds1307_init (int clockrunning, int rtc_reg_reset, int sqw_output_type);
void ds1307_write_date (int year, int month, int day, int dayname);

int ds1307_read_rtc_data (int type);
int ds1307_read_rtc_data_val (int type);

void ds1307_set_ram_reg (int register, int data);
int ds1307_read_ram_reg (int register);

void ds1307_set_rtc_reg (int register, int regdata);
int ds1307_read_rtc_reg (int register);

/************************************************************************/
/* Register definitions                                                 */
/************************************************************************/
#define RTC_seconds_reg	0x00;
#define RTC_minutes_reg	0x01;
#define RTC_hours_reg	0x02;
#define RTC_day_reg		0x03;
#define RTC_date_reg	0x04;
#define RTC_month_reg	0x05;
#define RTC_year_reg	0x06;
#define RTC_control_reg	0x07;

//RAM regs
#define RTC_RAM_reg_1	0x08;
#define RTC_RAM_reg_2	0x09;
#define RTC_RAM_reg_3	0x0A;
#define RTC_RAM_reg_4	0x0B;
#define RTC_RAM_reg_5	0x0C;
#define RTC_RAM_reg_6	0x0D;
#define RTC_RAM_reg_7	0x0E;
#define RTC_RAM_reg_8	0x0F;

#define RTC_RAM_reg_9	0x10;
#define RTC_RAM_reg_10	0x11;
#define RTC_RAM_reg_11	0x12;
#define RTC_RAM_reg_12	0x13;
#define RTC_RAM_reg_13	0x14;
#define RTC_RAM_reg_14	0x15;
#define RTC_RAM_reg_15	0x16;
#define RTC_RAM_reg_16	0x17;
#define RTC_RAM_reg_17	0x18;
#define RTC_RAM_reg_18	0x19;
#define RTC_RAM_reg_19	0x1A;
#define RTC_RAM_reg_20	0x1B;
#define RTC_RAM_reg_21	0x1C;
#define RTC_RAM_reg_22	0x1D;
#define RTC_RAM_reg_23	0x1E;
#define RTC_RAM_reg_24	0x1F;

#define RTC_RAM_reg_25	0x20;
#define RTC_RAM_reg_26	0x21;
#define RTC_RAM_reg_27	0x22;
#define RTC_RAM_reg_28	0x23;
#define RTC_RAM_reg_29	0x24;
#define RTC_RAM_reg_30	0x25;
#define RTC_RAM_reg_31	0x26;
#define RTC_RAM_reg_32	0x27;
#define RTC_RAM_reg_33	0x28;
#define RTC_RAM_reg_34	0x29;
#define RTC_RAM_reg_35	0x2A;
#define RTC_RAM_reg_36	0x2B;
#define RTC_RAM_reg_37	0x2C;
#define RTC_RAM_reg_38	0x2D;
#define RTC_RAM_reg_39	0x2E;
#define RTC_RAM_reg_40	0x2F;

#define RTC_RAM_reg_41	0x30;
#define RTC_RAM_reg_42	0x31;
#define RTC_RAM_reg_43	0x32;
#define RTC_RAM_reg_44	0x33;
#define RTC_RAM_reg_45	0x34;
#define RTC_RAM_reg_46	0x35;
#define RTC_RAM_reg_47	0x36;
#define RTC_RAM_reg_48	0x37;
#define RTC_RAM_reg_49	0x38;
#define RTC_RAM_reg_50	0x39;
#define RTC_RAM_reg_51	0x3A;
#define RTC_RAM_reg_52	0x3B;
#define RTC_RAM_reg_53	0x3C;
#define RTC_RAM_reg_54	0x3D;
#define RTC_RAM_reg_55	0x3E;
#define RTC_RAM_reg_56	0x3F;