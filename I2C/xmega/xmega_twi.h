/* Generated by CodeDescriptor 1.0.0.0 */
/*
* Project Name      -> xMEGA TWI (I2C)
* Version           -> 1.0
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 15.05.2017 16:18:16
* Description       -> Routines for TWI Handling
*
*
*
*/

#ifndef _XMEGA_TWI_H_
#define _XMEGA_TWI_H_


#define TWI_READ							0x01
#define TWI_WRITE							0x00

#define TWI_CLK								4e5
#define TWI_BAUD							(F_SYS, F_TWI) ((F_CPU / (2 * F_TWI)) - 5)
#define TWI_BAUDSETTING						TWI_BAUD(F_CPU, TWI_CLK)


/* twi_init
* @para             -> TWI Struct (TWI_t)
* @return           -> -none
* @description      ->
*/
extern void twi_init(TWI_t * twiname);


/* twi_write
* @para             -> TWI Struct (TWI_t) , reg = Register to write, data = Data to send
* @return           -> -none
* @description      ->
*/
extern void twi_write(TWI_t *twi, uint8_t reg , uint8_t data);


/* twi_read
* @para             -> *twi = Buffer, reg = Register to wish read
* @return           -> uint8_t Data from Register
* @description      ->
*/
extern uint8_t twi_read(TWI_t *twi, uint8_t reg);


/* twi_reads
* @para             -> *twi = Buffer, reg = Register to wish read, numb = Number of Bytes, *buff = Memory for Data
* @return           -> uint8_t Data from Register
* @description      ->
*/
void twi_reads(TWI_t *twi, uint8_t reg, uint8_t numb, uint8_t *buff);

#endif