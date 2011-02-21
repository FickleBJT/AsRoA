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
#include "include/PWM_gen.h"
#include "include/IK_calc.h"
#include "include/buttons.h"


static int delay_counter = 0;
static unsigned char which_button = 0x00;

int main()
{

	unsigned int servo_pos[3]; // Does not include gripper

	DDRA = 0x00; // Input to ADC
	DDRB = 0xFF;
	DDRC = 0x00; // Input from Buttons

	PORTC = 0xFF;
	sei();


	ADCSRA = 0x80;

	/*
	 * Setting up timers and PWM
	 *
	 * Each TCCRx register has 8 bits
	 * Bits 0-2 -> CSxy (clock select)
	 *	Scaling factor of 64 used
	 * Bit 3    -> WGMx1 (wavegen mode)
	 *	Set to Phase Correct PWM (0)
	 * Bits 4-5 -> COMxy (comp-out mode)
	 *	Set to up-set and down-clear
	 * Bit 6    -> WGMx0 (wavegen mode)
	 *	Set to Phase Correct PWM (1)
	 * Bit 7    -> FOCx *must be '0'*
	 * 'x' is register number (0-
	 * 'y' is bit number
	 */

	TCCR0 |= 0x73; // 2'b01110011
	TCCR1A |= 0x73;
	TCCR1B |= 0x73;
	TCCR2 |= 0x73;


	// Enable timer 0 interrupt
	TIMSK |= 0x01;

	OCR0 = 0x80;

	while(1)
	{
		if(which_button == B0)
			OCR0 = 0xDA;
		else if(which_button == B1)
			OCR0 = 0xC0;
	}	
}


/*
 * Interrupt Vectors
 *
 * TIMER0_COMP_vect:
 * 	 Used to count the delay between
 *   button samples
 *   At an 8MHz clk, this should
 *   trigger approx every 4.07 ms
*/

ISR(TIMER0_COMP_vect)
{
	if(delay_counter < BUTTONCHECKDELAY)
		delay_counter++;
	else
	{
		delay_counter = 0;
		which_button = check_buttons();
	}
}
