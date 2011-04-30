#ifndef __INTEGRATION_H
#define __INTEGRATION_H
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

#define SAMPLETOMIL 15.95052
#define MAXPOS 228.8 // mm
#define MAXVEL 114.4 // mm/s


void reset_velocity(float *velocity, float *last_vel);

void reset_position(float *position_x, float *position_y, float *position_z);

float integrate_and_zero(unsigned char y_val_one, unsigned char y_val_two, unsigned int x_ms, float current_area);

float integrate(float y_val_one, float y_val_two, unsigned int x_ms, float current_area);


#endif
