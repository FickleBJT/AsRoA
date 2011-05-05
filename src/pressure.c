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

#include "pressure.h"

#include <avr/io.h>

static unsigned int hold_count = 0;

unsigned int mode_switch(unsigned int current_mode, unsigned int glove_press_val, unsigned int bot_press_val)
{
	switch(current_mode) {
		case(0): { // follow mode, no inclination measurement
			if(glove_press_val > PRESSURECUTOFF) {
				hold_count++;
				return 0;
			}
			else if((hold_count < HOLDTIME) && (hold_count > MINTIME)) {
				hold_count = 0;
				return 2;
			}
			else if(hold_count > HOLDTIME) {
				hold_count = 0;
				return 1;
			}
			else {
				hold_count = 0;
				return 0;
			}
		}

		case(1): { // mirror mode, no inclination measurement
			if(glove_press_val > PRESSURECUTOFF) {
				hold_count++;
				return 1;
			}
			else if(hold_count < HOLDTIME) {
				hold_count = 0;
				return 3;
			}
			else {
				hold_count = 0;
				return 0;
			}
		}

		case(2): { // inclination measurement, will continue with follow mode
			if(bot_press_val > PRESSURECUTOFF) {
				hold_count++;
				return 2;
			}
			else if(hold_count < HOLDTIME) {
				hold_count = 0;
				return 2;
			}
			else {
				hold_count = 0;
				return 0;
			}
		}

		case(3): { // inclination measurement, will continue with mirror mode
			if(bot_press_val > PRESSURECUTOFF) {
				hold_count++;
				return 3;
			}
			else if(hold_count < HOLDTIME) {
				hold_count = 0;
				return 3;
			}
			else {
				hold_count = 0;
				return 1;
			}
		}

		default: {
			return 0;
		}
	}
}
