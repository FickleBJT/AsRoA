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
static unsigned char samples[MAX_CHANNELS] = {0};
static unsigned char sample_num = 0;




int main()
{
	//init_leds();
	init_adc();
	init_pwm(); // must later use "enable_pwm(unsigned int)"


	//Interrupts
	TIMSK |= TIMOVF2 + TIMOVF0;
	sei();


	while(1) {
		;
	}	
}




/*
 * Interrupt Vectors
 */


ISR(TIMER0_OVF_vect)
{
	if(delay_counter < BUTTONCHECKDELAY) {
		delay_counter++;
	}
	else {
		delay_counter = 0;
		which_button = check_buttons_click();
	}
}

ISR(TIMER2_OVF_vect)
{
	if(adc_count < ADCPERIOD) {
		adc_count++;
	}
	else {
		adc_count = 0;
		take_sample(sample_num);
	}
}

ISR(ADC_vect)
{
	samples[sample_num] = adc_scale(ADCH, sample_num); // scale and copy sample
	write_leds(samples[((which_button & 0x01)-1)]);
	if(sample_num == (MAX_CHANNELS - 1)) {
		sample_num = 0;
		OCR0 = pwm_scale(samples[0]);
		OCR2 = pwm_scale(samples[1]);
	}
	else {
		sample_num = sample_num + 1;
	}
}
