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

#include "buttons.h"
#include "PWM_gen.h"

#include <avr/io.h>

static unsigned int button_pressed = FALSE;

static unsigned char current = 0xFF;
static unsigned char previous = 0xFF;
static unsigned char saved = 0xFF;

void init_buttons(void) 
{
	DDRD &= 0x80;
	PORTD |= 0x7F;
}

unsigned char check_buttons_click(void) 
{
	current = (PIND | 0x80);
	if((previous != current) && (current != 0xFF)) {
		if(!button_pressed) {
			button_pressed = TRUE;
			saved = current;
		}
		else if(saved != current) {
			saved = current;
		}
		else {
			button_pressed = FALSE;
		}
	}
	previous = current;

	if(button_pressed) {
		return (~saved & 0x7F);
	}
	else {
		return 0x00;
	}
}
