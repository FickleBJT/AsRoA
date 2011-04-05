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

#include "PWM_gen.h"
#include "IK_calc.h"
#include "adc.h"
#include "buttons.h"
#include "extra_macros.h"
#include "LED.h"
#include "integration.h"

#include <avr/io.h>
#include <avr/interrupt.h>

static int delay_counter = 0;
static int adc_count = 0;

unsigned char which_button = 0x00;

unsigned int last_sample = 0;
float last_vel = 0;
float velocity = 0;
float position = 0;
unsigned char alpha = 0;
unsigned char beta = 0;

static unsigned int samples[MAX_CHANNELS] = {0};
static unsigned int sample_num = 0;

int main()
{
//	init_leds();
//	init_adc();
//	init_pwm(); // must later use "enable_pwm(unsigned int)"

//	enable_pwm(3);
//	enable_pwm(0);

	//Interrupts
//	TIMSK |= TIMOVF2 + TIMOVF0;

	sei();

	while(1) {
		integrate_and_zero((unsigned char)104, (unsigned char)60, 20, &velocity);
		position += integrate(last_vel, velocity, 40, &position);
		last_sample = samples[0];
		last_vel = velocity;
		 // wait for interrupts
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
		take_sample();
	}
}

ISR(ADC_vect)
{
	if(which_button) {
		if(which_button == B0) {
			write_leds(samples[0]);
		}
		else if(which_button == B1) {
			write_leds(samples[1]);
		}
		else if(which_button == B2) {
			write_leds((int)velocity);
		}
		else if(which_button == B3) {
			write_leds((int)position);
		}
	}
	else {
		clear_leds();
	}

	//samples[0] = ADCH;
	samples[sample_num] = adc_scale(ADCH, 2); // scale and return sample
	sample_num++;
	if(sample_num >= MAX_CHANNELS) {
		sample_num = 0;
		//	OCR0 = (pwm_scale(samples[1], 1)-32);
		//velocity += integrate(last_sample, samples[0], 10);
		//position += integrate(last_vel, velocity, 20);
		//last_sample = samples[0];
		//last_vel = velocity;
		//IK_solver(samples[0], samples[1], 0, &alpha, &beta);
		//OCR0 = pwm_scale((int)alpha, 1);
		OCR2 = pwm_scale(samples[0], 1);
		OCR0 = pwm_scale((unsigned char)(velocity+128), 1);
	}

	adc_set_channel(sample_num);
}
