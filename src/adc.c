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
#include <avr/interrupt.h>
#include "include/adc.h"

void init_adc()
{
	ADCSRA |= ENABLEADC + ADCINTENABLE + ADCPRESCALE2 + ADCPRESCALE1;
	ADMUX |= VREF0 + LADJUST; // Use AVCC as VREF and left adjust result
	DDRA &= 0x00;
}

void take_sample(unsigned int channel_num)
{
	ADCSRA &= ~AUTOTRIG; // We must be sure that only 1 sample will be taken
	ADCSRA |= STARTCONV;
}

void start_freerun(void)
{
	SFIOR &= 0x1F; // Set AUTOTRIG mode to "freerun" 
	ADCSRA |= AUTOTRIG + STARTCONV; // Turn on AUTOTRIG mode
}
