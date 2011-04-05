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

#include "IK_calc.h"

#include <avr/io.h>
#include <math.h>

static float c_side;

void IK_solver(unsigned char *pos_x, unsigned char *pos_y, float *alpha, float *beta)
{	
	c_side = hypot(*pos_x/1000.0, *pos_y/1000.0);

	*alpha = (acos(-0.0065/c_side + pow(c_side, 2)/(0.241*c_side)) + atan(*pos_y / *pos_x))*RADTODEG; // Shoulder joint
	*beta = (acos(1.0228 - pow(c_side, 2)/0.03)*RADTODEG); // Elbow joint
}
