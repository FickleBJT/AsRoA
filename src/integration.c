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

static int slope_sign = 0;
static char y_val_one_signed;
static char y_val_two_signed;
static char accel_calibration;

void reset_velocity(float *velocity, float *last_velocity)
{
	*velocity = 0;
	*last_velocity = 0;
}

void reset_position(float *position, float *last_position)
{

}

void calibrate_accel(unsigned char zero_accel)
{
	accel_calibration = (zero_accel - 128);
}

void integrate_and_zero(unsigned char y_val_one, unsigned char y_val_two, unsigned int x_ms, float *area)
{
	y_val_one_signed = (char)(y_val_one - (128 + accel_calibration));
	y_val_two_signed = (char)(y_val_two - (128 + accel_calibration));
	slope_sign = (y_val_two_signed - y_val_one_signed);
	
	if(slope_sign >= 0) {
		if(y_val_one >= 0x80) {
			*area += (((float)(0.5*slope_sign*x_ms)) + (float)(y_val_one_signed*x_ms))/1000.0;
		}
		else if(y_val_two <= 0x80) {
			*area += (((float)(-0.5*slope_sign*x_ms)) - (float)(y_val_two_signed*x_ms))/1000.0;
		}
		else { // In this case there are two triangles, one above the origin and one below
			*area += ((float)(0.5*slope_sign*x_ms)/1000.0)*(y_val_two_signed/(y_val_two_signed - y_val_one_signed)); // y_val_one is negative
			*area += ((float)(0.5*slope_sign*x_ms)/1000.0)*(y_val_one_signed/(y_val_two_signed - y_val_one_signed)); // y_val_one is negative
		}
	}
	else {
		if(y_val_two >= 0x80) {
			*area += (((float)(-0.5*slope_sign*x_ms) - (float)(y_val_two_signed*x_ms))/1000.0);
		}
		else if(y_val_one <= 0x80) { 
			*area += (((float)(0.5*slope_sign*x_ms) - (float)(y_val_one_signed*x_ms))/1000.0);
		}
		else { // In this case there are two triangles, one above the origin and one below
			*area += ((float)(0.5*slope_sign*x_ms)/1000.0)*(y_val_two_signed/(y_val_two_signed - y_val_one_signed)); // y_val_two is negative
			*area += ((float)(0.5*slope_sign*x_ms)/1000.0)*(y_val_one_signed/(y_val_two_signed - y_val_one_signed)); // y_val_two is negative
		}
	}
}

void integrate(float y_val_one, float y_val_two, unsigned int x_ms, float *area)
{
	slope_sign = ((int)y_val_two - (int)y_val_one);
	if(slope_sign >= 0) {
		if(y_val_one >= 0) {
			*area += (((float)(0.5*slope_sign*x_ms)) + (float)(y_val_one*x_ms))/1000.0;
		}
		else if(y_val_two <= 0) {
			*area += (((float)(-0.5*slope_sign*x_ms)) - (float)(y_val_two*x_ms))/1000.0;
		}
		else { // In this case there are two triangles, one above the origin and one below
			*area += ((float)(0.5*slope_sign*x_ms)/1000.0)*(y_val_two/(y_val_two - y_val_one)); // y_val_one is negative
			*area += ((float)(0.5*slope_sign*x_ms)/1000.0)*(y_val_one/(y_val_two - y_val_one)); // y_val_one is negative
		}
	}
	else {
		if(y_val_two >= 0) {
			*area += (((float)(-0.5*slope_sign*x_ms) - (float)(y_val_two*x_ms))/1000.0);
		}
		else if(y_val_one <= 0) { 
			*area += (((float)(0.5*slope_sign*x_ms) + (float)(y_val_one*x_ms))/1000.0);
		}
		else { // In this case there are two triangles, one above the origin and one below
			*area += ((float)(0.5*slope_sign*x_ms)/1000.0)*(y_val_two/(y_val_two - y_val_one)); // y_val_two is negative
			*area += ((float)(0.5*slope_sign*x_ms)/1000.0)*(y_val_one/(y_val_two - y_val_one)); // y_val_two is negative
		}
	}
}

