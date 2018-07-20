/************************************************************************/
/* Writes to the RTC-Data												*/
/************************************************************************/
void ds1307_init (int clockrunning, int rtc_reg_reset, int sqw_output_type)
{
	int configuration;
	
	if (rtc_reg_reset == Do_rtc_reg_reset)
	{
		ds1307_set_rtc_reg (RTC_seconds_reg, 0x00);
		ds1307_set_rtc_reg (RTC_minutes_reg, 0x00);
		ds1307_set_rtc_reg (RTC_hours_reg, 0x00);
		ds1307_set_rtc_reg (RTC_day_reg, 0x00);
		ds1307_set_rtc_reg (RTC_date_reg, 0x00);
		ds1307_set_rtc_reg (RTC_month_reg, 0x00);
		ds1307_set_rtc_reg (RTC_year_reg, 0x00);
	}
	
	if (clockrunning = Stop_Oscillator)
	{
		configuration = ds1307_read_rtc_reg(RTC_seconds_reg);
		configuration = configuration + 128;
		
		ds1307_set_rtc_reg(RTC_seconds_reg, configuration);
		
	}
	
	//write for square wave output	
	ds1307_set_rtc_reg (RTC_control_reg, sqw_output_type);
}

void ds1307_write_date (int year, int month, int day, int dayname)
{
	ds1307_set_rtc_reg (RTC_year_reg, year);
	ds1307_set_rtc_reg (RTC_month_reg, month);
	ds1307_set_rtc_reg (RTC_day_reg, dayname);
	ds1307_set_rtc_reg (RTC_date_reg, day);
}

/************************************************************************/
/* Reads of the RTC-Data												*/
/************************************************************************/
int ds1307_read_rtc_data (int type)
{
	int ret;
	int reg;
	
	reg = type;
	
	i2c_start(ds1307_address+I2C_WRITE);	
	i2c_write(reg);				
	i2c_start(ds1307_address+I2C_READ);
	ret = i2c_readNak();
	i2c_stop();
	
	return ret;
}

/************************************************************************/
/* Reads and Writes to the RAM                                          */
/************************************************************************/
int ds1307_set_ram_reg (int register, int regdata)
{
	if (register < RTC_RAM_reg_1 | register > RTC_RAM_reg_56) //check address fail
	{
		return FAIL;
	} 
	else
	{
		if ( regdata > 255) //check size fail
		{
			return FAIL;
		} 
		else
		{
			i2c_start(ds1307_address+I2C_WRITE);
			i2c_write(register);
			i2c_write(regdata);
			i2c_stop();
			
			return WIN;
		}
	}	
}

int ds1307_read_ram_reg (int register)
{
	int ret;
	
	if (register < RTC_RAM_reg_1 | register > RTC_RAM_reg_56) //check address fail
	{
		return FAIL;
	} 
	else
	{
		i2c_start(ds1307_address+I2C_WRITE);
		i2c_write(register);
		i2c_start(ds1307_address+I2C_READ);
		ret = i2c_readNak();
		i2c_stop();
	
		return ret;
	}		
}

/************************************************************************/
/* Reads and Writes of the RTC-Registers								*/
/************************************************************************/
void ds1307_set_rtc_reg (int register, int regdata)
{
	if (register < RTC_seconds_reg | register > RTC_year_reg)  //check address fail
	{
		return FAIL;
	} 
	else
	{
		if ( regdata > 255) //check size fail
		{
			return FAIL;
		} 
		else
		{
			i2c_start(ds1307_address+I2C_WRITE);
			i2c_write(register);
			i2c_write(regdata);
			i2c_stop();
			
			return WIN;
		}
}

int ds1307_read_rtc_reg (int register)
{
	int ret;
	
	if (register < RTC_seconds_reg | register > RTC_year_reg) //check address fail
	{
		return FAIL;
	} 
	else
	{
		i2c_start(ds1307_address+I2C_WRITE);
		i2c_write(register);
		i2c_start(ds1307_address+I2C_READ);
		ret = i2c_readNak();
		i2c_stop();
	
		return ret;
	}
}