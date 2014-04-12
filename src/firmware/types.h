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

#ifndef TYPES_H_
#define TYPES_H_

//-----------------------------------------------------------------------------
// ADC
#ifdef _SXOSD
#	define HW_ADC_SCALE		2000,2000,500,500
#else
#	define HW_ADC_SCALE		3100,3100,500,500
#endif // _SXOSD
#define HW_ADC_NUMBERS		4
#define HW_ADC_BITS			10

//-----------------------------------------------------------------------------
// VIDEO
#define TIME_FPS_NTSC		60
#define TIME_FPS_PAL		50
#ifndef	_SXOSD
#	define SERVICE_LINES_NTSC	18
#	define SERVICE_LINES_PAL	22
#else
#	define SERVICE_LINES_NTSC	11
#	define SERVICE_LINES_PAL	14
#endif

//-----------------------------------------------------------------------------
// IO
#define HW_KEY_MASKS		(1<<5),(1<<4)
#define HW_LED_MASKS		(1<<6),(1<<3)

//-----------------------------------------------------------------------------
// TEXT
#define TEXT_ROW_MAX_CHARS	35
#define TEXT_CHAR_HEIGHT	8

#define TEXT_ROWS			17
#define ELEMENTS_COUNT		23

#define TEXT_RADAR_SEGM_MAX	5

// CHARS
#define CHAR_BAT1			0x2A
#define CHAR_BAT2			0x2B
#define CHAR_CURRENT		0x3D
#define CHAR_DIST_TRAVELED	0x69
#define CHAR_MAX_DIST		0x6A

#define CHAR_HOME_ARROW		";/-?;/-?"
#define CHAR_GPS_PACKETS	"-?;/"
#define CHAR_RADAR_CENTER	0x21

#define UOSD_FOURCC			MAKEFOURCC('U','O','S','D')
#define UOSD_VERSION		1

enum {
	MODE_NTSC		= 0,
	MODE_PAL,
	MODE_COUNT
};

enum {
	GPS_4800		= 0,
	GPS_9600
};

enum {
	HOME_SET_BUTTON	= 0,
	HOME_SET_GPS_FIX,
	HOME_SET_SPEED
};

enum {
	UNITS_METRIC	= 0,
	UNITS_IMPERIAL
};

enum {
	DEVICE_EOSD		= 0,
	DEVICE_GOSD,
	DEVICE_SXOSD,
	DEVICE_COUNT
};

// ADC
typedef struct {
	u8				high;
	u8				low;
	u16				val100;
} analog_value;

// Time
typedef struct {
	union {
		struct {
			u8	tick;
			u8	sec;
			u8	min;
			u8	blink;
		};		
	};
} time;

// GPS
typedef struct {
	union {
		struct {
			u8		hour;
			u8		min	: 7;
			u8		neg	: 1;
			u16		dmin;
		};
		u32			data;			
	};		
} gps_coord;

typedef struct {
	gps_coord		latitude;
	gps_coord		longitude;
	s16				altitude;
} gps_pos;

typedef struct {	
	gps_pos			pos;
	u16				angle;
	u8				speed;
	u8				sats;
	u8				fix;
	u8				chksum_valid;
} gps_data;

// Text
typedef struct {
	u8				x,y;
}	lt_pos;

typedef struct {
	u8				length		: 6;
	u8				shadowed	: 1;
	u8				visible		: 1;
}	lt_props;

typedef struct {
	lt_pos			p;
	lt_props		props;
}	lt_text;

typedef struct {
	lt_pos			p;
	u16				last_val;
	lt_props		props;
}	lt_scale;

typedef struct {
	lt_pos			c;
	u16				dist[TEXT_RADAR_SEGM_MAX];
	lt_props		props;
}	lt_radar;	

typedef struct {
	lt_pos			c;
	lt_props		props;
}	lt_home_arrow;	

typedef struct {
	u8				input;
	u8				reversed;
	u16				in_min;
	u16				in_max;
	u16				out_min;
	u16				out_max;
}	adc_def;

typedef struct	{
	u32				fourcc;
	u8				version;

	u8				video_mode;		
	u8				units;
	u8				device;
	struct {
		u8			key_mask;
		u8			led_mask;
	} pin;
	u8				fps;
	
	// GPS
	struct {
		u8			baud;			
		s16			corr_lat;	
		s16			corr_lon;	
		u8			home_set;		
		u8			home_set_min_fix;
		u8			home_set_min_sats;
		u8			home_set_min_speed;
		u8			speed_in_knot;
	} gps;		
	
	// ADC's
	adc_def			vol1;
	adc_def			vol2;
	adc_def			rssi;
	adc_def			current;

	// alarm levels
	struct {
		u8			enabled;
		u16			vol1_low;		//		Warn if below this level (in volt)
		u16			vol2_low;		//		Warn if below this level (in volt)
		u8			rssi_low;		// =	Warn if below this level (in %)
		u8			speed_high;		// =	Warn if above this level (in kmh/mph)
		u16			alt_low;		//		Warn if below this level (in meters/feet)
		u16			alt_high;		//		Warn if above this level (in meters/feet)
		u32			dist_high;		//		Warn if below this level (in meters/feet)
	} alarm;
	
	struct {
		u8			rows[TEXT_ROWS];
		u8			rows_offset;
		lt_text		sign;
		lt_text		volt1;
		lt_text		volt2;
		lt_text		timer;
		lt_text		alt;
		lt_text		speed;
		lt_text		lat;
		lt_text		lon;
		lt_text		sat;
		lt_text		bear_to_home;
		lt_text		dist_to_home;
		lt_text		home;
		lt_text		rssi;
	
		lt_text		stat_max_speed;
		lt_text		stat_max_dist;
		lt_text		stat_trip;
		lt_text		stat_max_alt;
	
		lt_text		current;
		lt_text		power_usage;

		lt_scale	speed_scale;
		lt_scale	alt_scale;
		lt_radar	radar;
		lt_home_arrow home_arrow;
	}				layout;
	char			callsign[9];
}	config;	

#endif /* TYPES_H_ */