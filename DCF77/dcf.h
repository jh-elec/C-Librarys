/* *****************************************************************************
 * File:        dcf.h
 * Project:     -
 * Author:      Nicolas Meyertöns
 * Version:     1.4 (26.01.2015)
 * Web:         http://pic-projekte.de/
 * ****************************************************************************/

#ifndef DCF_H
#define	DCF_H

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>


/*******************************************************************************
 * DCF77 Datenstruktur
 */

enum eDay
{
	Montag,
	Dienstag,
	Mittwoch,
	Donnerstag,
	Freitag,
	Samstag,
	Sonntag
	
};  

struct sTime
{
	uint8_t Day;
	uint8_t Month;
	uint16_t Year;
	uint8_t Hour;
	uint8_t Min;
	uint8_t Sec;
	enum eDay wDay;
	bool stime;
	
}; typedef struct sTime tTime;
/*******************************************************************************
 * Hier musst du einstellen an welchem Port-Pin das Datensignal des DCF-Moduls
 * angeschlossen ist. Bitte vergiss nicht auf das zugehörige Trisbit zu setzen.
 */
 
#define DCF77_DATA  (!(PINB & (1<<PB2)))

/*******************************************************************************
 * Es folgen einige Makros (nicht ändern!)
 */

#define DCF_LOW     0   	// ~100 ms gemessen
#define DCF_HIGH    1   	// ~200 ms gemessen
#define DCF_START   3   	// Startbit gemessen >> 200ms
#define DCF_ZEIT    21  	// Start der Zeit- und Datumsinfo
#define DCF_MIN     21  	// Beginn Minuten (x7)
#define DCF_STD     29  	// Beginn Stunden (x6)
#define DCF_KT      36  	// Beginn Kalendertag (x6)
#define DCF_WT      42  	// Beginn Wochentag (x3)
#define DCF_M       45  	// Beginn Monat (x5)
#define DCF_J       50  	// Beginn Jahr (x8)

#define MESZ        true
#define MEZ         false

 /*******************************************************************************
 * Die folgenden Variablen müssen auch außerhalb der Datei dcf.c zugänglich sein
 * und werden aus diesem Grund mit dem Schlüsselwort extern versehen.
 */
 
 
 extern volatile bool checkDCF;
 extern volatile bool syncDCF;
 extern volatile tTime time;
 extern volatile bool dcf_Start;
 extern volatile bool Enable_DCF77_Scan;
 
 /*******************************************************************************
 * Es folgt die Liste mit den Prototypen der implementierten Funktionen.
 */
 
bool dcf_running (void);
bool dcf_collect (void);
bool dcf_decode (void);
void dcf_check (void);

#endif	/* DCF_H */