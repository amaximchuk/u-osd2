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

#ifndef CONFIG_H_
#define CONFIG_H_

//-----------------------------------------------------------------------------
// CPU speed
//-----------------------------------------------------------------------------
#define F_CPU 24000000UL

//-----------------------------------------------------------------------------
// GPS
//-----------------------------------------------------------------------------
#define GPS_MAX_CHARS		16
#define GPS_BUFFER_SIZE		28
//#define GPS_DEBUG

//-----------------------------------------------------------------------------
// Sensor refresh rate
//-----------------------------------------------------------------------------
#define SENSOR_UPDATE_RATE	2

// Line delay
#define LINE_DELAY_NTSC		_delay_us(1.3)
#define LINE_DELAY_PAL		_delay_us(1.7)

//-----------------------------------------------------------------------------
// HARDWARE
//-----------------------------------------------------------------------------
#ifdef _SXOSD
#	define SHADOW_ON		BIT_SET(DDRD, PD7);
#	define SHADOW_OFF		BIT_CLEAR(DDRD, PD7);
#	define LTRIG_IN			PD3	//IRQ: INT0
#else
#	define SHADOW_ON		BIT_SET(DDRB, PB1);
#	define SHADOW_OFF		BIT_CLEAR(DDRB, PB1);
#	define LTRIG_IN			PD2	//IRQ: INT0
#endif

#define WHITE_OUT			PB3
#define SS_OUT				PB2

#endif /* CONFIG_H_ */