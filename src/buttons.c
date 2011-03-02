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
static unsigned char saved = 0xFF;


void init_buttons(void) {
	DDRD &= 0x80;
	PORTD |= 0x7F;
}

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

unsigned char check_buttons_click(void) {
	current = (PIND | 0x80);
	if((previous != current) && (current != 0xFF)) {
		if(!button_pressed) {
			button_pressed = TRUE;
			saved = current;
		}
		else {
			button_pressed = FALSE;
		}
	}
	if(button_pressed) {
		return (~saved & 0x7F);
	}
	else {
		return 0x00;
	}
}
		
			

unsigned char button_to_ocr(unsigned char which_button)
{
		switch(which_button) {
			case(B0): {
				return 0x40;
			}
			case(B1): {
				return 0x48;
			}
			case(B2): {
				return 0x50;
			}
			case(B3): {
				return 0x58;
			}
			case(B4): {
				return 0x60;
			}
			case(B5): {
				return 0x68;
			}
			case(B6): {
				return 0x70;
			}
			case(B7): {
				return 0x78;
			}
			default: {
				break;
			}
		}
		return 0;
}
