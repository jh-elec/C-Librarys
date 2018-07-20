// ht1632c
// Basic HT1632C driver library for
// JY-MCU 3208 Display
// Rick Shear

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

#include "ht1632c.h"



void ht1632c_init ()  // Initialization for HT1632C
  {

	// Set pins for display as output
	
	csreg |= (1 << cspin);
    // Set CS line high to disable chip
	csport |= (1 << cspin);
	
	wrreg |= (1 << wrpin);
	dtreg |= (1 << dtpin);
		
	ht1632c_send_command(0x01);  // System Enable
	ht1632c_send_command(0x03);  // LED On 
	ht1632c_send_command(0x18);  // 1632C Master Mode 
	ht1632c_send_command(0x20);  // N-MOS open-drain output and 32 ROW * 8 COM
	ht1632c_send_command(0xA5);  // Set PWM to 1/4 intensity (can be A0 Thru AF)
  }

void ht1632c_chipselect()
  {
    // set CS line low for selected chip
    csport &= ~(1<< cspin);
  }

void ht1632c_chipfree()
  {
    // free selected chip by setting CS line high
    csport |= (1<< cspin);
  }

void ht1632c_send_to_display (uint8_t data_to_send, uint8_t dtlen)
  {
    while (dtlen) {
	  // Set WR pin low
	  wrport &= ~(1 << wrpin);
     if (data_to_send & dtlen) {
      //If bit is high, set data pin high
	  dtport |= (1 << dtpin);
      }
      else {
      // Otherwise, set it low
	  dtport &= ~(1 << dtpin);
      }
      // Clock data into HT1632C by setting write pin high
	  wrport |= (1 << wrpin);
	  // shift to next bit in line	
      dtlen >>= 1;
    }
  }

void ht1632c_send_page ()
  {
  	uint8_t addr;
	ht1632c_chipselect();
	ht1632c_send_to_display(0b101,1<<2);
	ht1632c_send_to_display(0,1<<6);
	for(addr=0;addr<64;addr++)
	{
		ht1632c_send_to_display(ht1632_shadowram[addr],1<<3);
	}
	ht1632c_chipfree();
  }

void ht1632c_send_command (uint8_t command)
  {
    ht1632c_chipselect();  // Select chip
    ht1632c_send_to_display(0b100 ,1<<2);  // Send 3 bits to identify as a command
    ht1632c_send_to_display(command, 1<<7);  // send the command data
    ht1632c_send_to_display(0, 1);     // send a final bit to complete the sequence (doesn't matter if 0 or 1)
    ht1632c_chipfree(); // Release chip
  }

void ht1632c_send_data (uint8_t address, uint8_t data)
  {
    ht1632c_chipselect();  // Select chip
    ht1632c_send_to_display(0b101, 1<<2);  // send 3 bits to identify as data write
    ht1632c_send_to_display(address, 1<<6); // Send memory address
    ht1632c_send_to_display(data, 1<<3); // send nibble of data
    ht1632c_chipfree(); // Release chip
  }

uint8_t ht1632c_get (uint8_t x, uint8_t y)
  {
    uint8_t bitval;
    int addr;

    if ((x < 0) || (y < 0) || (x > (COLS-1)) || (y > (ROWS-1))) {
      return 0;
    }
 
    bitval = 8>>(x&3);  // compute which bit will need set (Will equal 1, 2, 4, or 8)
    addr = ((x>>3)<<4)+(y<<1)+((x&4)>>2);  // compute which memory word this is in
  
    if((ht1632_shadowram[addr] & bitval) != 0) {
      return 1;
    } else {
      return 0;
    }
  }

void ht1632c_set (uint8_t x, uint8_t y, uint8_t val)
  {
    uint8_t bitval;
    int addr;

    if ((x < 0) || (y < 0) || (x > 31) || (y > 7)) {
      return;
    }
    bitval = 8>>(x&3);  // compute which bit will need set (Will equal 1, 2, 4, or 8)
    addr = ((x>>3)<<4)+(y<<1)+((x&4)>>2);  // compute which memory word this is in
  
    if (val) {  // Modify the shadow memory
      ht1632_shadowram[addr] |= bitval;
    }
    else {
      ht1632_shadowram[addr] &= ~bitval;
    }
 
  }
