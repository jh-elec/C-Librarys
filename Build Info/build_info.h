/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> Build Infos
* Version           -> 1.0.0.1231
* Author            -> Jan Homann @ Workstadion.: XXXJANIIIX3XXX
* Build Date        -> 31.12.2017 11:19:49
* Description       -> Building infos from Preprocessor
*
*
*	Versions - String.:
*
*	Hauptversion(Major) | Nebenversion(Minior) |  Tag & Monat
*
*
*	Hauptversion.:			Gibt an wie oft die Firmware komplett neu erstellt wurde
*	Nebenversion.:			Gibt an ob es eine neue Subroutine gibt
*	Tag & Monat.:			Datum der letzten ‹bersetzung
*/

#ifndef _BUILD_INFO_H_
#define _BUILD_INFO_H_

/* BuildVersion
* @para             -> none
* @return           -> pointer @ string address
* @description      -> build info of project
*/
char *BuildVersion(void);


#endif // _BUILD_INFO_H_
