/*
 * Copyright:      Jan Homann
 * Author   :      Jan Homann 
 * Version  :      1.0 
 * Date     :	   24.02.2016
 * Source   :      -Datasheet
 */

// Manual Reference Table 23 (Page 68). Function of Each Bit in the Interrupt Mask Register.

#define ZXTOA							0x0040		//Enables an interrupt when there is a zero-crossing timeout detection on Phase A
#define ZXA								0x0200		//Enables an interrupt when there is a zero-crossing in the voltage channel of Phase A
#define ZXB								0x0400		//Enables an interrupt when there is a zero-crossing in the voltage channel of Phase B
#define ZXC								0x0800		//Enables an interrupt when there is a zero-crossing in the voltage channel of Phase C
#define LENERGY							0x1000		//Enables an interrupt when the energy accumulations over LINECYC are finished
#define VACF							0x0080		//Switch the VARCF output pin to an output frequency that is proportional to the total apparent power
#define HALF_LINE_CYCLES				100			//Number of half-line cycles that the active energie are accumulated for the accumulation mode
													//100 half-line cycles * 10ms = 1 Sekunde


#define REG_RSTATUS			0x1A		// IRQ (Reset) Status Register, reset after a read operation
#define REG_CHKSUM			0x7E		// Checksum Register
#define REG_AWATTHR			0x01		// Phase A Active Energy	
#define REG_BWATTHR			0x02		// Phase B Active Energy	
#define REG_CWATTHR			0x03		// Phase C Active Energy	
#define REG_AIRMS			0x0A		// Phase A RMS Current		
#define REG_BIRMS			0x0B		// Phase B RMS Current		
#define REG_CIRMS			0x0C		// Phase C RMS Current		
#define REG_AVRMS			0x0D		// Phase A RMS Voltage		
#define REG_BVRMS			0x0E		// Phase B RMS Voltage		
#define REG_CVRMS			0x0F		// Phase C RMS Voltage	
#define REG_FREQ			0x10		// Frequency of the Line Input	
#define REG_TEMP			0x11		// Temperature Register
#define REG_GAIN			0x23		// PGA Gain Register
#define REG_AVRMSOS			0x33		// Phase A voltage RMS offset correction Register
#define REG_BVRMSOS			0x34		// Phase B voltage RMS offset correction Register
#define REG_CVRMSOS			0x35		// Phase C voltage RMS offset correction Register
#define REG_AIRMSOS			0x36		// Phase A current RMS offset correction Register
#define REG_BIRMSOS			0x37		// Phase B current RMS offset correction Register
#define REG_CIRMSOS			0x38		// Phase C current RMS offset correction Register
#define REG_AVRMSGAIN		0x24		// Phase A Voltage RMS Gain Register
#define REG_BVRMSGAIN		0x25		// Phase B Voltage RMS Gain Register
#define REG_CVRMSGAIN		0x26		// Phase C Voltage RMS Gain Register
#define REG_AIGAIN			0x27		// Phase A Current RMS Gain Register
#define REG_BIGAIN			0x28		// Phase B Current RMS Gain Register
#define REG_CIGAIN			0x29		// Phase C Current RMS Gain Register
#define REG_LCYCMODE		0x17		// Line cycle mode register
#define REG_LINECYC			0x1C		// Line cycle register
#define REG_ZXTOUT			0x1B		// Zero-Cross timeout register
#define REG_MASK			0x18		// IRQ Mask register
#define REG_AWATTOS			0x39		// Phase A Active Energy offset calibration Register
#define REG_BWATTOS			0x3A		// Phase B Active Energy offset calibration Register
#define REG_CWATTOS			0x3B		// Phase C Active Energy offset calibration Register
#define REG_AWG				0x2A		// Phase C Active Energy Gain		calibration Register
#define REG_BWG				0x2B		// Phase C Active Energy Gain		calibration Register
#define REG_CWG				0x2C		// Phase C Active Energy Gain		calibration Register
#define REG_AVAHR			0x07		// Phase A Apparent Power
#define REG_BVAHR			0x08		// Phase B Apparent Power
#define REG_CVAHR			0x09		// Phase C Apparent Power
#define REG_AVAG			0x30		// Phase A Apparent Power Gain Register
#define REG_BVAG			0x31		// Phase B Apparent Power Gain Register
#define REG_CVAG			0x32		// Phase C Apparent Power Gain Register
#define REG_OPMODE			0x13		// Operational Moder Register
#define REG_APHCAL			0x3F		// Phase A Phase Calibration Register
#define REG_BPHCAL			0x40		// Phase B Phase Calibration Register
#define REG_CPHCAL			0x41		// Phase C Phase Calibration Register
#define REG_COMPMODE		0x16		// Computation Mode Register
#define REG_WAVMODE			0x15		// Waveform Mode Register
#define REG_APCFNUM			0x45		// Active Power		CF Scaling Numerator	 Register
#define REG_APCFDEN			0x46		// Active Power		CF Scaling Denominator Register
#define REG_VARCFNUM		0x47		// Reactive Power CF Scaling Numerator	 Register
#define REG_VARCFDEN		0x48		// Reactive Power CF Scaling Denominator Register




#define FULL_SCALE_IRMS			0x182A7F	// 0x1D51C0 --> aprox. full-scale value of the current rms register for 50Hz, 5A and Integrattor off (Chip Manual Reference Page 29)
#define RMS_SPANNUNG_MAX		364			  // 364.513	--> Maximale Mess-RMS-Spannunng = (0,5V / (R22 / (R14+R17+R22))) / SQRT(2); +-0,5V is the specified full-scale analog input signal
#define FULL_SCALE_VRMS			0x198F24	// 0x199B82 --> aprox. full-scale value of the Voltage RMS register for 50Hz, 364,5V								 (Chip Manual Reference Page 29)
#define WIRKLEISTUNG_MAX		1820			// RMS_STROM_MAX * RMS_SPANNUNG_MAX bei ohmsche Last = 5 * 364
#define FULL_SCALE_WATT			0xD93DA		// full-scale value  of the Apparent Power register





/***Prototype functions***/






/* read 8 bit signed integer from slave
*
* parameter :  reg = address of register to write, data = data to write
*
* returns	: 8 bit signed 
*/
int8_t ADE7758_read_int8(uint8_t reg);

/* read 16 bit signed integer from slave
*
* parameter :  reg = address of register to write, data = data to write
*
* returns	: 16 bit signed
*/
int16_t ADE7758_read_int16(uint8_t reg);

/* read 16 bit unsigned integer from slave
*
* parameter :  reg = address of register to write, data = data to write
*
* returns	: 16 bit unsigned
*/
uint16_t ADE7758_read_uint16(uint8_t reg);

/* read 32 bit signed integer from slave
*
* parameter :  reg = address of register to write, data = data to write
*
* returns	: 32 bit signed
*/
uint32_t ADE7758_read_uint24(uint8_t reg);

/* write 8 bit unsigned to slave
*
* parameter :  reg = address of register to write, data = data to write
*
* returns	: none
*/
void ADE7758_write_uint8(uint8_t reg, uint8_t byte_1);

/* write 16 bit unsigned to slave
*
* parameter :  reg = address of register to write, data = data to write
*
* returns	: none
*/
void ADE7758_write_uint16(uint8_t reg, uint8_t byte_1, uint8_t byte_2);

/* write 24 bit unsigned to slave
*
* parameter :  reg = address of register to write, data = data to write
*
* returns	: none
*/
void ADE7758_write_uint24(uint8_t reg, uint8_t byte_3, uint8_t byte_2, uint8_t byte_1);

/* read temperature
*
* parameter : temperature offset
*
* returns	: temperature in °C
*/
int8_t ADE7758_readTEMP(void);

