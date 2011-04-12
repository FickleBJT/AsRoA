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

float c_side;

void IK_solver(unsigned char pos_x, unsigned char pos_y, float *alpha, float *beta)
{	
	c_side = hypot(pos_x/1032.38, pos_y/1032.38); // Should calibrate to size of arm

	if(c_side < 0.247) {
		*alpha = (acos((-0.0065f / c_side) + (pow(c_side, 2) / (0.2413f * c_side))) + atan2((float)pos_y, (float)pos_x)) * RADTODEG; // Shoulder joint
		*beta = acos(1.0013f - (pow(c_side, 2) / 0.0306f)) * RADTODEG; // Elbow joint
	}
}

void IK_solver_threed(unsigned char pos_x, unsigned char pos_y, unsigned char pos_z, float *alpha, float *beta, float *theta)
{
	c_side = hypot((float)pos_x / 1032.38, (float)pos_y / 1032.38);

	if(c_side < 0.247) {
		*alpha = (acos((pow(c_side, 2) - 0.00157f) / (0.2414f * c_side)) + atan2(pos_y, pos_x)) * RADTODEG;
		//*alpha = gamma;
		//*alpha += atan2(pos_y, pos_x) * RADTODEG;
		//*alpha = (acos((0.01456f + (pow(apos_x, 2) + pow(apos_y, 2)) - 0.01613f) / (2.0f*0.01207f*c_side)) + atan2((float)pos_y, (float)pos_x)) * RADTODEG;
		*beta = acos((0.030685f - pow(c_side, 2)) / (0.0306451f)) * RADTODEG;
		*theta = atan2((float)pos_z, (float)pos_x) * RADTODEG;
	}
}
