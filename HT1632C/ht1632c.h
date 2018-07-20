// ht1632c
// custom HT1632C driver library for
// JY-MCU 3208 Display
// Rick Shear

#ifndef _HT1632C_H
#define _HT1632C_H

#ifndef ROWS
  #define ROWS 8
#endif

#ifndef COLS
  #define COLS 32
#endif


#define csreg  DDRB
#define wrreg  DDRB
#define dtreg  DDRB

#define csport PORTB
#define wrport PORTB
#define dtport PORTB

#define cspin  PB3
#define wrpin  PB4
#define dtpin  PB5

volatile uint8_t ht1632_shadowram[(COLS*ROWS)/4];

void ht1632c_send_page ();
void ht1632c_send_command (uint8_t command);
void ht1632c_send_data (uint8_t address, uint8_t data);
void ht1632c_init ();
void ht1632c_chipselect();
void ht1632c_chipfree();
void ht1632c_send_to_display (uint8_t data_to_send, uint8_t dtlen);
void ht1632c_set (uint8_t x, uint8_t y, uint8_t val);
uint8_t ht1632c_get (uint8_t x, uint8_t y);


#endif