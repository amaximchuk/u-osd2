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

//-----------------------------------------------------------------------------
// Screen and sensor refresh rate
//-----------------------------------------------------------------------------
#define SCREEN_UPDATE_RATE	10
#define SENSOR_UPDATE_RATE	1

// NTSC
#define LINE_DELAY_NTSC		_delay_us(1.3)
// PAL
#define LINE_DELAY_PAL		_delay_us(1.7)
#define FRAME_SYNC_LINE		200

//-----------------------------------------------------------------------------
// HARDWARE
//-----------------------------------------------------------------------------
#define BLACK_OUT			PB1
#define WHITE_OUT			PB3
#define LTRIG_IN			PD2

#define BLACK_MASK			(1<<BLACK_OUT)
#define WHITE_MASK			(1<<WHITE_OUT)

#define LTRIG_MASK			(1<<LTRIG_IN) //INT0
#define SS_MASK				(1<<PB2)

#define OUT1				(1<<PB1)
#define OUT2				(1<<PB3)

#define LTRIG				(1<<PD2) //INT0
#define SS					(1<<PB2)

#endif /* CONFIG_H_ */