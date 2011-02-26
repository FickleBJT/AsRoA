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
#include "include/adc.h"
#include "include/buttons.h"
#include "include/extra_macros.h"
#include "include/LED.h"


static int delay_counter = 0;
static unsigned char which_button = 0x00;
unsigned char ocr_val = 0xC0;
unsigned int samples[MAX_CHANNELS-1];




int main()
{
	init_leds();
	init_adc();
	init_pwm();


	PORTD |= 0x7F;


	//Interrupts
	TIMSK |= TIMOVF2;
	sei();


	while(1) {
		if(which_button == B0) {
			ocr_val = 0xC0;
		}
		else if(which_button == B1) {
			ocr_val = 0xC0;
		}
	}	
}




/*
 * Interrupt Vectors
 */


ISR(TIMER2_OVF_vect)
{
	//OCR2 = ocr_val;
	if(delay_counter < BUTTONCHECKDELAY) {
		delay_counter++;
	}
	else {
		delay_counter = 0;
	which_button = check_buttons();

	}
}

ISR(ADC_vect)
{
	samples[(ADMUX & 0x0F)] = ADCH; // copy sample
	// possibly increment ADC channel here?
}
