
#ifndef AVR_ADC
#define AVR_ADC

#include <avr/io.h>

/*! \brief Init ADC sub-system
*/
void init_ADC ();

/*! \brife Read ADC value at an channel
* \b Require ADC inited
* \param[in] channel ADC channel to read
* \return ADC output value
*/
uint16_t read_ADC(uint8_t ch);

#endif