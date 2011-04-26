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
#include "pressure.h"

#include <avr/io.h>
#include <avr/interrupt.h>

static unsigned short delay_counter = 0;
static unsigned short adc_count = 0;

unsigned char which_button = 0x04;

unsigned char last_sample_x = 0;
unsigned char last_sample_y = 0;
unsigned char last_sample_z = 0;
float velocity_x = 0;
float velocity_y = 0;
float velocity_z = 0;
float last_vel_x = 0;
float last_vel_y = 0;
float last_vel_z = 0;
static float position_x = 0.1144;
static float position_y = 0;
static float position_z = 0;
static float alpha = 0.0;
static float beta = 0.0;
static float theta = 0.0;
unsigned char samples[MAXCHANNELS] = {0, 0, 0, 0};
static unsigned int sample_num = 0;
unsigned int start_integration = 0;

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
ISR(TIMER0_OVF_vect) // Triggers every 4.08 ms @ 8 MHz
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

}

ISR(TIMER2_OVF_vect) // Triggers every 4.08 ms @ 8 MHz
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
			write_leds(samples[0] - 3);
			start_integration = 1;
		}
		else if(which_button == B1) {
			//reset_velocity(&velocity_x, &last_vel_x);
			//reset_velocity(&velocity_y, &last_vel_y);
			//reset_velocity(&velocity_z, &last_vel_z);
			write_leds((unsigned char)(velocity_x * SAMPLESTOMETERS));
			//start_integration = 1;
		}
		else if(which_button == B2) {
			write_leds((unsigned char)(position_x * SAMPLESTOMETERS));
			//write_leds(samples[1]);
			start_integration = 1;
		}
		else if(which_button == B3) {
			reset_velocity(&velocity_x, &last_vel_x);
			reset_velocity(&velocity_y, &last_vel_y);
			reset_velocity(&velocity_z, &last_vel_z);
			//write_leds(samples[2]);
			start_integration = 1;

		}
		else if((which_button == B4) && (start_integration == 1)) {
			reset_velocity(&velocity_x, &last_vel_x);
			reset_velocity(&velocity_y, &last_vel_y);
			reset_velocity(&velocity_z, &last_vel_z);
			start_integration = 0;
			write_leds(0x81);
		}
	}
	else {
		clear_leds();
		start_integration = 1;
	}


	samples[sample_num] = ADCH; 
	sample_num++;
	if(sample_num >= MAXCHANNELS) {
		sample_num = 0;
		if(start_integration) {
			velocity_x = integrate_and_zero(last_sample_x, samples[0], INTEGRATIONTIME, velocity_x);
			velocity_y = integrate_and_zero(last_sample_y, samples[1], INTEGRATIONTIME, velocity_y);
			velocity_z = integrate_and_zero(last_sample_z, samples[2], INTEGRATIONTIME, velocity_z);
			position_x = integrate(last_vel_x, velocity_x, INTEGRATIONTIME, position_x);
			position_y = integrate(last_vel_y, velocity_y, INTEGRATIONTIME, position_y);
			position_z = integrate(last_vel_z, velocity_z, INTEGRATIONTIME, position_z);
			last_sample_x = samples[0];
			last_sample_y = samples[1];
			last_sample_z = samples[2];
			last_vel_x = velocity_x;
			last_vel_y = velocity_y;
			last_vel_z = velocity_z;
		}

		IK_solver_threed((position_x * SAMPLESTOMETERS), samples[1], 128, &alpha, &beta, &theta);
	//	OCR0 = pwm_scale(theta, 0);
		OCR1A = pwm_scale(alpha, 1);
		OCR1B = pwm_scale(beta, 2);
	//	OCR2 = (unsigned char)pwm_scale((float)samples[3], 4);
	}

	adc_set_channel(sample_num);
}
