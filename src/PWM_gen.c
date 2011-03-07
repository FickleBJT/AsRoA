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

#include "extra_macros.h"

#include <avr/io.h>
#include <math.h>

void init_pwm(void)
{
	TCCR0 |= WAVGEN00 + COMPMOD01 + CLKSEL01 + CLKSEL00; // Phase Correct PWM : clk/64
	TCCR2 |= WAVGEN20 + COMPMOD21 + CLKSEL21 + CLKSEL20; // Phase Correct PWM : clk/32

	DDRD |= 0x80; // Set PIND7 as output for OC2
	DDRB |= 0x08; // Set PINB3 as output for OC0

	PORTD |= 0x80;
	PORTB |= 0x08;

	OCR2 = 0xB1; // 0xB1 should create 1500usec pulse @ clk/32
	OCR0 = 0x4C; // 0x9C should create 1500usec pulse @ clk/64
}

void enable_pwm(unsigned int channel) // channels 0-3
{
	switch(channel) {
		case(0): {
			TCCR0 |= COMPMOD01;
		}
		case(1): {
			TCCR1A |= COMPMOD1A1;
		}
		case(2): {
			TCCR1A |= COMPMOD1B1;
		}
		case(3): {
			TCCR2 |= COMPMOD21;
		}
	}
}

void disable_pwm(unsigned int channel) // channel 0-3
{
	switch(channel) {
		case(0): {
			TCCR0 &= ~COMPMOD01;
		}
		case(1): {
			TCCR1A &= ~COMPMOD1A1;
		}
		case(2): {
			TCCR1A &= ~COMPMOD1B1;
		}
		case(3): {
			TCCR2 &= ~COMPMOD21;
		}
	}
}


/**********************************************
 * joint can be a value from 0 to 4
 * 0 - Base rotate
 * 1 - Shoulder
 * 2 - Elbow
 * 3 - Wrist
 * 4 - Gripper
 *********************************************/

unsigned char pwm_scale(unsigned char position, unsigned int joint)
{
	switch(joint) {
		case(0): {
			return (0.54*(position+35));
		}
		case(1): {
			if(position >= 0xF8) {
				return 0xF8;
			}
			else if(position <= 0x08) {
				return 0x08;
			}
			else {
				return position;
			}
		}
		default: {
			return 0;
		}
	}

}
