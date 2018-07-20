
/************************************************************************/
/* Needs the I2C-Lib form Peter Fleury                                  */
/************************************************************************/

//I2C slave address |L|H|L|L|A2|A1|A0|R/W|
//I/O data bus |P7|P6|P5|P4|P3|P2|P1|P0

void pcf8574_output (int portexaddress, int portexdata);
int	 pcf8574_input	(int portexaddress);

#define pcf8574_address_LLL		32;
#define pcf8574_address_LLH		33;
#define pcf8574_address_LHL		34;
#define pcf8574_address_LHH		35;
#define pcf8574_address_HLL		36;
#define pcf8574_address_HLH		37;
#define pcf8574_address_HHL		38;
#define pcf8574_address_HHH		39;