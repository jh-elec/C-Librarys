/************************************************************************/
/************************************************************************/


void TSL2561_init(int address) 
{
    i2c_start(address+I2C_WRITE);
    i2c_write(0x80);
    i2c_write(0x03);
    i2c_stop();
}

int read_tsl2561_adc_raw (int address, int channel)
{
	int tsl2561_raw;
	int tsl2561_raw_msb;
	int tsl2561_raw_lsb;
	
	i2c_start(address+I2C_WRITE);
    if (channel == 0)
	{
        i2c_write(0xAC);
	}		
    else
	{
        i2c_write(0xAE);
	}	
    i2c_start(address+I2C_READ);
    tsl2561_raw_lsb = i2c_readAck();
    tsl2561_raw_msb = i2c_readNak();
    i2c_stop();	
	
	tsl2561_raw = 256 * tsl2561_raw_msb;
	tsl2561_raw = tsl2561_raw + tsl2561_raw_lsb;
	
	return tsl2561_raw;
}

int get_real_brightness_raw (int address)
{
	int brightness_ir_vis_raw;
	int brightness_ir_raw;
	int brightness_vis_raw;
	
	brightness_ir_vis_raw = read_tsl2561_adc_raw(address, adc_channel_ir_vis);
	brightness_ir_raw	  = read_tsl2561_adc_raw(address, adc_channel_ir);
	
	brightness_vis_raw = brightness_ir_vis_raw - brightness_ir_raw;
	
	return brightness_vis_raw;
}

int get_tsl2561_vis_lux (int address)
{
	int vis_raw;
	int vis_lux;
	
	vis_raw = get_real_brightness_raw(address);
	
	vis_lux = convert_tsl2561_raw_lux(vis_raw);
	
	return vis_lux;
}

int convert_tsl2561_raw_lux (int tsl2561_raw)
{
	int tsl2561_lux;
	
	//ToDo: write the conversion
	
	return tsl2561_lux;
}