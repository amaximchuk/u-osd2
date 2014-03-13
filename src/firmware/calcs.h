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

#ifndef CALCS_H_
#define CALCS_H_

static void calc_home(gps_pos *curr, gps_pos *home, u32 *const distance_result, u16 *const bearing_result) {
	// calculates bearing and distance to reference point
	// vehicle in curr
	// reference point in home

	// returns nothing
	// saves result to distance and bearing pointers

	// presents errors ~ 5m, and 5° for short distances (few km)
	// should present considerable errors for longer distances

	// Ernani Reis, Aug 2011
  	if (curr->latitude.data == home->latitude.data && curr->longitude.data == home->longitude.data) 
	{
		*distance_result = 0;
		if (bearing_result)
			*bearing_result = 0;
		return;
	}

	// cosseno de lat recicla p/ seno de home
	s16	c;
	u32 distance;
	u16 bearing;
	  
	{  
		s16 h_hour		= home->latitude.hour;
		s16 c_hour		= curr->latitude.hour;
		if (home->latitude.neg != curr->latitude.neg)
			c_hour		= -c_hour;
		// c ~ cos(lat) -> long to fix the delta			
		c				= cosT(abs(h_hour + c_hour) >> 1);
	}		

	s32 currLatSec100	= nmea_to_sec100(&curr->latitude);
	s32 homeLatSec100	= nmea_to_sec100(&home->latitude);
	s32 currLongSec100	= nmea_to_sec100(&curr->longitude);
	s32 homeLongSec100	= nmea_to_sec100(&home->longitude);

	s32 deltaLong		= homeLongSec100- currLongSec100;	// Calculate deltas
	s32 deltaLat		= homeLatSec100	- currLatSec100;	// Delta still long with error?

	deltaLong			*= c;	// corrige dla p/ lat da região
	deltaLong			>>= 8;	// cosine is 0-255 so divide with 255

	// Make sure we don't go over uin32_t when doing sqr of deltaLong & deltaLat
	u8 mult		= 0;
	while ((absi32(deltaLong) >= 0xFFFF) || (absi32(deltaLat) >= 0xFFFF)) {
		deltaLong		>>= 1;
		deltaLat		>>= 1;
		mult++;
	}
  
	u32 a				= (deltaLong * deltaLong);
	u32 b				= (deltaLat * deltaLat);

	// Make sure that the squared deltas don't go over uin32_t when adding them together.
	// This should only activate at long distances!
	u8 mult2	= 0;
	while ((a >> 24) + (b >> 24) > 0xFF) {
		a		>>= 2;
		b		>>= 2;
		mult2++;
	}
	
	distance	= fast_sqrt(a + b);
	distance	<<= mult2;

	// Only calculate bearing if bearing pointer is set.
	if (bearing_result) {
		bearing = 0;
		if (distance > 0) { // Over home -> do not compute; Home forced to 0
			if (deltaLat == 0) {
				if (deltaLong >= 0) {		// Home = East
					bearing		= 90; 
				} else {					// Home = West
					bearing		= 270;
				}
			} else {
				if (absi32(deltaLong) >= absi32(deltaLat)) {	// low angle, sine is better
					c = (absi32(deltaLat) << 8) / distance;		// computes sine -> dist > 0
					bearing		= asinT(c);
				} else {										// high angle, cosine is better
					c = (absi32(deltaLong) << 8) / distance;	// Computes cosine
					bearing		= 90 - asinT(c);
				}
				if (deltaLat > 0) {
					if (deltaLong >= 0) {	// NE quadrant
						bearing = 90 - bearing;
					} else {				// NW quadrant
						bearing = 270 + bearing;
					}
				} else {
					if (deltaLong >= 0) {	// SE quadrant
						bearing = 90 + bearing;
					} else {				// SW quadrant
						bearing = 270 - bearing;
					}
				}
			}				
		}
		*bearing_result = bearing;
	}
	// dist * 309 / 1000 = meters
	distance	= distance * 633;	// ate aqui em segundo * 100
	distance	= distance >> 11;	// metros, agora
	distance	<<= mult;
	if (cfg.units == UNITS_IMPERIAL) {
		distance	= meter_to_feet(distance);
		// dist * 3281 / 1000 = miles
		//distance	*= 3360;
		//distance	>>= 10;
	}		
	*distance_result = distance;
}

static void calc_distance_traveled() {
	if (!g_gps_valid_data.fix)	
		return;
		
	if (!g_gps_stat_set) {
		g_gps_stat_pos	= g_gps_valid_data.pos;
		g_gps_stat_set	= TRUE;
	}		

	u32					distance;
	calc_home			(&g_gps_valid_data.pos, &g_gps_stat_pos, &distance, NULL);
	if (distance) {
		g_stat_dist_traveled+= distance;
		g_gps_stat_pos	= g_gps_valid_data.pos;
	}		
}

#endif /* CALCS_H_ */