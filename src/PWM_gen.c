/********************************************************************
* Copyright 2010, 2011 Ryan Bittner <rbittner@gwmail.gwu.edu>
*
* This file is part of AsRoA.
*
* AsRoA is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* AsRoA is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with AsRoA. If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include <avr/io.h>
#include "include/extra_macros.h"

// Will only work properly with 8-bit uC's

void init_pwm(void)
{
	TCCR2 |= WAVGEN20 + COMPMOD21 + CLKSEL21 + CLKSEL20; // Phase Correct PWM : clk/32
	DDRD |= 0x80; // Set PIND7 as output for OC2
	PORTD |= 0xFF;
	OCR2 = 0xB1; // 0xB1 should create 1500usec pulse
}


unsigned int pos_to_ocr_conv(unsigned int servo_pos)
{
	unsigned int ocr_set;
	ocr_set = servo_pos; // Will not work yet
	return ocr_set;
	
}
