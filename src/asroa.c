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

float new_area = 0;

//static unsigned char accel_input[100] = {127, 132, 137, 142, 147, 151, 156, 160, 164, 167, 170, 172, 175, 176, 177, 178, 178, 178, 177, 175, 173, 171, 168, 165, 161, 158, 153, 149, 144, 139, 134, 129, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127,	127, 127, 127, 127, 127, 127, 127, 127, 127, 127 ,127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 122, 117, 112, 107, 103, 98, 94, 90, 87, 84, 82, 79, 78, 77, 76, 76, 76, 77, 79, 81, 83, 86, 89, 93, 96, 101, 105, 110, 115, 120, 125, 127, 127, 127, 127};
//static unsigned char accel_input[375] = {127,127,127,125,125,127,127,127,127,127,127,127,127,127,127,127,125,125,125,127,127,127,127,127,127,127,125,125,125,125,127,127,127,127,127,127,125,127,127,125,127,127,127,123,127,125,127,125,125,125,125,127,123,127,125,125,127,123,123,121,117,107,100,92,86,80,74,70,68,70,68,70,74,76,82,84,88,92,96,100,103,105,107,109,113,113,113,117,117,117,115,113,109,107,105,105,100,103,100,98,100,98,98,98,96,96,94,94,94,92,90,92,90,88,88,86,86,86,88,86,86,84,86,86,84,84,82,82,80,80,82,76,80,78,80,78,80,82,82,84,82,84,86,84,86,84,84,88,90,90,92,92,94,94,96,98,100,103,107,109,113,119,121,123,129,133,135,141,145,147,154,158,162,166,170,176,180,184,190,196,202,207,213,219,225,231,235,239,241,247,247,249,251,251,251,251,247,247,247,245,243,243,241,239,237,235,233,227,219,209,198,186,176,166,156,145,139,133,129,125,123,121,119,119,121,121,119,121,123,123,123,123,123,125,127,125,125,127,125,125,125,125,125,127,125,127,125,127,127,125,125,123,127,125,123,127,125,123,123,123,123,123,125,125,125,125,125,123,125,125,127,125,125,123,125,125,125,125,125,125,127,127,125,125,127,125,127,125,125,125,125,127,123,123,125,123,125,123,123,125,125,125,125,125,125,125,127,125,125,125,127,125,125,127,127,123,125,125,125,125,127,125,125,125,127,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,127,125,123,125,125,125,125,125,125,125,123,125,125,125,125,123,125,125,127,125,125,125,127,127,127,123,127,125,125,125,125,127,125,127,127};

static unsigned short delay_counter = 0;
static unsigned int adc_count = 0;
static unsigned int servo_select = 0; // 0 -> Base rotate servo; 1 -> Wrist Inclination servo

unsigned char which_button = 0x00;
unsigned int current_mode = 0;
int x = 0;
unsigned int trash_tracker = 0;
unsigned char last_sample_x = 127;
unsigned char last_sample_y = 127;
unsigned char last_sample_z = 127;
float velocity_x = 0;
float velocity_y = 0;
float velocity_z = 0;
float last_vel_x = 0;
float last_vel_y = 0;
float last_vel_z = 0;
float position_x = 100.0;
static float position_y = 0;
static float position_z = 0;
float alpha = 0.0;
float beta = 0.0;
float echo = 0.0;
float theta = 0.0;
unsigned char samples[MAXCHANNELS] = {0};
static unsigned int sample_num = 0;
unsigned int start_integration = 1;

int main()
{
	init_leds();
	init_adc();
	init_pwm();

	GICR |= 0x20; // External Interrupt 2 (INT2)
	TIMSK |= TIMOVF2 + TIMOVF0 + TIMOVF1; // Interrupts

	DDRB |= 0x80;

	sei();

	while(1) {
		if(servo_select) {
			PORTB |= 0x80;
		}
		else {
			PORTB &= ~0x80;
		}
	;	 // wait for interrupts
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
	if(which_button) {
		if(which_button == B0) {
			if(current_mode < 2) {
				write_leds(samples[4]);
				start_integration = 1;
			}
			else {
				start_integration = 0;
				write_leds(samples[6]);
			}
		}
		else if(which_button == B1) {
			if(current_mode < 2) {
				write_leds(samples[5]);
				start_integration = 1;
			}
			else {
				start_integration = 0;
				write_leds(samples[7]);
			}
		}
		else if(which_button == B2) {
			if(current_mode < 2) {
				write_leds(samples[3]);
				start_integration = 1;
			}
			else {
				start_integration = 0;
				write_leds(samples[4]);
			}
		}
		else if(which_button == B3) {
			write_leds(current_mode);
			reset_velocity(&velocity_x, &last_vel_x);
		}
		else if((which_button == B4) && (start_integration == 1)) {

		}
	}
	else {
		clear_leds();

	}
}

ISR(TIMER2_OVF_vect) // Triggers every 2.04 ms @ 8 MHz
{

	if(!TIMERMODE) {
		if(adc_count < ADCPERIOD) {
			adc_count++;
		}
		else {
			adc_count = 0;
			take_sample();
		}
	}

}

ISR(INT2_vect) // Triggered on PINB2
{
	if(TIMERMODE) {
		if(adc_count < ADCPERIOD) {
			adc_count++;
		}
		else {
			adc_count = 0;
			take_sample();
		}
	}
}

ISR(ADC_vect)
{

	samples[sample_num] = ADCH; 
	sample_num++;
	if(sample_num >= MAXCHANNELS) {
		sample_num = 0;
		current_mode = mode_switch(current_mode, samples[4], samples[5]);
		if(start_integration) {
			if(trash_tracker) {
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
			else {
				trash_tracker = 1;
			}		


			IK_solver_threed(samples[0], samples[1], samples[2], &alpha, &beta, &theta);
			servo_select = 0;
			OCR0 = pwm_scale(alpha, 0);

		}
		
		else if(current_mode >= 2) {
			echo = inclination_solver(samples[7], samples[6]);
			servo_select = 1;
			OCR0 = pwm_scale(echo, 0);


		}

		OCR1A = pwm_scale(alpha, 1);
		OCR1B = pwm_scale(beta, 2);
		OCR2 = (unsigned char)pwm_scale(samples[3], 3);
	}

	adc_set_channel(sample_num);
}
