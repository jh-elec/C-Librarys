/*
 * Copyright:      Jan Homann
 * Author:         Jan Homann
 * Version:        1.0
 */

#define BMP180                0xEE
#define BMP180_SCO            5

typedef struct
{
	int AC1;
	int AC2;
	int AC3;
	unsigned int AC4;
	unsigned int AC5;
	unsigned int AC6;
	int B1;
	int B2;
	int MB;
	int MC;
	int MD;
	
	long pressure;
	long temperature;
	
}bmp180_t;


extern int temp_tmp;
extern long temp_result, press_result;

extern int temp_tmp;
extern long press_tmp;

extern bmp180_t cal;

extern	void bmp180Init(void);

extern char bmp180GetCal(bmp180_t *cal);
extern char bmp180ReadBytes(unsigned char *values, char length);
extern char bmp180ReadInt(char address, int *value);
extern char bmp180ReadUint(char address, unsigned int *value);
extern char bmp180ReadByte(char address, char *value);
extern char bmp180ReadUByte(char address, unsigned char *value);
extern char bmp180WriteBytes(unsigned char *values, char length);

extern char bmp180WriteByte(char address, char value);
extern char bmp180StartTemperature(int *value);
extern char bmp180StartPressure(long *value, unsigned char oss);
extern char bmpGetTemperaure(bmp180_t cal, long *temperatur);
extern char bmp180GetPressure( bmp180_t *cal , unsigned char oss );
extern char bmpGetID(char *id);


extern	void bmp180_show_cal(bmp180_t cal);

