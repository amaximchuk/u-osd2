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

#ifndef MATH_H_
#define MATH_H_

const static u8 sinData[91] PROGMEM = {
0,	4,	9,	13,	18,	22,	27,	31,	35,	40,	44,	49,	53,	57,	62,	66,	70,	75,	79,	83,	87,	91,
96,	100,104,108,112,116,120,124,128,131,135,139,143,146,150,153,157,160,164,167,171,174,
177,180,183,186,190,192,195,198,201,204,206,209,211,214,216,219,221,223,225,227,229,231,
233,235,236,238,240,241,243,244,245,246,247,248,249,250,251,252,253,253,254,254,254,255,
255,255,255
};

static s16 sinT(u16 angle) {
	while (angle >= 360)
		angle	-= 360;
	u16 pos		= angle;
	u8 neg		= 0;
	// 180-359 is same as 0-179 but negative.
	if (pos >= 180) { 
		pos		= pos - 180;
		neg		= 1;
	}
	// 0-89 is equal to 90-179 except backwards.
	if (pos >= 90) { 
		pos		= 180 - pos;
	}
	s16 table	= (s16)(pgm_read_byte(&sinData[pos]));
	return neg ? -table : table;
}

static s16 cosT(u16 angle) {
	return sinT(angle + 90);
}

static u8 asinT(s16 val) {
	u8 result	= 0;
	// find first element not before _Val, using operator<
	u8 count	= 90;

	// divide and conquer, find half that contains answer
	for (; 0 < count; ) {	
		u8 count2	= count >> 1;
		u8 mid		= result;
		mid			+= count2;

		if (sinT(mid) < val)
			result	= ++mid, count -= count2 + 1;
		else
			count	= count2;
	}
	return result;
}					
/*
static s16 atan2(s8 y, s8 x) {
	s16 angle;
	if (0 == y) {
		if (x >= 0)	return 0;
		else		return 180;
	}

	if (x==0) {
		if (y>0)	return 90;
		else		return 270;
	}

	angle			= (3667 * x * y);
	if (abs(y)<=abs(x))	angle = angle / (64 * x * x + 17 * y * y);
	else				angle = 90 - angle / (64 * y * y + 17 * x * x);

	if (y<0)		angle = angle + 180;
	return angle;
}
*/
#endif /* MATH_H_ */