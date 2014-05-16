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

#ifndef VARS_H_
#define VARS_H_

// Key
static u8				g_key_pressed		= 0;
static u8				g_key_press_time	= 0;

// ADC
static u16				g_adc_raw[HW_ADC_NUMBERS];
static u16				g_adc_scale[HW_ADC_NUMBERS] PROGMEM = {HW_ADC_SCALE};

static analog_value		g_sensor_voltage1;
static analog_value		g_sensor_voltage2;
static u8				g_sensor_rssi		= 0;
static analog_value		g_sensor_current;

static u32				g_sensor_power_usage= 0;

// System
static time				g_time				= {};

// Stats
//static u32				g_stat_dist_accum	= 0;
static u16				g_stat_dist_traveled= 0;
static u8				g_stat_max_speed	= 0;
static s16				g_stat_max_altitude	= 0;
static u16				g_stat_max_distance	= 0;

// GPS
static u8				g_gps_buffer[GPS_BUFFER_SIZE] = {0};
static u8				g_gps_buffer_r		= 0;
static u8				g_gps_buffer_w		= 0;
static gps_data			g_gps_valid_data;
static gps_data			g_gps_data;
static gps_pos			g_gps_stat_pos;

static u8				g_gps_stat_packet	= 0;

static u16				g_home_distance		= 0;	// Distance to home
static u16				g_home_bearing		= 0;	// Direction to home

static gps_pos			g_home_pos; 
static u8				g_home_set			= 0;
static u8				g_home_fix_count	= 0;

// Text & Layout
static volatile u8		g_frame_sync		= 0;
static u8				g_active_line		= 0;
static u8				g_skip_line			= 0;
static char				g_length_unit		= 0;

// rows
static u8				g_text_row;
static char				g_text_buffer[TEXT_ROWS][TEXT_ROW_MAX_CHARS];
static lt_pos			g_radar_last_pos;

static lt_pos			g_home_arrow_last_pos;

static s8				g_home_arrow_segments[8][2] PROGMEM = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
static u16				g_gps_baud[]				PROGMEM = {48,96,192,384,576};
static char				g_char_home_arrow[]			PROGMEM = {";/-?;/-?"};
static char				g_char_gps_packets[]		PROGMEM = {"-?;/"};

#ifdef GPS_DEBUG
	static u8			g_gps_stat_lost_packet= 0;
#endif // GPS_DEBUG

//*
static config			cfg;
/*/
static config			EEMEM cfg = {
	UOSD_FOURCC,
	UOSD_VERSION,

	MODE_NTSC,	
	UNITS_METRIC,		
	{	// gps
		GPS_9600,
		0,		// corr_lat
		0,		// corr_lon
		HOME_SET_GPS_FIX,		// home set 
		10,		// home set min fix
		4,		// home set min sats
		10,		// home set min speed
		1,		// speed in knot
	},
	{	// vol 1
		0,		// ADC input
		0,		// reversed
		0,		// 0
		3100,	// 31
		0,		// 0
		3100,	// 31
	},
	{	// vol 2
		1,		// ADC input
		0,		// reversed
		0,		// 0
		3100,	// 31
		0,		// 0
		3100,	// 31
	},
	{	// rssi
		2,		// ADC input
		0,		// reversed
		150,	// 1.5V
		500,	// 5.V
		0,		// 0%
		100,	// 100%
	},
	{	// current
		2,		// ADC input
		0,		// reversed
		0,		// 0V
		500,	// 5V
		0,		// 0A
		3800,	// 38A
	},
	{
		1,		// enabled
		750,	// vol1_low
		750,	// vol2_low
		50,		// rssi_low	
		100,	// speed_high
		10,		// alt_low	
		1000,	// alt_high
		2000,	// dist_high
	},
	
	{
		{
			0		+ 18,
			12		+ 18,
		
			110-6*8	+ 18,
			110-5*8	+ 18,
			110-4*8 + 18,
			110-3*8 + 18,
			110-2*8 + 18,
			110-1*8 + 18,
			110+0*8 + 18,
			110+1*8 + 18,
			110+2*8 + 18,
			110+3*8 + 18,
			110+4*8 + 18,
			110+5*8 + 18,
		
			191		+ 18,
			203		+ 18,
			215		+ 18,
			225		+ 18,
		},
		0,
		{{0,	1	},	{16,1,1}},							// sign
		{{0,	14	},	{7,	1,1}},							// volt1
		{{0,	15	},	{7,	1,1}},							// volt2
		{{0,	0	},	{6,	1,1}},							// time
		{{30,	8	},	{5,	1,1}},							// alt
		{{1,	8	},	{4,	1,1}},							// speed
		{{0,	18	},	{11,1,1}},							// lat
		{{23,	16	},	{12,1,1}},							// lon
		{{30,	0	},	{4,	1,1}},							// sat
		{{15,	15	},	{5,	1,1}},							// bear_to_home
		{{15,	16	},	{6,	1,1}},							// dist_to_home
		{{17,	14	},	{1,	1,1}},							// home
															
		{{30,	1	},	{5,	1,1}},							// rssi
		{{1,	2	},	{3,	1,1}},							// stat_max_speed
		{{21,	0	},	{7,	1,1}},							// stat_max_dist
		{{8,	0	},	{7,	1,1}},							// stat_trip
		{{31,	2	},	{4,	1,1}},							// stat_max_alt
															
		{{27,	14	},	{7,	1,1}},							// current
		{{27,	15	},	{8,	1,1}},							// power_usage

		{{5,	2	},	0,{0,0,1}},							// speed_scale
		{{29,	2	},	0,{0,0,1}},							// alt_scale
		{{17,	8	},	{17,8}, {10, 50, 100, 200, 500},{0,0,1}},	// radar
	},		
	{
		"CALLSIGN"
	},
};
//*/

#endif /* VARS_H_ */