/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> Kommando Interpreter
* Version           -> 1.0.1.1124
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 20.09.2017 07:50:01
* Description       ->	Exitcodes   
*
*/

#ifndef __CMD_EXIT_H__
#define __CMD_EXIT_H__


enum Cmd_Exitcodes_Enum
{
	CMD_EXIT_OK,
	CMD_EXIT_FAIL,
};

typedef struct
{
	enum Cmd_Exitcodes_Enum eExit;
	uint8_t uiExit;
}CmdExit_t;

#endif 
