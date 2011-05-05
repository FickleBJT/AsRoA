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
	TCCR0 |= WAVGEN00 + COMPMOD01 + CLKSEL01 + CLKSEL00; // 8-bit Phase Correct PWM : clk/64
	TCCR1A |= WAVGEN11 + WAVGEN10 + COMPMOD1A1 + COMPMOD1B1; //  10-bit Phase Correct PWM : clk/8
	TCCR1B |= CLKSEL11;
	TCCR2 |= WAVGEN20 + COMPMOD21 + CLKSEL21 + CLKSEL20; // 8-bit Phase Correct PWM : clk/32

	DDRB |= 0x08; // Set PINB3 as output for OC0
	DDRD |= 0x80 + 0x20 + 0x10; // Set PIND7 as output for OC2 and PIND5/PIND4 as outputs for OC1A and OC1B

	PORTB |= 0x08; // OC0
	PORTD |= 0x80 + 0x20 + 0x10; // OC2 + OC1A + OC1B

	OCR0 = 0x5C;
	OCR1A = 0x02EC;
	OCR1B = 0x02EC;
	OCR2 = 0x5C;
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

float pwm_scale(float position, unsigned int joint)
{
	switch(joint) {
		case(0): { // Base Rotate or Wrist
			return ((0.6278f * position) + 37.0f);
		}

		case(1): { // Shoulder
			if(position < 135) {
				return ((5.2296f * (float)(135 - (int)position)) + 294.0f); 
			}
			else {
				return 0x03CF;
			}
		}

		case(2): { // Elbow
			if(position < 135) {
				return (5.2296f * (position) + 294.0f); 
			}
			else {
				return 0x03CF;
			}
		}

		case(3): { // Gripper
		return ((1.8f * (float)(255 - (int)position)) - 34.0f);
		//	return ((0.954f * (float)(255 - (int)position)) + 34.0f); 
		}

		default: {
			return position;
		}
	}

}
