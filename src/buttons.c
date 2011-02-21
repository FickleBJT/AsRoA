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
#include "include/buttons.h"

static unsigned int button_pressed = FALSE;
static unsigned char current = 0xFF;
static unsigned char previous = 0xFF;
static unsigned char which_button = 0x00;


unsigned char check_buttons(void) {
	previous = current;
	if(PINC != 0xFF)
	{
		current = PINC;
			if(previous == 0xFF)
			{
				button_pressed = TRUE;
				determine_which_button();
			}
			else if(previous != current)
				button_pressed = FALSE;
	}
	if(button_pressed)
		return which_button;
	else
		return 0x00;

}


void determine_which_button(void) {
	which_button = ~current;
}
