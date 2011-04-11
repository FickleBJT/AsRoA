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

static unsigned short delay_counter = 0;
static unsigned short adc_count = 0;

unsigned char which_button = 0x00;

unsigned char last_sample_x = 0;
unsigned char last_sample_y = 0;
unsigned char last_sample_z = 0;
float velocity_x = 0;
float velocity_y = 0;
float velocity_z = 0;
float last_vel_x = 0;
float last_vel_y = 0;
float last_vel_z = 0;
float position_x = 0;
float position_y = 0;
float position_z = 0;
float alpha = 0.0;
float beta = 0.0;
float theta = 0.0;
//unsigned char x_in = 59;
//unsigned char y_in = 132;
static unsigned char samples[MAX_CHANNELS] = {0, 0};
static unsigned int sample_num = 0;
unsigned int start = 0;

int main()
{
	init_leds();
	init_adc();
	init_pwm();

	TIMSK |= TIMOVF2 + TIMOVF0 + TIMOVF1; // Interrupts

	sei();

	while(1) {
		; // wait for interrupts
	}
	return 0;
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

ISR(TIMER1_OVF_vect)
{
	;
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
			start = 1;
		}
		else if(which_button == B1) {
			write_leds(samples[1]);
			start = 1;
		}
		else if(which_button == B2) {
			write_leds(samples[2]);
			start = 1;
		}
		else if(which_button == B3) {
			write_leds((unsigned char)beta);
			start = 1;
		}
		else if((which_button == B4) && (start == 1)) {
			reset_velocity(&velocity_x, &last_vel_x);
			reset_velocity(&velocity_y, &last_vel_y);
			start = 0;
			write_leds(0x81);
		}
	}
	else {
		clear_leds();
		start = 1;
	}


	samples[sample_num] = ADCH; // scale and return sample
	sample_num++;
	if(sample_num >= MAX_CHANNELS) {
		sample_num = 0;
		if(start) {
			integrate_and_zero(last_sample_x, samples[0], 10, &velocity_x);
			integrate_and_zero(last_sample_y, samples[1], 10, &velocity_y);
			integrate_and_zero(last_sample_y, samples[2], 10, &velocity_z);
			integrate(last_vel_x, velocity_x, 20, &position_x);
			integrate(last_vel_y, velocity_y, 20, &position_y);
			integrate(last_vel_z, velocity_z, 20, &position_z);
			last_sample_x = samples[0];
			last_sample_y = samples[1];
			last_sample_z = samples[2];
			last_vel_x = velocity_x;
			last_vel_y = velocity_y;
			last_vel_z = velocity_z;
		}

		IK_solver(samples[0], samples[1], &alpha, &beta);

		OCR0 = pwm_scale(&theta, 0);
		OCR1A = pwm_scale(&alpha, 1);
		OCR1B = pwm_scale(&beta, 2);
		OCR2 = pwm_scale((float *)&samples[2], 4);
	}

	adc_set_channel(sample_num);
}
