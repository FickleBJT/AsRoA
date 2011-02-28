/********************************************************************
* Copyright 2010, 2011 Ryan Bittner <rbittner@gwmail.gwu.edu>
*
* This file is part of AsRoA.
*
* AsRoA is free software: you can redistribute it and/or modify
* it under the terms of the GNU GePORTD 0 18neral Public License as published by
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
#include "include/buttons.h"
#include "include/PWM_gen.h"

static unsigned int button_pressed = FALSE;
static unsigned char current = 0xFF;
static unsigned char previous = 0xFF;


unsigned char check_buttons_hold(void) {
	current = (PIND | 0x80);
	if(previous != current) {
		if(current != 0xFF) {
			button_pressed = TRUE;
		}
		else {
			button_pressed = FALSE;
		}
	}
	previous = current;

	if(button_pressed) {
		return (~current & 0x7F);
	}
	else {
		return 0x00;
	}
}



unsigned char button_to_ocr(unsigned char which_button)
{
		switch(which_button) {
			case(B0): {
				enable_pwm();
				return 0x6B;
			}
			case(B1): {
				enable_pwm();
				return 0x80;
			}
			case(B2): {
				enable_pwm();
				return 0x94;
			}
			case(B3): {
				enable_pwm();
				return 0xA8;
			}
			case(B4): {
				enable_pwm();
				return 0xBB;
			}
			case(B5): {
				enable_pwm();
				return 0xD0;
			}
			case(B6): {
				enable_pwm();
				return 0xE4;
			}
			case(B7): {
				enable_pwm();
				return 0xF8;
			}
			default: {
				break;
			}
		}

		disable_pwm();

		return 0;
}
