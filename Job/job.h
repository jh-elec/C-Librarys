/*************************************************************
*|
*|	\@file  	job.h
*|	\@brief 	-
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Job´s annehmen und verschicken
*|
*|	\@date		21/10/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.211019
*|
*|**************************************************************/

#ifndef __JOB_H__
#define __JOB_H__

#include <stdio.h>
//#include <avr.io>
#include "C:\Users\Hm\Desktop\C-Librarys\trunk\Frame\frame_types.h"


/*!<-- defines -- >*/
/*****************************************************************/

extern inline void _CALL_FUNCTION( void *pAddr )
{
    void (*MakeExecutable)(void) = pAddr;
    MakeExecutable();
};

typedef struct
{
    void    *pValue;
    uint8_t uiType;
    uint8_t uiElements;
}sJobTable_t;


/*****************************************************************/


/*!<-- global variables -- >*/
/*****************************************************************/



/*****************************************************************/


/*!<-- functions -- >*/
/*****************************************************************/



/*****************************************************************/


// end of file


#endif // __JOB_H__
