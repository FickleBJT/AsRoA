#ifndef __PRESSURE_H
#define __PRESSURE_H
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

#define PRESSURECUTOFF 16 // sample value cutoff
#define HOLDTIME 200 // The length that pressure must be held to switch modes
#define MINTIME 20

unsigned int mode_switch(unsigned int current_mode, unsigned int glove_press_val, unsigned int bot_press_val);


#endif
