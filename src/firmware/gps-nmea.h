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

#ifndef GPS_NMEA_H_
#define GPS_NMEA_H_

#define GPS_START_CHAR '$'
#define GPS_SEPARATOR_CHAR ','
#define GPS_CHECKSUM_CHAT '*'
#define GPS_TYPE_NONE 0
#define GPS_TYPE_GPGGA 1
#define GPS_TYPE_GPRMC 2

#define GPS_PART_NONE 0
#define GPS_PART_CHECKSUM 200
#define GPS_PART_FINISHED 255
// GPGGA parts
#define GPS_PART_GPGGA_TIME 1
#define GPS_PART_GPGGA_LAT 2
#define GPS_PART_GPGGA_LAT_UNIT 3
#define GPS_PART_GPGGA_LONG 4
#define GPS_PART_GPGGA_LONG_UNIT 5
#define GPS_PART_GPGGA_FIX 6
#define GPS_PART_GPGGA_SATS 7
#define GPS_PART_GPGGA_DILUTION 8
#define GPS_PART_GPGGA_ALTITUDE 9
#define GPS_PART_GPGGA_ALTITUDE_UNIT 10
#define GPS_PART_GPGGA_GEOID_HEIGHT 11
#define GPS_PART_GPGGA_GEOID_HEIGHT_UNIT 12
// GPRMC parts
#define GPS_GPRMC_PART_OFFSET 100
#define GPS_PART_GPRMC_TIME GPS_GPRMC_PART_OFFSET + 1
#define GPS_PART_GPRMC_STATUS GPS_GPRMC_PART_OFFSET + 2
#define GPS_PART_GPRMC_LAT GPS_GPRMC_PART_OFFSET + 3
#define GPS_PART_GPRMC_LAT_UNIT GPS_GPRMC_PART_OFFSET + 4
#define GPS_PART_GPRMC_LONG GPS_GPRMC_PART_OFFSET + 5
#define GPS_PART_GPRMC_LONG_UNIT GPS_GPRMC_PART_OFFSET + 6
#define GPS_PART_GPRMC_SPEED GPS_GPRMC_PART_OFFSET + 7
#define GPS_PART_GPRMC_ANGLE GPS_GPRMC_PART_OFFSET + 8
#define GPS_PART_GPRMC_DATE GPS_GPRMC_PART_OFFSET + 9
#define GPS_PART_GPRMC_MAG_VARIATION GPS_GPRMC_PART_OFFSET + 10
#define GPS_PART_GPRMC_MAG_VARIATION_UNIT GPS_GPRMC_PART_OFFSET + 11

static char g_gps_text[GPS_MAX_CHARS];
static u8 g_gps_text_pos		= 0;
static u8 g_gps_text_part_step	= GPS_PART_FINISHED; // Try to start on a $
static u8 g_gps_text_type		= GPS_TYPE_NONE;
static u8 g_gps_checksum		= 0;

static void parse_gps_part() {
	if (g_gps_text_type == GPS_TYPE_NONE) {
		if (sz_ncmpb(g_gps_text, "GPGGA", 5)) {
			g_gps_text_type		= GPS_TYPE_GPGGA;
			g_gps_text_part_step= GPS_PART_NONE;
		} else if (sz_ncmpb(g_gps_text, "GPRMC", 5)) {
			g_gps_text_type		= GPS_TYPE_GPRMC;
			g_gps_text_part_step= GPS_GPRMC_PART_OFFSET;
		}
	} else if (g_gps_text_type != GPS_TYPE_NONE) {
		if (g_gps_text_pos != 0) {
			switch (g_gps_text_part_step) {
			case GPS_PART_GPGGA_TIME:
			case GPS_PART_GPRMC_TIME:
				//g_gps_data.time	= parse_int(g_gps_text, GPS_MAX_CHARS);
				break;
			//case GPS_PART_GPGGA_LAT:
			case GPS_PART_GPRMC_LAT:
				parse_gps_coord	(&g_gps_data.pos.latitude, g_gps_text, cfg.gps.corr_lat);
				break;
			//case GPS_PART_GPGGA_LAT_UNIT:
			case GPS_PART_GPRMC_LAT_UNIT:
				if (*g_gps_text == 'S') {
					g_gps_data.pos.latitude.neg = 1;
				}
				break;
			//case GPS_PART_GPGGA_LONG:
			case GPS_PART_GPRMC_LONG:
				parse_gps_coord	(&g_gps_data.pos.longitude, g_gps_text, cfg.gps.corr_lon);
				break;
			//case GPS_PART_GPGGA_LONG_UNIT:
			case GPS_PART_GPRMC_LONG_UNIT:
				if (*g_gps_text == 'W') {
					g_gps_data.pos.longitude.neg = 1;
				}
				break;
			case GPS_PART_GPGGA_FIX:
				g_gps_data.fix		= parse_int(g_gps_text, GPS_MAX_CHARS, 0) ? TRUE : FALSE;
				break;
			case GPS_PART_GPRMC_STATUS:
				// Status
				break;
			case GPS_PART_GPGGA_SATS:
				g_gps_data.sats		= parse_int(g_gps_text, GPS_MAX_CHARS, 0);
				break;
			case GPS_PART_GPGGA_DILUTION:
				// Horizontal dilution of position
				break;
			case GPS_PART_GPGGA_ALTITUDE:
				g_gps_data.pos.altitude = parse_int(g_gps_text, GPS_MAX_CHARS, 0);
				if (cfg.units == UNITS_IMPERIAL) {
					g_gps_data.pos.altitude	= meter_to_feet(g_gps_data.pos.altitude);
				}					
				break;
			case GPS_PART_GPGGA_ALTITUDE_UNIT:
				// Altitude unit TODO!
				break;
			case GPS_PART_GPGGA_GEOID_HEIGHT:
				// If missing altitude wrong?
				break;
			case GPS_PART_GPGGA_GEOID_HEIGHT_UNIT:
				// Geoid unit
				break;
			case GPS_PART_GPRMC_SPEED: {
				u16 speed	= parse_int(g_gps_text, GPS_MAX_CHARS, 2);
				if (cfg.units == UNITS_IMPERIAL) {
					if (cfg.gps.speed_in_knot)	g_gps_data.speed = knot_to_mph(speed) / 100;
					else						g_gps_data.speed = kmph_to_mph(speed) / 100;
				} else {
					if (cfg.gps.speed_in_knot)	g_gps_data.speed = knot_to_kmph(speed) / 100;
					else						g_gps_data.speed = speed / 100;
				}
				} break;
			case GPS_PART_GPRMC_ANGLE:
				g_gps_data.angle = parse_int(g_gps_text, GPS_MAX_CHARS, 0); // Only use int part
				//sz_ncpy(g_text_dbg_ptr,g_gps_text,GPS_MAX_CHARS);
				break;
			case GPS_PART_GPRMC_DATE:
				//g_gps_data.date = parse_int(g_gps_text, GPS_MAX_CHARS);
				break;
			case GPS_PART_CHECKSUM:
				{
					u8 val = parse_hex(g_gps_text, GPS_MAX_CHARS);
					g_gps_data.chksum_valid = (val == g_gps_checksum);
					//if (!g_gps_data.chksum_valid) g_gps_stat_lost_packet++;
				}				
				break;
			}				
		}			
	}
}

static void decode_gps(char data) {
	if (g_gps_text_part_step == GPS_PART_FINISHED && data != '$') {
		return;
	}
	switch (data) {
	case '$':
		g_gps_text_pos			= 0;
		g_gps_text_type			= GPS_TYPE_NONE;
		g_gps_checksum			= 0;
		g_gps_text_part_step	= 0;
		memcpy					(&g_gps_data,&g_gps_valid_data,sizeof(g_gps_data));
		g_gps_data.chksum_valid	= FALSE;
		sz_clear				(g_gps_text,GPS_MAX_CHARS);
		break;
	case ',':
		g_gps_checksum			^= data;
	case '*':
		parse_gps_part			();
		sz_clear				(g_gps_text,GPS_MAX_CHARS);
		g_gps_text_pos			= 0;
		g_gps_text_part_step++;
		if (data =='*') {
			g_gps_text_part_step= GPS_PART_CHECKSUM;
		}			
		break;
	case '\r':
	case '\n':
		parse_gps_part			();
		g_gps_text_part_step	= GPS_PART_FINISHED;
		finish_gps_decoding		();
		break;
	default:
		g_gps_text[g_gps_text_pos]	= data;
		if (g_gps_text_part_step != GPS_PART_CHECKSUM) {
			g_gps_checksum		^= data;
		}
		if (g_gps_text_pos < GPS_MAX_CHARS-1) {
			++g_gps_text_pos;
		}
		break;
	}
}

#endif /* GPS_NMEA_H_ */
