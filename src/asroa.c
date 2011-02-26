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
unsigned char port_val = 0x00;
unsigned char ocr_val = 0x80;

int main()
{

	//unsigned int servo_pos[3]; // Does not include gripper

	TCCR2 |= 0x40 + 0x20 + 0x04;
	SREG = 0x80; //general interrupt enable
	OCR2 = 0x0B;
	DDRA = 0x00; // Input to ADC
	DDRB = 0xFF;
	DDRD = 0x80; // Input from Buttons, output for PIND7 (OC2)

	//PORTB = 0xFF;
	PORTD = 0xFF;

	//ADCSRA = 0x80;

	/*
	 * Setting up timers and PWM
	 * Timer input clock is clk/64
	 * Phase-Correct PWM Mode used (8-bit)
	 *
	 */


	// Enable timer 0 compare interrupt
	TIMSK |= 0x80;
	//OCR0 = 0x50;

	//TCCR0 |= 0x2B;
	//TCCR1A |= WGM10 + COM1A1 + COM1A0;
	//TCCR1B |= CS11 + CS10;



	//enable interrupts
	sei();

	while(1) {
		if(which_button == B0) {
			ocr_val = 0xDA;
		}
		else if(which_button == B1) {
			ocr_val = 0xC0;
		}
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
	OCR0 = ocr_val;
	if(delay_counter < BUTTONCHECKDELAY) {
		delay_counter++;
	}
	else {
		delay_counter = 0;
		which_button = check_buttons();
	}
}

ISR(TIMER2_COMP_vect)
{
	OCR2 = ocr_val;
	if(delay_counter < BUTTONCHECKDELAY) {
		delay_counter++;
	}
	else {
		delay_counter = 0;
		which_button = check_buttons();
	}
}
