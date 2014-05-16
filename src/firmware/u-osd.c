/*u-osd - A simple open source OSD
Copyright (C) 2013 Alex Maximchuk

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include "defines.h"
#include "config.h"

#include <util/delay.h>

#include "types.h"
#include "vars.h"

#include "oem6x8.h"

#include "math.h"
#include "utils.h"
#include "calcs.h"

#include "gps.h"

#include "layout.h"

#include "vars.h"
#include "setup.h"
#include "sensors.h"

static void update_time(u8 fps) {
	++g_time.tick;

	if (g_time.tick >= fps) {
		g_time.tick	= 0;
		g_time.sec++;
	}
	if (g_time.sec >= 60) {
		g_time.sec = 0;
		g_time.min++;
	}
	g_time.blink	= (g_time.tick > (fps / 2));
}

__attribute__((unused))
static void reset_time() {
	g_time.tick		= 0;
	g_time.sec		= 0;
	g_time.min		= 0;
}	

__attribute__((unused))
static void on_frame(u8 fps) {
	if (g_key_pressed) {
		++g_key_press_time;
	}

	update_time		(fps);

	if (g_time.tick % (fps / SENSOR_UPDATE_RATE) == 0) {
		measure_analog	();
		update_sensors	();
	}
	if ((0 == g_time.sec % 2) && g_time.tick == 0) {
		calc_distance_traveled();
	}		
	if (g_time.tick % 2 == 0) {
		update_layout	();
	}

	g_frame_sync		= 0;
}

__attribute__ ((noreturn))
void main(void) {
	setup				();
	while (1) {
		update_gps		();
#ifndef	_SXOSD
		u8 key			= cfg.pin.key_mask;
		u8 led			= cfg.pin.led_mask;
		if((PIND & key) != key) {
			g_key_pressed = 1;
			if (g_key_press_time > cfg.fps) {	// long press
				PORTD	|= led;		
				if (g_gps_valid_data.chksum_valid && g_gps_valid_data.fix) {
					g_key_press_time = 0;
					set_home_pos();
					reset_time	();
				}
			}
		} else {
			if (g_key_pressed) {
				PORTD	&= ~led;	// led off
			}				
			g_key_pressed	= 0;
			g_key_press_time= 0;
		}
#endif // _SXOSD

		if (g_frame_sync) {
			on_frame	(g_frame_sync);
		}
	}
}

#ifdef _SXOSD
// VSYNC interrupt
ISR(INT0_vect)
{
	g_active_line	= 0;
	g_skip_line		= 0;
	g_text_row		= 0;
}
// HSYNC interrupt
ISR(INT1_vect) {
	update_line		();
}
#else
ISR(INT0_vect) {
	update_line		();
	poll_gps		();
}
#endif // _SXOSD
