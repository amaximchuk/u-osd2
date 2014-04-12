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

#ifndef GPS_H_
#define GPS_H_

static void set_home_pos() {
	// reset stats
	g_stat_dist_traveled= 0;
	g_stat_max_speed	= 0;
	g_stat_max_altitude	= 0;
	g_stat_max_distance	= 0;

	g_home_pos			= g_gps_valid_data.pos;
	g_gps_stat_pos		= g_gps_valid_data.pos;

	g_home_set	= 1;
}

static void finish_gps_decoding() {
	if (g_gps_data.chksum_valid) {
		if (++g_gps_stat_packet > 3) 
			g_gps_stat_packet = 0;
		
		g_gps_valid_data	= g_gps_data;
		
		if (g_home_set == 0) {
			if (g_gps_valid_data.fix) {
				if (HOME_SET_GPS_FIX == cfg.gps.home_set) {
					if (g_home_fix_count >= cfg.gps.home_set_min_fix) {
						if (g_gps_valid_data.sats >= cfg.gps.home_set_min_sats) {
							set_home_pos();
						}				  
					} else {
						++g_home_fix_count;
					}
				} else if (HOME_SET_SPEED == cfg.gps.home_set) {
					if (g_gps_valid_data.speed >= cfg.gps.home_set_min_speed) {
						set_home_pos();
					}
				}					
			} else {
				g_home_fix_count = 0;
			}
		} else {
			if (g_gps_valid_data.speed > g_stat_max_speed) {
				g_stat_max_speed = g_gps_valid_data.speed;
			}
			if (g_gps_valid_data.pos.altitude - g_home_pos.altitude > g_stat_max_altitude) {
				g_stat_max_altitude = g_gps_valid_data.pos.altitude - g_home_pos.altitude;
			}
		}
	}
}

#if defined(GPS_DIYD)
#	include "gps-diyd.h"
#else
#	include "gps-nmea.h"
#endif

#endif /* GPS_H_ */