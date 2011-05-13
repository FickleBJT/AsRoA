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

#include "integration.h"

#include <avr/io.h>
#include <math.h>

static float slope_sign = 0;
static float y_val_one_signed;
static float y_val_two_signed;
static float proportion;
static int accel_calibration = -1;

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
	accel_calibration = ((int)zero_accel - 128);
}

float integrate_and_zero(unsigned char y_val_one, unsigned char y_val_two, float x_ms, float current_area)
{
	float new_area = 0;
	y_val_one_signed = (float)(((int)y_val_one - (128 + accel_calibration))) * SAMPLETOMIL;
	y_val_two_signed = (float)(((int)y_val_two - (128 + accel_calibration))) * SAMPLETOMIL;
	slope_sign = ((float)((y_val_two - (128 + accel_calibration)) - (y_val_one - (128 + accel_calibration))) * SAMPLETOMIL);
	
	if(slope_sign >= 0) {
		if((y_val_one + accel_calibration) >= 0x82) {
			new_area = ((0.5f * slope_sign + y_val_one_signed) * ((float)x_ms / 1000.0f));
		}
		else if((y_val_two + accel_calibration) <= 0x7E) {
			new_area = (((-0.5f * slope_sign) + y_val_two_signed) * ((float)x_ms / 1000.0f));
		}
		else if(((y_val_two + accel_calibration) > 0x80) && (y_val_one + accel_calibration < 0x80))  { // In this case there are two triangles, one above the origin and one below
			proportion = (y_val_two_signed / (y_val_two_signed - y_val_one_signed) + y_val_one_signed / (y_val_two_signed - y_val_one_signed));
			new_area = (0.5f * slope_sign) * ((float)x_ms / 1000.0f) * proportion;
		}
		else {
			new_area = 0;
		}
	}
	else {
		if((y_val_two + accel_calibration) >= 0x82) {
			new_area = ((-0.5f * slope_sign + y_val_two_signed) * ((float)x_ms / 1000.0f));
		}
		else if((y_val_one + accel_calibration) <= 0x7E) { 
			new_area = ((0.5f * slope_sign + y_val_one_signed) * ((float)x_ms / 1000.0f));
		}
		else if(((y_val_two + accel_calibration) < 0x80) && (y_val_one > 0x80)) { // In this case there are two triangles, one above the origin and one below
			proportion = (y_val_two_signed / (y_val_two_signed - y_val_one_signed) + y_val_one_signed / (y_val_two_signed - y_val_one_signed));
			new_area = ((0.5f * slope_sign) * ((float)x_ms / 1000.0f) * proportion);
		}
		else {
			new_area = 0;
		}
	}
	if(fabs(current_area + new_area) < MAXVEL) {
		return (current_area + new_area);
	}
	else {
		return current_area;
	}
}

float integrate(float y_val_one, float y_val_two, float x_ms, float current_area)
{
	float new_area;
	slope_sign = (y_val_two - y_val_one);
	if(slope_sign >= 0) {
		if(y_val_one >= 2) {
			new_area = ((0.5f * (y_val_one + y_val_two)) * ((float)x_ms / 1000.0f));
		}
		else if(y_val_two <= -2) {
			new_area = ((0.5f * (y_val_one + y_val_two)) * ((float)x_ms / 1000.0f));
		}
		else if((y_val_one > 0) && (y_val_two < 0)) { // In this case there are two triangles, one above the origin and one below
			proportion = (y_val_two / (y_val_two - y_val_one) + y_val_one / (y_val_two - y_val_one));
			new_area = (0.5f * slope_sign) * ((float)x_ms / 1000.0f) * proportion;
		}
		else {
			new_area = 0;
		}
	}
	else {
		if(y_val_two >= 2) {
			new_area = ((0.5f * (y_val_one + y_val_two)) * ((float)x_ms / 1000.0f));
		}
		else if(y_val_one <= -2) { 
			new_area = ((0.5f * (y_val_one + y_val_two)) * ((float)x_ms / 1000.0f));
		}
		else if((y_val_one < 0) && (y_val_two > 0)) { // In this case there are two triangles, one above the origin and one below
			proportion = (y_val_two / (y_val_two - y_val_one) + y_val_one / (y_val_two - y_val_one));
			new_area = ((0.5f * slope_sign) * ((float)x_ms / 1000.0f) * proportion);
		}
		else {
			new_area = 0;
		}
	}
	if(current_area + new_area < MAXPOS && (current_area + new_area) > 0.01) {
		return (current_area + new_area);
	}
	else {
		return current_area;
	}
}

