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

void IK_solver(int pos_x, int pos_y, int pos_z, float *alpha, float *beta)
{
	
	float c_side = sqrt(pow(pos_x, 2) + pow(pos_y, 2));

	*alpha = (acos((-0.0065/c_side + pow(c_side, 2)/(0.241*c_side)))*RADTODEG + atan(pos_y/pos_x)); // Shoulder joint
	*beta = (acos(1.0228 - pow(c_side, 2)/0.03)*RADTODEG); // Elbow joint
}
