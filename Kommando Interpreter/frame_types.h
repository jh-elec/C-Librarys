/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> Kommando Interpreter
* Version           -> 1.0.1.1124
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 20.09.2017 07:50:01
* Description       ->	Datentypen  
*
*/

#ifndef __CMD_TYPES_H__
#define __CMD_TYPES_H__


enum Cmd_Data_Type_Enum
{
	DATA_TYPE_UCHAR,
    DATA_TYPE_UINT8,
    DATA_TYPE_UINT16,
    DATA_TYPE_UINT32,
	
    DATA_TYPE_FLOAT,
    DATA_TYPE_STRING,

	DATA_TYPE_CHAR,
    DATA_TYPE_INT8,
    DATA_TYPE_INT16,
    DATA_TYPE_INT32,

    __DATA_TYPE_MAX_INDEX__
};


typedef struct
{
	enum Cmd_Data_Type_Enum eDataTypes;
	uint8_t uiDataType;
}CmdDataTypes_t;

#endif 