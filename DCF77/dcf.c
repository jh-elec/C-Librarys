/* *****************************************************************************
 * File:        dcf.c
 * Project:     -
 * Author:      Nicolas Meyertöns
 * Version:     1.4 (26.01.2015)
 * Web:         http://pic-projekte.de/
 * ****************************************************************************/

/* Einbinden von Headerdateien */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "dcf.h"

/*******************************************************************************
 * Anlegen einiger globaler Variablen
 */
 
volatile bool dcf_available;             // Es liegen Daten zum Abholen bereit
volatile uint8_t dcf_value;              // Hier sind die Daten abgelegt
volatile bool dcf_start;                 // Startbit wurde erkannt/Messung läuft
volatile static uint8_t dcf_data[8];     // Datenvektor (Bitweise)
volatile bool checkDCF;                  // Pin Überwachung
volatile bool syncDCF;
volatile bool Enable_DCF77_Scan;

volatile tTime time = { 1,1,2014,12,05,0,Sonntag,MEZ };

/*******************************************************************************
 * 
 * Diese Funktion signalisiert den Start eines DCF77 Strangs.
 * Sobald das Starbit erkannt wurde liefert die Funktion true zurück.
 * Nachdem alle 59 Bit empfangen wurden wechselt der Rückgabewert auf
 * false und es wird nach der nächsten Gültigen Sequenze gesucht.
 *
 * Hinweis: Diese Funktion arbeitet mit dcf_collect Hand in Hand.
 *          Das Enable_DCF77_Scan dcf_start gibt an ob das Startbit des DCF Signals
 *          erkannt wurde. Zurück gesetzt wird es durch die dcf_collect
 *          Funktion nach einlesen aller 59 Bits.
 */

bool dcf_running(void)
{
    /*Wurde das Startbit empfangen?*/
    if( dcf_available && (dcf_value == DCF_START) && !dcf_start )
    {
        dcf_start = true;
        dcf_available = false;
    }

    return dcf_start;
}

/*******************************************************************************
 * 
 * Hier werden nach Erkennung des Startbits durch dcf_running alle
 * 59 Bits des DCF Signals eingelesen und speicheroptimiert gesichert.
 * Nachdem alle Bits empfangen wurden wird das Enable_DCF77_Scan dcf_start gelöscht
 * um zu signalisieren, dass die Übertragung abgeschlossen ist.
 *
 * Die Funktion liefert true zurück, sobald eine Sequenze vollständig
 * empfangen wurde. Danach kann mit der Auswertung mit dcf_decode
 * begonnen werden.
 */

bool dcf_collect(void)
{
    /*Aktuelle Bitposition*/
    static uint8_t dcf_cnt = 0;

    /*Sind neue Daten verfügbar?*/
    if( dcf_available )
    {
        /*Wurde eine 1 empfangen?*/
        if( dcf_value == DCF_HIGH )
        {
            /*Das entsprechende Bit setzten*/
            dcf_data[(dcf_cnt/8)] |=  (0x01 << (dcf_cnt % 8));
        }
        /*Nein, es wurde eine 0 empfangen*/
        else if( dcf_value == DCF_LOW )
        {
            /*Das entsprechende Bit löschen*/
            dcf_data[(dcf_cnt/8)] &= ~(0x01 << (dcf_cnt % 8));
        }
        /* Nein, es beginnt eine neue Sequenze */
        else if( dcf_value == DCF_START )
        {
            dcf_cnt = 0;
            dcf_available = false;
            return false;
        }
        /*Das nächste Bit*/
        dcf_cnt++;
        /*Daten sind verarbeitet/als*/
        dcf_available = false;
    }

    /*Alle Bits empfangen?*/
    if( dcf_cnt > 58 )
    {
        dcf_cnt = 0;
        dcf_start = false;
        return true;
    }
    else
    {
        return false;
    }
}

/*******************************************************************************
 * 
 * Hier findet die Dekodierung des empfangenen DCF77 Signals statt.
 * Es wird des Weiteren auch auf Parität der Daten geprüft um zu vermeiden,
 * dass fehlerhaft empfangene Daten als gültig gewertet werden. Somit wird
 * sichergestellt, dass die Urhzeit stehts korrekt empfangen wird.
 *
 * Die Funktion gibt bei Erfolg einen Pointer auf eine Struktur mit den
 * gültigen Zeitinformationen zurück. Im Fehlerfall liefert die Funktion
 * einen Nullpointer.
 */

bool dcf_decode (void)
{
    uint8_t par = 0, k = 0;

    /*Paritätsprüfung*/

    for(k=DCF_ZEIT; k<59; k++)
    {
        if( dcf_data[(k)/8] & (0x01 << (k%8)) )
        {
            par++;
        }
    }
    if( par % 2 )
    {
        /*Fehlgeschlagen*/
        return false;
    }

    /*Auswertung*/

    time.Sec = 0;
    time.Min = 0;
    time.Hour = 0;
    time.Day = 0;
    time.wDay = 0;
    time.Month = 0;
    time.Year = 0;

    /*Zusammenrechnen der Einer-Stellen*/
    for(k=0; k<4; k++)
    {
        /*Minute*/
        if( dcf_data[(DCF_MIN+k)/8] & (0x01 << ((DCF_MIN+k) % 8)) )
        {
            time.Min |= (0x01 << k);
        }
        /*Stunde*/
        if( dcf_data[(DCF_STD+k)/8] & (0x01 << ((DCF_STD+k) % 8)) )
        {
            time.Hour |= (0x01 << k);
        }
        /*Tag*/
        if( dcf_data[(DCF_KT+k)/8] & (0x01 << ((DCF_KT+k) % 8)) )
        {
            time.Day |= (0x01 << k);
        }
        /*Wochentag*/
        if( dcf_data[(DCF_WT+k)/8] & (0x01 << ((DCF_WT+k) % 8)) && (k < 3) )
        {
            time.wDay |= (0x01 << k);
        }
        /*Monat*/
        if( dcf_data[(DCF_M+k)/8] & (0x01 << ((DCF_M+k) % 8)) )
        {
            time.Month |= (0x01 << k);
        }
        /*Jahr*/
        if( dcf_data[(DCF_J+k)/8] & (0x01 << ((DCF_J+k) % 8)) )
        {
            time.Year |= (0x01 << k);
        }
    }

    /*Hinzuaddieren der Zehner-Stellen*/
    for(k=4; k<8; k++)
    {
        /*Minute*/
        if( dcf_data[(DCF_MIN+k)/8] & (0x01 << ((DCF_MIN+k) % 8)) && (k < 7) )
        {
            time.Min += (0x0A << (k-4));
        }
        /*Stunde*/
        if( dcf_data[(DCF_STD+k)/8] & (0x01 << ((DCF_STD+k) % 8)) && (k < 6) )
        {
            time.Hour += (0x0A << (k-4));
        }
        /*Tag*/
        if( dcf_data[(DCF_KT+k)/8] & (0x01 << ((DCF_KT+k) % 8)) && (k < 6) )
        {
            time.Day += (0x0A << (k-4));
        }
        /*Monat*/
        if( dcf_data[(DCF_M+k)/8] & (0x01 << ((DCF_M+k) % 8)) && (k == 4) )
        {
            time.Month += (0x0A << (k-4));
        }
        /*Jahr*/
        if( dcf_data[(DCF_J+k)/8] & (0x01 << ((DCF_J+k) % 8)) )
        {
            time.Year += (0x0A << (k-4));
        }
    }

    /*Die Information des Jahres erfolgt über DCF77 zweistellig*/
    time.Year += 2000;

    /*Sommer-/Winterzeit*/
    if( dcf_data[2] & 0x02 )
    {
        time.stime = MESZ;
    }
    else
    {
        time.stime = MEZ;
    }

    /*Erfolg melden*/
    return true;
}

/*******************************************************************************
 * 
 * Diese Funktion überwacht den Datenstrom des DCF77 Signals.
 * Es wird die Länge eines Impulses und/oder einer Impulspause
 * gemessen und ausgewertet.
 */
 
void dcf_check (void)
{
    /*Zur Messung der DCF77 Puls-/Pausenlänge*/
    static uint16_t dcf_cntL = 0;
    static uint16_t dcf_cntH = 0;

    /* Überwachung des Signals */
    if( DCF77_DATA )
    {
        /* >1500 ms low? */
        if( dcf_cntL > 1500 )
        {
			//PORTD |= (1<<PD1);
            dcf_available = true;
            dcf_value = DCF_START;
            dcf_cntL = 0;
        }
        /* Störimpuls */
        else
        {
            dcf_cntL = 0;
        }

        /*High-Counter*/
        dcf_cntH++;
    }
    else
    {
        /* >180 ms high? */
        if( dcf_cntH > 180 )
        {
			//PORTD ^= (1<<PD1);
            dcf_available = true;
            dcf_value = DCF_HIGH;
            dcf_cntH = 0;
        }
        /* >80 ms high? */
        else if( dcf_cntH > 80 )
        {
			//PORTD ^= (1<<PD1);
            dcf_available = true;
            dcf_value = DCF_LOW;
            dcf_cntH = 0;
        }
        /* Störimpuls */
        else
        {
            dcf_cntH = 0;
        }

        /*Low-Counter*/
        dcf_cntL++;
    }
    

    /*Läuft eine Aufnahme?*/
    if( dcf_running() )
    {
		
        /*Wurden alle Bits empfangen?*/
        if( dcf_collect() )
        {
			
			
			//PORTD |= (1<<PD1);
            /*Erfolgreich dekodiert?*/
            if( dcf_decode() )
            {
				Enable_DCF77_Scan = false;
                // Abgleich fertig!
                syncDCF = false;
            }
			
            /*Abgleich fehlgeschlagen*/
            else
            {
                // Abgleich fertig!
                syncDCF = false;
            }
        }
    }
}