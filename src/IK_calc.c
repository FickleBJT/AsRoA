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
float d_side;
float apos_x;
float apos_y;
float apos_z;


void IK_solver(unsigned char pos_x, unsigned char pos_y, float *alpha, float *beta)
{	
	apos_x = (float)pos_x / SAMPLESTOMETERS;
	apos_y = (float)pos_y / SAMPLESTOMETERS;
	c_side = hypot(apos_x, apos_y);

	if(c_side < 0.2288) {
		*alpha = (acos((-0.0065f / c_side) + (pow(c_side, 2) / (0.2413f * c_side))) + atan2(apos_y, apos_x)) * RADTODEG; // Shoulder joint
		*beta = acos(1.0013f - (pow(c_side, 2) / 0.0306f)) * RADTODEG; // Elbow joint
	}
}

void IK_solver_threed(unsigned char pos_x, unsigned char pos_y, unsigned char pos_z, float *alpha, float *beta, float *theta)
{
	if(pos_x > 0x4D) {
		apos_x = ((float)pos_x);
	}
	else {
		apos_x = 58.0;
	}
	apos_y = ((float)pos_y);
	apos_z = ((float)pos_z - 128.0);
	c_side = hypot(apos_x, apos_y);
	d_side = hypot(c_side, apos_z );

	if(d_side < 228.8) {
		*alpha = (acos((pow(d_side, 2) - 1572.6f) / (241.3f * d_side)) + (1.570796f - atan2( hypot(apos_x, apos_z), apos_y))) * RADTODEG;
		*beta = acos((30685.4f - pow(d_side, 2)) / (30645.1f)) * RADTODEG;
		*theta = atan2(apos_z, apos_x) * RADTODEG;
	}
}
