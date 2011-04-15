#ifndef __IK_CALC_H
#define __IK_CALC_H

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

#define RADTODEG 57.2957795f
#define SAMPLESTOMETERS 1032.38866

void IK_solver(unsigned char pos_x, unsigned char pos_y, float *alpha, float *beta);

void IK_solver_threed(unsigned char pos_x, unsigned char pos_y, unsigned char pos_z, float *alpha, float *beta, float *theta);

#endif

