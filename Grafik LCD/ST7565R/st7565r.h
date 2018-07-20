/* Generated by CodeDescriptor 1.3.0.0714 */
/*
* Project Name      -> ST7565R
* Version           -> 1.0.1.0816
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 16.08.2017 07:08:38
* Description       -> Description
*
*
*
*/



#ifndef __ST7565R_H__
#define __ST7565R_H__

#include <avr/io.h>


/* change this settings to your display */
#define GLCD_WIDTH_SIZE					128
#define GLCD_HIGH_SIZE					32
#define GLCD_PAGE_SIZE					4


/* change your code here */
#define GLCD_CTR_A0_DDR					PORTD.DIR
#define GLCD_CTR_A0_PORT				PORTD.OUT
#define GLCD_CTR_A0_bp					0

#define GLCD_CTR_SD_DDR					PORTD.DIR
#define GLCD_CTR_SD_PORT				PORTD.OUT
#define GLCD_CTR_SD_bp					3

#define GLCD_CTR_SCK_DDR				PORTD.DIR
#define GLCD_CTR_SCK_PORT				PORTD.OUT
#define GLCD_CTR_SCK_bp					1

#define GLCD_CTR_CS_DDR					PORTF.DIR
#define GLCD_CTR_CS_PORT				PORTF.OUT
#define GLCD_CTR_CS_bp					3

#define GLCD_CTR_RESET_DDR				PORTA.DIR
#define GLCD_CTR_RESET_PORT				PORTA.OUT
#define GLCD_CTR_RESET_bp				3

#define GLCD_CTR_BACKLIGHT_DDR			PORTE.DIR
#define GLCD_CTR_BACKLIGHT_PORT			PORTE.OUT
#define GLCD_CTR_BACKLIGHT_bp			4
		
/*************************************************/


/* DONT TOUCH! Commands */
#define BLACK						1
#define WHITE						0

#define GLCD_WIDTH					128
#define GLCD_HEIGHT					32

#define CMD_DISPLAY_OFF				0xAE
#define CMD_DISPLAY_ON				0xAF

#define CMD_SET_DISP_START_LINE		0x40
#define CMD_SET_PAGE				0xB0

#define CMD_SET_COLUMN_UPPER		0x10
#define CMD_SET_COLUMN_LOWER		0x00

#define CMD_SET_ADC_NORMAL			0xA0
#define CMD_SET_ADC_REVERSE			0xA1

#define CMD_SET_DISP_NORMAL			0xA6
#define CMD_SET_DISP_REVERSE		0xA7

#define CMD_SET_ALLPTS_NORMAL		0xA4
#define CMD_SET_ALLPTS_ON			0xA5
#define CMD_SET_BIAS_9				0xA2
#define CMD_SET_BIAS_7				0xA3

#define CMD_RMW						0xE0
#define CMD_RMW_CLEAR				0xEE
#define CMD_INTERNAL_RESET			0xE2
#define CMD_SET_COM_NORMAL			0xC0
#define CMD_SET_COM_REVERSE			0xC8
#define CMD_SET_POWER_CONTROL		0x28
#define CMD_SET_RESISTOR_RATIO		0x20
#define CMD_SET_VOLUME_FIRST		0x81
#define CMD_SET_VOLUME_SECOND		0
#define CMD_SET_STATIC_OFF			0xAC
#define CMD_SET_STATIC_ON			0xAD
#define CMD_SET_STATIC_REG			0x0
#define CMD_SET_BOOSTER_FIRST		0xF8
#define CMD_SET_BOOSTER_234			0
#define CMD_SET_BOOSTER_5			1
#define CMD_SET_BOOSTER_6			3
#define CMD_NOP						0xE3
#define CMD_TEST					0xF0


#define PAGE_ADDRESS_SET			0xBF
#define DISPLAY_STARTLINE_SET		0x7F

/*
*	Berechnet die Page vom Display anhand von Koordianten
*	Damit kann man die Page vom Display ausw�hlen an der man schreiben m�chte
*
*	72 = Da die Fonts von unten nach oben geschrieben werden ist das der Offset.
*	Sollte man das sp�ter noch mal �ndern wollen m�sste man sich "glcdPutc" anschauen
*/
#define GLCD_CALC_PAGE( PIXEL )	( ( ( GLCD_HIGH_SIZE + 8 ) - ( ( PIXEL + 2 ) * 8 ) ) )


/*************************************************/


/* Init. Struct
* @para             -> -none
* @return           -> -none
* @description      -> change for your application
*/
static const uint8_t glcd_config[] =
{
  CMD_SET_DISP_START_LINE,		// Display Start Line 0
  CMD_SET_ADC_NORMAL,			// Display Normal / reverse A0
  CMD_SET_COM_NORMAL,			// Display Normal / reverse C8
  CMD_SET_DISP_NORMAL,
  CMD_SET_BIAS_9,				// Set Bias
  0x2F,							// Power Control set
  CMD_SET_BOOSTER_FIRST,		// Booster Ratio set
  CMD_SET_BOOSTER_234,			// ""
  0x00,							// Contrast set
  CMD_SET_VOLUME_FIRST,
  0x00,
  CMD_SET_STATIC_OFF,			// Static Indicator set
  0x00,							// ""
  CMD_DISPLAY_ON				// Display on/off
};

typedef struct
{
	/*
	*	Nach dem Aufruf von ( calcFontStart() )
	*	Enth�lt "indexNum" den Anfang der Pixeldaten f�r
	*	das gesuchte Zeichen
	*/
	uint16_t indexNum;
	
	/*
	*	Zeichenbreite
	*/
	uint8_t  width;
	
	/*
	*	Zeiger auf den aktuell benutzten Font
	*/
	const uint8_t __flash *fontPtr;
}font_t;


/*
*	Aufbau der Fontdatei
*	Die ersten Bytes m�ssen dieser Struktur folgen
*/
enum fontStruct
{
	FONT_CHAR_WIDTH,
	FONT_CHAR_HIGH,
	FONT_FIRST_CHAR,
	FONT_NUM_OF_CHARS,
	FONT_BEGINN_RAW_DATA,
	FONT_FONT_FIXED
	
}fontStruct_enum;

/*
*	Aufbau der Bitmapdatei
*	Die ersten Bytes m�ssen dieser Struktur folgen
*/
enum bitmapStruct
{
	BITMAP_WIDTH,
	BITMAP_HIGH,
	BITMAP_BEGINN_RAW_DATA,
	
}bitmapStruct_enum;

/*
*	Berechnet den Abstand "x" zum aktuellen Font
*/
uint8_t calcXWidth(uint8_t x);

/*
*	Berechnet die breite eines Zeichens damit man Zeichenweise schreiben kann
*/
//#define GLCD_Y_CALC(x)	(x*(actualFontWidth(font)))

/*
*	Aufbau einer Fontdatei
*
*	const uint8_t __flash Arial_14[]=
*	{
*		0x1E, 0x6C,			// size
*		0x0A,				// Breite jedes einzelnen Zeichens
*		0x0E,				// H�he jedes einzelnen Zeichens
*		0x20,				// Erstes Zeichen im Array
*		0x60,				// Anzahl der Zeichen im Array
*	
*		0x68,				// Ab hier gehen die eigentlichen Pixeldaten los
*		0x00,				// Ist das Font in der Breite variable oder fest? variable = 0, fest = 1
*	
*		// char widths
*		0x04, 0x02, 0x05, 0x06, 0x07, // Wenn die Zeichen variable sind, haben diese immer eine Unterschiedliche
*										 Breite. Die Breite wird einfach mit einer Funktion aufsummiert, so erh�lt
*										 man sp�ter den Anfang des jeweils gesuchten Zeichens. Nat�rlich muss
*										 das gesuchte Zeichen noch mit dem ersten Zeichen im Array subtrahiert werden ( gesuchtes_zeichen - erstes_zeichen_im_array).
*										 Anhand der Differenz wei� man wie oft die Breite des jeweiligen Zeichens aufsummiert werden muss.
*	
*		// font data
*		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 32
*		0xFE, 0xFE, 0x18, 0x18, // 33
*		0x1E, 0x1E, 0x00, 0x1E, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, // 34
*		0x90, 0xF8, 0x9E, 0x90, 0xF8, 0x9E, 0x1C, 0x00, 0x00, 0x1C, 0x00, 0x00, // 35
*		0x18, 0x3C, 0x26, 0xFF, 0x66, 0xCC, 0x88, 0x04, 0x0C, 0x18, 0x3C, 0x18, 0x0C, 0x04, // 36	
*	};
*/

/*
*	Aufbau eines Bitmaps
*
*	const unsigned char __flash key [] =
*	{
*		110,	// Breite des Bildes.: Hier muss die Bildbreite festgelegt werden
*		48,		// H�he des Bildes.: Hier muss die Bildh�he festgelegt werden
*	
*		3,		// Offset.: Hier steht wie viele Bytes vor den eigentlichen Pixeldaten (z.B als Bildinformationen) kommen.
*	
*		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFC,
*		0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFE,
*		0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
*		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
*		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
*		...
*	};
*/






/* glcdSendData
* @para             -> byte = Data to send
* @return           -> -none
* @description      -> Send Data to Display Controller
*/
void glcdSendData(uint8_t byte);

/* glcdSendCmd
* @para             -> cmd = Command to send
* @return           -> -none
* @description      -> Send Command to Display Controller
*/
void glcdSendCmd(uint8_t Cmd);

/* glcdInit
* @para             -> cont = Init Display Contrast
* @return           -> -none
* @description      -> Set the Contrast of Display Screen
*/
void glcdInit( void );

/* glcdDrawRect
* @para             -> y = 0..63 , x = 0..127 , h = 0..63 , w = 0..127 , *buff = Buffer to save the Data
* @return           -> -none
* @description      -> Draw a Rectangle to Display Screen
*/
void glcdDrawRect(uint8_t y, uint8_t x, uint8_t h, uint8_t w, uint8_t *buff);

/* glcdSetPageColumn
* @para             -> pageAddr = 0..7 , column = 0..127
* @return           -> -none
* @description      -> Set the "Cursor" to y,x
*/
void glcdSetPageColumn(uint8_t pageAddr, uint8_t column);

/* glcdSetPixel
* @para             -> y = 0..63 , x = 0..127
* @return           -> -none
* @description      -> Set Pixel on y,x
*/
void glcdSetPixel(uint8_t y, uint8_t x);

/* glcdClearPixel
* @para             -> y = 0..63 , x = 0..127
* @return           -> -none
* @description      -> Clear Pixel on y,x
*/
void glcdClearPixel(uint8_t y, uint8_t x);

/* glcdDrawLine
* @para             -> y = 0..63 , x = 0..127 , leng
* @return           -> -none
* @description      -> Clear Pixel on y,x
*/
void glcdDrawLine(uint8_t y, uint8_t x, uint8_t leng);

/* glcdClearLine
* @para             -> y = 0..63 , x = 0..127 , leng
* @return           -> -none
* @description      -> Clear a line on Display
*/
void glcdClearLine(uint8_t y, uint8_t x, uint8_t leng);

/* glcdClear
* @para             -> -none
* @return           -> -none
* @description      -> Clear Display Screen
*/
void glcdClear(void);

/* glcdSetPageColumn
* @para             -> pageAddr = Page 0 - 7 , column = 0..127
* @return           -> -none
* @description      -> Set Coursor to y , x Position
*/
void glcdSetPageColumn(uint8_t pageAddr, uint8_t column);

/* glcdPuts
* @para             -> *str = pointer of the string
* @return           -> -none
* @description      -> Write String on Display Screen
*/
void glcdPuts(char *str, uint8_t y , uint8_t x);

/* glcdPrintImage
* @para             -> *image = address of picture, sizeofimage = size of picture array , y = 0..64 , x = 0..128
* @return           -> -none
* @description      -> Write Array Image to Display Screen
*/
void glcdPrintImage(const __flash unsigned char *image, uint16_t sizeofimage, uint8_t y, uint8_t x);

/* glcdPutc
* @para             -> c = Character to write on Display Screen, y = 0..63 , x = 0..127
* @return           -> -none
* @description      -> Display a Character on Display
*/
void glcdPutc(char c, uint8_t y , uint8_t x);

/* glcdSetFont
* @para             -> *chooseFontPtr = Address of Array Image
* @return           -> -none
* @description      -> display a picture on display
*/
void glcdSetFont(const uint8_t __flash *chooseFontPtr);


void glcdFill(void);


#endif