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
static int adc_count = 0;
unsigned char which_button = 0x00;
unsigned char ocr_val = 0xB1;
unsigned char samples[MAX_CHANNELS];




int main()
{
	init_leds();
	init_adc();
	init_pwm(); // must later use "enable_pwm()"


	//Interrupts
	TIMSK |= TIMOVF2;
	sei();


	while(1) {
		which_button = check_buttons_hold();
		write_leds(which_button);
		ocr_val = button_to_ocr(which_button);
		OCR2 = ocr_val ? ocr_val : OCR2;
	}	
}




/*
 * Interrupt Vectors
 */


ISR(TIMER2_OVF_vect)
{
	if(delay_counter < BUTTONCHECKDELAY) {
		delay_counter++;
	}
	else {
		delay_counter = 0;
		which_button = check_buttons_hold();
	}
	if(adc_count < ADCPERIOD) {
		adc_count++;
	}
	else {
		adc_count = 0;
		take_sample(0);
	}
}

ISR(ADC_vect)
{
	samples[(ADMUX & 0x07)] = ADCH; // copy sample
	// possibly increment ADC channel here?
	//write_leds(samples[(ADMUX & 0x0F)]);
	//OCR2 = (pos_to_ocr_conv(samples[ADMUX & 0x0F]));
}
