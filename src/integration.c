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

void integrate_and_zero(unsigned char y_val_one, unsigned char y_val_two, unsigned int x_ms, float *area)
{
	y_val_one_signed = (char)(y_val_one - 128);
	y_val_two_signed = (char)(y_val_two - 128);
	slope_sign = (y_val_two_signed - y_val_one_signed);
	
	if(slope_sign >= 0) {
		if(y_val_one >= 0x80) {
			*area += (((float)(0.5*slope_sign*x_ms)) + (float)(y_val_one_signed*x_ms))/1000.0;
		}
		else if(y_val_two <= 0x80) {
			*area += (((float)(-0.5*slope_sign*x_ms)) - (float)(y_val_two_signed*x_ms))/1000.0;
		}
		else { // This will cause unnecessary positive error with the current equation
			*area += ((float)(0.5*slope_sign*x_ms)/1000.0);
		}
	}
	else {
		if(y_val_two >= 0x80) {
			*area += (((float)(-0.5*slope_sign*x_ms) - (float)(y_val_two_signed*x_ms))/1000.0);
		}
		else if(y_val_one <= 0x80) { 
			*area += (((float)(0.5*slope_sign*x_ms) - (float)(y_val_one_signed*x_ms))/1000.0);
		}
		else { // This will cause unnecessary negative error with the current equation
			*area += ((float)(-0.5*slope_sign*x_ms)/(1000.0));
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
		else { // This will cause unnecessary positive error with the current equation
			*area += ((float)(0.5*slope_sign*x_ms)/1000.0);
		}
	}
	else {
		if(y_val_two >= 0) {
			*area += (((float)(-0.5*slope_sign*x_ms) - (float)(y_val_two*x_ms))/1000.0);
		}
		else if(y_val_one <= 0) { 
			*area += (((float)(0.5*slope_sign*x_ms) + (float)(y_val_one*x_ms))/1000.0);
		}
		else { // This will cause unnecessary negative error with the current equation
			*area += ((float)(-0.5*slope_sign*x_ms)/(1000.0));
		}
	}
}

