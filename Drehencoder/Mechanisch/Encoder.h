/*
	Routine for read Mechanical Encoder
*/


/***Prototype functions***/

/* read the run right you are move it*/
/* Parameter : none
   Return    : run right = ++ || run left = --
*/
int8_t encode_read( void );


/* init the timer from the µC @ 1ms */
/* Parameter : none
   Return    : none
*/
void encode_init( void );