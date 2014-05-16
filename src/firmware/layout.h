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

#ifndef LAYOUT_H_
#define LAYOUT_H_

//0                   1                   2                   3
//0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4
//----------------------------------------------------------------------//
//0 0 0 : 0 0       T 9 9 9 9 m     H ^     M 9 9 9 9 m         A 9 9 % // 0 
//K B D 9 9 9                                                   S 0 3   // 1 
//                                                                      //
//  9 9 9   |                                               |   9 9 9 9 // 2 
//          |                 . . . . . . .                 |           // 3 
//          |               . . . . . . . . .               |           // 4 
//          |             . . . . . . . . . . .             |           // 5 
//          |             . . . . . . . . . . .             |           // 6 
//          |             . . . . \ | / . . . .             |           // 7 
//  9 9 9 > |             . . . . - O - . . . .             | < 9 9 9 9 // 8 
//          |             . . . . / | \ . . . .             |           // 9 
//          |             . . . . . . . . . . .             |           //10 
//          |             . . . . . ^ . . . . .             |           //11 
//          |               . . . . . . . . .               |           //12 
//          |                 . . . . . . .                 |           //13 
//                                                                      // 
//                                                                      //
//P 0 7 . 5 2 V						                    P 0 9 . 0 0 A   //14 
//V 0 7 . 5 2 V                 < 1 7 9 >               P 9 9 9 9 m A h //15 
//N 0 0 . 0 0 . 0 0 0 0         9 9 9 9 m       E 0 0 0 . 0 0 . 0 0 0 0 //16  
//----------------------------------------------------------------------//
//0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4
//0                   1                   2                   3

__attribute__((noinline))										
pstr str_ptr(u8 row, u8 col) {
	return			g_text_buffer[row] + col;
}	

__attribute__((noinline))
void str_shadowed(lt_text *str) {
	if (!str->props.shadowed) return;
	pstr dst		= str_ptr(str->p.y,str->p.x);
	pcstr dst_end	= dst + str->props.length;
	while (dst < dst_end) {
		if (0 != *dst)
			*dst	|= CHAR_SH;
		dst++;
	}		
}

__attribute__((noinline))
void str_clear_tail(pstr ptr, pcstr ptr_end) {
	while (ptr < ptr_end)
		*ptr++		= 0;
}	

__attribute__((noinline))
void str_fill(lt_text *str, char ch) {
	if (!str->props.visible) return;
	pstr dst		= str_ptr(str->p.y,str->p.x);
	pcstr dst_end	= dst + str->props.length;
	while (dst < dst_end)
		*dst++		= ch;
}	

__attribute__((noinline))
pstr print_text(lt_text *str, pcstr src, u8 clear_tail) {
	if (!str->props.visible) return 0; 
	pstr dst		= str_ptr(str->p.y,str->p.x);
	pcstr dst_end	= dst + str->props.length;
	while (*src && dst < dst_end)
		*dst++		= (*src++);
	str_shadowed	(str);
	if (clear_tail)	
		str_clear_tail	(dst,dst_end);
	return			dst;
}

__attribute__((noinline))
void print_float(lt_text *str, u8 high, u8 hwidth, u8 hmin, u8 low, u8 lwidth, u8 lmin, char prefix, char suffix, char delim) {
	if (!str->props.visible) return; 
	pstr dst		= str_ptr(str->p.y,str->p.x);
	pcstr dst_end	= dst + str->props.length;

	if (prefix)	
		*dst++		= prefix;
	dst				= utoa16(high,dst,hwidth,hmin);
	if (delim)
		*dst++		= delim;
	dst				= utoa16(low,dst,lwidth,lmin);
	if (suffix)	
		*dst++		= suffix;

	str_shadowed	(str);
	str_clear_tail	(dst, dst_end);
}	

__attribute__((noinline))
pstr print_decimal(lt_text *str, u16 val, u8 m, char prefix, char suffix) {
	if (!str->props.visible) return 0;
	pstr dst		= str_ptr(str->p.y,str->p.x);
	pcstr dst_end	= dst + str->props.length;

	u8 w			= str->props.length;
	if (prefix) {
		*dst++		= prefix;
		w--;
	}
	if (suffix)		w--;
	dst				= utoa16(val,dst,w,m);
	if (suffix)		*dst++		= suffix;
	
	str_shadowed	(str);
	str_clear_tail	(dst, dst_end);
	return			dst;
}	

__attribute__((noinline))
void print_gps_coord(lt_text *str, gps_coord *val, u8 lat) {
	if (!str->props.visible) return; 
	pstr dst		= str_ptr(str->p.y,str->p.x);

	u8	h			= val->hour;
	u8	m			= val->min; 
	u16	dm			= val->dmin;
	s8	neg			= val->neg;
	
	if (lat) 		*dst++	= neg ? 'S' : 'N';
	else {
		if (h < 100)*dst++	= 0;
		*dst++		= neg ? 'W' : 'E';
	}			
	dst				= utoa16(h,dst,3,2);
	*dst++			= '\'';
	dst				= utoa16(m,dst,2,2);
	*dst++			= '.';
	dst				= utoa16(dm,dst,4,4);
	
	str_shadowed	(str);
}	

__attribute__((noinline))
void print_scale(lt_scale *str, u16 new_val, char c00, char c01, char c1) {
	if (!str->props.visible) return; 
	
	// smoothing
	u16 val			= str->last_val;
	val				= (val + new_val) / 2;
	str->last_val	= val;
	
	pstr ptr		= 0;
	u8 rem			= val - ((val >> 2) << 2);
	u8 a, b;
	if (0 == rem)		{	a = c1;  b = c00;	}
	else if (1 == rem)	{	a = c1;  b = c01;	}
	else if (2 == rem)	{	a = c00; b = c1;	}
	else				{	a = c01; b = c1;	}

	u8 x			= str->p.x;
	u8 y			= str->p.y;
	for (u8 i = 0; i < str->props.length; ++i) { 
		u8 c		= i & 1 ? a : b;
		ptr			= str_ptr(y+i,x);
		*ptr		= c;
	}
}

__attribute__((noinline))
u8 radar_calc_pos(u8 r, u16 angle, s8 *rx, s8 *ry) {
	s8 x			=  ((r * cosT(angle + 90)) / 256);
	s8 y			=  ((r * sinT(angle + 90)) / 256);
	*rx	= x; *ry = y;
	return x * x + y * y;
}

__attribute__((noinline))
u8 course_index(u16 ath) {
	u8 idx			= (ath + 1 + 22) / 45;				// 202 cycles
//	u8 idx			= ((ath + 1 + 22) * 91) >> 12;		// 18 cycles (x / 45)
	if (idx >= 8)	idx	= 0;
	return			idx;
}	

void print_radar(lt_radar *loc, u16 bearing, u16 angle, u16 dist, u8 speed) {
	// radar
	if (!loc->props.visible) return;
	u8 r;
	for (r = 0; r < TEXT_RADAR_SEGM_MAX; r++) {
		if (dist < cfg.layout.radar.dist[r]) 
			break;
	}

	u8 lim			= ((2*r + 1) * (2*r + 1)) >> 2;
	u8 jit			= 0;
	s8				rx,ry;
	do {
		if (radar_calc_pos(r + 1, bearing + jit, &rx, &ry) <= lim)
			break;
		if (radar_calc_pos(r + 1, bearing - jit, &rx, &ry) <= lim)
			break;
	} while (++jit < 8);

	char sh			= loc->props.shadowed ? CHAR_SH : 0;

	u8 cx			= loc->c.x;
	u8 cy			= loc->c.y;

	rx				+= cx;
	ry				+= cy;

	char rs			= (speed > 1) ? course_index(angle) + 0x6E : 0x6C;
		
	u8 lx			= g_radar_last_pos.x;
	u8 ly			= g_radar_last_pos.y;
	pstr ptr		= str_ptr(ly,lx);	*ptr = 0;
	ptr				= str_ptr(cy,cx);	*ptr = CHAR_RADAR_CENTER | sh;
	ptr				= str_ptr(ry,rx);	*ptr = rs | sh;
		
	g_radar_last_pos.x	= rx;
	g_radar_last_pos.y	= ry;
}

void print_home_arrow(lt_home_arrow *loc, u16 ath, u8 speed)
{
	if (!loc->props.visible) return;
	u8 idx			= course_index(ath);
	char sh			= loc->props.shadowed ? CHAR_SH : 0;

	u8 cx			= loc->c.x;
	u8 cy			= loc->c.y;
	u8 lx			= g_home_arrow_last_pos.x;
	u8 ly			= g_home_arrow_last_pos.y;

	lt_pos			segm;
	segm.d			= pgm_read_word(&g_home_arrow_segments[idx]);
	u8 rx			= cx + segm.x;
	u8 ry			= cy + segm.y;

	pstr ptr		= str_ptr(ly,lx);	*ptr = 0;
	ptr				= str_ptr(cy,cx);	*ptr = sh | CHAR_RADAR_CENTER;
	ptr				= str_ptr(ry,rx);	*ptr = sh | (pgm_read_byte(&g_char_home_arrow[idx]));

	g_home_arrow_last_pos.x = rx;
	g_home_arrow_last_pos.y = ry;
}

void print_course(lt_text *course, s16 ath, u8 speed) 
{
	if (!course->props.visible) return;
	char rs			= course_index(ath) + 0x5B;
	if (ath < 180)	print_decimal	(course, ath,		3, ' ',rs);
	else			print_decimal	(course, 360-ath,	3, rs,' ');
}

void print_home(lt_text *home, u8 home_set, u16 ath, u8 b1Hz, u8 speed) {
	if (!home->props.visible) return;
	pstr dst		= str_ptr(home->p.y,home->p.x);
	char sh			= home->props.shadowed ? CHAR_SH : 0;
	if (home_set) {
		*dst++		= sh | ')';
		char rs		= course_index(ath) + 0x5B;
		*dst		= sh | rs;
	} else {
		*dst		= sh | (b1Hz ? '(' : ' ');
	}
}

__attribute__((noinline,unused))
u8 check_alarm(u8 high, u8 low, u8 alarm)
{
	u16 level		= high * 100 + low;
	return			level && (level < alarm); 
}

static void update_layout() {
	u8 blink		= g_time.blink;

#ifndef _SXOSD
	u8 led_mask		= cfg.pin.led_mask;
	if (g_gps_valid_data.fix) {
		if (g_home_set)	PORTD |= led_mask;
		else {
			if (blink)	PORTD |= led_mask;
			else		PORTD &=~led_mask;
		}			
	} else {
		PORTD		^= led_mask;
	}
#endif // _SXOSD

	if (g_home_set) {
		u32			distance;
		calc_home	(&g_gps_valid_data.pos, &g_home_pos, &distance, &g_home_bearing);
		if (distance > g_stat_max_distance) {
			g_stat_max_distance = distance;
		}
		g_home_distance	= distance;
	}

	// smoothed
	s16 alt_rel		= g_gps_valid_data.pos.altitude - g_home_pos.altitude;
	u8 speed		= g_gps_valid_data.speed;
	
	u8 sat_fix		= g_gps_valid_data.fix;
	
	// alarms
	u8 aSpeed		= FALSE;
	u8 aAltitude	= FALSE;
	u8 aDistToHome	= FALSE;
	u8 aBatt1		= FALSE;
	u8 aBatt2		= FALSE;
	u8 aRssi		= FALSE;
	if (cfg.alarm.enabled) {
		aSpeed		= speed > cfg.alarm.speed_high;
		aAltitude	= alt_rel > cfg.alarm.alt_high || alt_rel < cfg.alarm.alt_low;
		aDistToHome	= (g_home_distance > cfg.alarm.dist_high);
		aBatt1		= check_alarm(g_sensor_voltage1.high, g_sensor_voltage1.low, cfg.alarm.vol1_low); // g_sensor_voltage1.val100	&& (g_sensor_voltage1.val100 < cfg.alarm.vol1_low);
		aBatt2		= check_alarm(g_sensor_voltage2.high, g_sensor_voltage2.low, cfg.alarm.vol2_low); // g_sensor_voltage2.val100	&& (g_sensor_voltage2.val100 < cfg.alarm.vol2_low);
		aRssi		= g_sensor_rssi				&& (g_sensor_rssi < cfg.alarm.rssi_low);
	}		
	
	// print values
	print_text		(&cfg.layout.sign,		cfg.callsign, TRUE);
	print_float		(&cfg.layout.timer,		g_time.min, 3,2, g_time.sec, 2,2, 0, 0, ':');

	if (sat_fix) {
		print_gps_coord	(&cfg.layout.lon,	&g_gps_valid_data.pos.longitude,0);
		print_gps_coord	(&cfg.layout.lat,	&g_gps_valid_data.pos.latitude,	1);
	} else {
		str_fill	(&cfg.layout.lon, '-');
		str_fill	(&cfg.layout.lat, '-');
	}		
	if (sat_fix || blink)	print_decimal	(&cfg.layout.sat, g_gps_valid_data.sats,2, '&', pgm_read_byte(&g_char_gps_packets[g_gps_stat_packet]));
	else					str_fill		(&cfg.layout.sat, ' ');

	if (!aBatt1 || blink)	print_float		(&cfg.layout.volt1, g_sensor_voltage1.high, 2, 1, g_sensor_voltage1.low, 2, 2, CHAR_BAT1, 'V', '.');
	else					str_fill		(&cfg.layout.volt1, ' ');
	if (!aBatt2 || blink)	print_float		(&cfg.layout.volt2, g_sensor_voltage2.high, 2, 1, g_sensor_voltage2.low, 2, 2, CHAR_BAT2, 'V', '.');
	else					str_fill		(&cfg.layout.volt2, ' ');
	
#ifdef GPS_DEBUG
	print_decimal	(&cfg.layout.rssi,g_gps_stat_lost_packet, 3, '&', 0);
#else
	if (!aRssi || blink)	print_decimal	(&cfg.layout.rssi,g_sensor_rssi, 2, 0x40, '%');
	else					str_fill		(&cfg.layout.rssi, ' ');
#endif // GPS_DEBUG
	
	print_float		(&cfg.layout.current,	g_sensor_current.high, 3, 1, g_sensor_current.low, 2, 2, CHAR_CURRENT, 'A', '.');	
	pstr str	=	print_decimal			(&cfg.layout.power_usage, g_sensor_power_usage >> 10,	4,	CHAR_CURRENT, 'm');	
	if (str) {
		u8 sh	= cfg.layout.power_usage.props.shadowed ? CHAR_SH : 0;
		*str++	= sh | 'A';
		*str++	= sh | 'h';
	}			
	
	print_decimal	(&cfg.layout.stat_max_speed,g_stat_max_speed,		3, 0, 0);
	print_decimal	(&cfg.layout.stat_max_alt,	g_stat_max_altitude,	4, 0, 0);
	print_decimal	(&cfg.layout.stat_trip,		g_stat_dist_traveled,	4, CHAR_DIST_TRAVELED,g_length_unit);
	print_decimal	(&cfg.layout.stat_max_dist,	g_stat_max_distance,	4, CHAR_MAX_DIST,g_length_unit);
	
	if (!aDistToHome || blink)	print_decimal	(&cfg.layout.dist_to_home, g_home_distance,	4, 0,g_length_unit);
	else						str_fill		(&cfg.layout.dist_to_home, ' ');
	
	if (alt_rel < 0)			alt_rel = 0;
	if (!aAltitude || blink)	print_decimal	(&cfg.layout.alt,	alt_rel, 5,'<',0);
	else						str_fill		(&cfg.layout.alt,	' ');

	if (!aSpeed || blink)		print_decimal	(&cfg.layout.speed,	speed,	 4, 0, '>');
	else						str_fill		(&cfg.layout.speed, ' ');
	
	// draw scales
	print_scale		(&cfg.layout.alt_scale,		alt_rel,0x22,0x23,0x24);
	print_scale		(&cfg.layout.speed_scale,	speed,	0x63,0x64,0x65);
	
	s16 angle_to_home			= (g_home_bearing -	g_gps_valid_data.angle);
	if (angle_to_home < 0)		angle_to_home += 360;
	
	print_home_arrow(&cfg.layout.home_arrow,	angle_to_home, speed);
	print_radar		(&cfg.layout.radar,			g_home_bearing,	g_gps_valid_data.angle, g_home_distance, speed);
	print_course	(&cfg.layout.bear_to_home,	angle_to_home, speed);
	print_home		(&cfg.layout.home,			g_home_set,	angle_to_home, blink, speed);
}

static void draw_line(u8 row, u8 line)
{	
	// 1185 cycles
	if (MODE_NTSC == cfg.video_mode)	LINE_DELAY_NTSC;
	else								LINE_DELAY_PAL;
	
	pcstr str			= g_text_buffer[row];
	u16 line_offs		= (u16)oem6x8 + CHAR_LENGTH * line;

	u8 it				= TEXT_ROW_MAX_CHARS;
	do {
//		DELAY_1_NOP		();
		// 28 cycles
		u8 out;
		u8 sh;
		const u8 ch		= *str++;
		if (ch) {
			u8 of		= ch;
			BIT_CLEAR	(of,CHAR_SH_BIT);
			of			-= CHAR_OFFSET;
			u16 addr	= line_offs + of;
			out			= pgm_read_byte_near(addr);
			sh			= ch & CHAR_SH;
		} else {
			sh			= FALSE;
			out			= 0;
			DELAY_9_NOP	();
		}				
		if (sh)			{	SHADOW_ON;					}
		else			{	SHADOW_OFF;	DELAY_1_NOP();	}
		SPDR			= out;

		DELAY_9_NOP		();
//		DELAY_1_NOP		();
	} while (--it);					
	DELAY_10_NOP		();
	DELAY_7_NOP			();
	SPDR				= 0x00;
	SHADOW_OFF;
}

__attribute__((unused))
static void update_line() {
	_delay_us			(4.5);		// wait 4.5 us to see if H or V sync
#ifndef _SXOSD
	if(!BIT_TEST(PIND, LTRIG_IN)) 
#endif // _SXOSD
	{ // H sync
		// skip first N lines
		u8 line			= g_skip_line;
		if (++line <= cfg.layout.rows_offset) {
			g_skip_line	= line;
			return;
		}			

		u8 rnum			= g_text_row;
		if (rnum < TEXT_ROWS) {
			line		= g_active_line;
			u8 r_line	= cfg.layout.rows[rnum];
			u8 l_end	= r_line + TEXT_CHAR_HEIGHT;
			if (line >= r_line && line < l_end) {
				u8 pmline	= (line - r_line);
				draw_line	(rnum,pmline);
			}
			line++;
			if (line == 255)	line--;
			if (line == l_end){
				rnum++;
				if (rnum == TEXT_ROWS)
					g_frame_sync = cfg.fps;
			}				
			
			g_text_row		= rnum;
			g_active_line	= line;
		}
	} 
#ifndef _SXOSD
	else { // V sync
		g_active_line	= 0;
		g_skip_line		= 0;
		g_text_row		= 0;
	}
#endif // _SXOSD
}

#endif /* LAYOUT_H_ */