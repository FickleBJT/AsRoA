#ifndef __ADC_H
#define __ADC_H

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

#define TIMERMODE 1 // 1 -> External timer used for ADC; 0 -> Internal timer used for ADC
#define MAXCHANNELS 8
#define ADCPERIOD 1
#define TIMERTWOPERIOD 2.04
#define EXTERNALTIMERPERIOD 0.1

#if TIMERMODE
	#define INTEGRATIONTIME (MAXCHANNELS * ADCPERIOD * EXTERNALTIMERPERIOD)
#else
	#define INTEGRATIONTIME (MAXCHANNELS * ADCPERIOD * TIMERTWOPERIOD)
#endif

#define ENABLEADC    0x80
#define STARTCONV    0x40
#define AUTOTRIG     0x20
#define ADCINTFLAG   0x10
#define ADCINTENABLE 0x08
#define ADCPRESCALE2 0x04
#define ADCPRESCALE1 0x02
#define ADCPRESCALE0 0x01

#define VREF1        0x80
#define VREF0		 0x40
#define LADJUST      0x20
#define MUX04		 0x10
#define MUX03		 0x08
#define MUX02		 0x04
#define MUX01		 0x02
#define MUX00		 0x01

void init_adc(void);

void take_sample(void);

void start_freerun(void);

void adc_set_channel(unsigned int channel_num);

#endif
