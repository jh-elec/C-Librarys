
void pcf8574_output (int portexaddress, int portexdata)
{
	i2c_start(portexaddress+I2C_WRITE);
	i2c_write(portexdata);
	i2c_write(portexdata);
	i2c_stop();
}

int	 pcf8574_input	(int portexaddress)
{
	int portexin1;
	int portexin2;
	
	i2c_start(portexaddress+I2C_READ);
	
	portexin1 = i2c_readAck();
	portexin2 = i2c_readAck();
	
	i2c_stop();
	
	if (portexin1 == portexin2)
	{
		return portexin1;
	}
	else
	{
		return "ERR";
	}
}