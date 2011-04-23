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
#include <math.h>

static float slope_sign = 0;
static float y_val_one_signed;
static float y_val_two_signed;
static float proportion;
static char accel_calibration = 3;

void reset_velocity(float *velocity, float *last_velocity)
{
	*velocity = 0;
	*last_velocity = 0;
}

void reset_position(float *position_x, float *position_y, float *position_z)
{

}

void calibrate_accel(unsigned char zero_accel)
{
	accel_calibration = (zero_accel - 128);
}

float integrate_and_zero(unsigned char y_val_one, unsigned char y_val_two, unsigned int x_ms)
{
	float new_area;
	y_val_one_signed = (float)(y_val_one - (128 + accel_calibration));
	y_val_two_signed = (float)(y_val_two - (128 + accel_calibration));
	slope_sign = (y_val_two_signed - y_val_one_signed);
	
	if(slope_sign >= 0) {
		if(y_val_one >= 0x80) {
			new_area = ((0.5f * slope_sign + y_val_one_signed) * ((float)x_ms / 1000.0f));
		}
		else if(y_val_two <= 0x80) {
			new_area = (((-0.5f * slope_sign) + y_val_two_signed) * ((float)x_ms / 1000.0f));
		}
		else { // In this case there are two triangles, one above the origin and one below
			proportion = (y_val_two_signed / (y_val_two_signed - y_val_one_signed) + y_val_one_signed / (y_val_two_signed - y_val_one_signed));
			new_area = (0.5f * slope_sign) * ((float)x_ms / 1000.0f) * proportion;
		}
	}
	else {
		if(y_val_two >= 0x80) {
			new_area = ((-0.5f * slope_sign + y_val_two_signed) * ((float)x_ms / 1000.0f));
		}
		else if(y_val_one <= 0x80) { 
			new_area = ((0.5f * slope_sign + y_val_one_signed) * ((float)x_ms / 1000.0f));
		}
		else { // In this case there are two triangles, one above the origin and one below
			proportion = (y_val_two_signed / (y_val_two_signed - y_val_one_signed) + y_val_one_signed / (y_val_two_signed - y_val_one_signed));
			new_area = ((0.5f * slope_sign) * ((float)x_ms / 1000.0f) * proportion);
		}
	}
	return new_area;
}

float integrate(float y_val_one, float y_val_two, unsigned int x_ms, float current_area)
{
	float new_area;
	slope_sign = (y_val_two - y_val_one);
	if(slope_sign >= 0) {
		if(y_val_one >= 0) {
			new_area = ((0.5f * slope_sign + y_val_one) * ((float)x_ms / 1000.0f));
		}
		else if(y_val_two <= 0) {
			new_area = (((-0.5f * slope_sign) + y_val_two) * ((float)x_ms / 1000.0f));
		}
		else { // In this case there are two triangles, one above the origin and one below
			proportion = (y_val_two / (y_val_two - y_val_one) + y_val_one / (y_val_two - y_val_one));
			new_area = (0.5f * slope_sign) * ((float)x_ms / 1000.0f) * proportion;
		}
	}
	else {
		if(y_val_two >= 0) {
			new_area = ((-0.5f * slope_sign + y_val_two_signed) * ((float)x_ms / 1000.0f));
		}
		else if(y_val_one <= 0) { 
			new_area = ((0.5f * slope_sign + y_val_one_signed) * ((float)x_ms / 1000.0f));
		}
		else { // In this case there are two triangles, one above the origin and one below
			proportion = (y_val_two / (y_val_two - y_val_one) + y_val_one / (y_val_two - y_val_one));
			new_area = ((0.5f * slope_sign) * ((float)x_ms / 1000.0f) * proportion);
		}
	}
	if((current_area + new_area) < 0.2288) { // May not work
		return (current_area + new_area);
	}
	else {
		return current_area;
	}
}

