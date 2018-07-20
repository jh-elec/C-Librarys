
void mcp23017_reg_write (int mcp23017_address, int regnumber, int regvalue)
{
	i2c_start(mcp23017_address+I2C_WRITE);
	i2c_write(regnumber);
	i2c_write(regvalue);
	i2c_stop();
}

void mcp23017_reg_read (int mcp23017_address, int regnumber)
{
	int regvalue;
	i2c_start(mcp23017_address+I2C_READ);
	i2c_write(regnumber);
	regvalue = i2c_readAck();
	i2c_stop();
}