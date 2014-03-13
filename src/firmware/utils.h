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

#ifndef UTILS_H_
#define UTILS_H_

#define BIT_SET(reg,bit) reg|=(1<<bit)
#define BIT_CLEAR(reg,bit) reg&=~(1<<bit)
#define BIT_TEST(reg,bit) (reg&(1<<bit))

#define DELAY_1_NOP() asm("nop\r\n")
#define DELAY_2_NOP() asm("nop\r\nnop\r\n")
#define DELAY_3_NOP() asm("nop\r\nnop\r\nnop\r\n")
#define DELAY_4_NOP() asm("nop\r\nnop\r\nnop\r\nnop\r\n")
#define DELAY_5_NOP() asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\n")
#define DELAY_6_NOP() asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n")
#define DELAY_7_NOP() asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n")
#define DELAY_8_NOP() asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n")
#define DELAY_9_NOP() asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n")
#define DELAY_10_NOP() asm("nop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\nnop\r\n")

// abs seems to only support 16 bits so I had to make my own.
static u32 absi32(s32 a) {
	return (a > 0) ? a : -a;
}

// http://www.strchr.com/
//u32 abs32(s32 a) {
//	s32 mask = (a >> 31);
//	return (a + mask) ^ mask;
//}

// http://chipenable.ru/index.php/programming-avr/item/144-sqrt-root.html#comment-2412
// -93% (14 times)
static u32 fast_sqrt(u32 x)
{
	u32 b;
	u32 m	= 0x40000000; // for u32
	u32 y	= 0;
	while (m){
		b	= y | m;
		y	= y >> 1;
		if (x >= b) {
			x = x - b;
			y = y | m;
		}
		m	>>= 2; 
	}
	return y;
}

static inline u8 is_digit(char c) {
	if (c >= '0' && c <= '9') {
		return TRUE;
	}
	return FALSE;
}

__attribute__((unused))
static pstr sz_cpy(pstr dst, pcstr src) 
{
	while (src) *dst++ = *src++;
	return dst;
}	

__attribute__((unused))
static pstr sz_ncpy(pstr dst, pcstr src, u8 len) {
	memcpy(dst, src, len * sizeof(char));
	return dst + len;
}	

__attribute__((unused))
static u8 sz_len(const char *s) {
    const char *start = s;
    while(*s)	s++;
    return s - start;
}

__attribute__((noinline,unused))
static void sz_clear(pstr dst, u8 len) {
	memset(dst,0,len);
}	

__attribute__((noinline,unused))
static u8 sz_ncmpb(pcstr dst, pcstr src, u8 len) {
	pcstr end = dst + len;
	while (dst < end)
		if (*dst++ != *src++) return FALSE;
	return TRUE;
}	


// Binary to Decimal Conversion in Limited Precision
// http://homepage.cs.uiowa.edu/~jones/bcd/decimal.html

// -32089..32089
__attribute__((unused))
static char* itoa16_lim(s16 n, char *s, u8 w)
{
    u8 d4, d3, d2, d1, d0, q;
	char *out	= s;

    if (n < 0) {
        *out++	= '-';
        n		= -n;
    }

    d1 = (n>>4)  & 0xF;
    d2 = (n>>8)  & 0xF;
    d3 = (n>>12) & 0xF;

    d0 = 6*(d3 + d2 + d1) + (n & 0xF);
    q = (d0 * 0xCD) >> 11;
    d0 = d0 - 10*q;

    d1 = q + 9*d3 + 5*d2 + d1;
    q = (d1 * 0xCD) >> 11;
    d1 = d1 - 10*q;

    d2 = q + 2*d2;
    q = (d2 * 0x1A) >> 8;
    d2 = d2 - 10*q;

    d3 = q + 4*d3;
    d4 = (d3 * 0x1A) >> 8;
    d3 = d3 - 10*d4;

    if (w>4 || d4)	*out++	= d4 + '0';
    if (w>3 || d3)	*out++	= d3 + '0';
    if (w>2 || d2)	*out++	= d2 + '0';
    if (w>1 || d1)	*out++	= d1 + '0';
    if (w>0 || d0)	*out++	= d0 + '0';
	*out		= 0;
	return		out;
}

static char* utoa16( u16 n, char *s, u8 w, u8 m)
{
    u8 d3, d2, d1, d0, q;
	char *out		= s;
	if (m > w) m	= w;

    d0	= n       &	0x1F;
    d1	= (n>>5)  &	0xF;
    d2	= (n>>9)  &	0xF;
    d3	= (n>>13) &	0x7;

    d0	= 2 * (d3 + d2 + d1) + d0;
    q	= (d0 * (u16)0x67) >> 10;
    d0	= d0 - 10 * q;

    d1	= 9 * d3 + d2 + 3 * d1 + q;
    q	= (d1 * (u16)0x67) >> 10;
    d1	= d1 - 10 * q;

    d2	= d3 + 5 * d2 + q;
    q	= (d2 * (u16)0x67) >> 10;
    d2	= d2 - 10 * q;

    d3	= 8 * d3 + q;
    q	= (d3 * (u16)0x1A) >> 8;
    d3	= d3 - 10 * q;

    if (w && (m>4 || q))	{ *out++ = q  + '0'; --w;}
    if (w && (m>3 || d3))	{ *out++ = d3 + '0'; --w;}
    if (w && (m>2 || d2))	{ *out++ = d2 + '0'; --w;}
    if (w && (m>1 || d1))	{ *out++ = d1 + '0'; --w;}
    if (w && (m>0 || d0))	{ *out++ = d0 + '0'; }
//	*out		= 0;
	return		out;
}

__attribute__((unused))
static char* utoa8( u8 n, char *s, u8 w, u8 m)
{
    u8 d1, d0, q;
	char *out		= s;
	if (m > w) m	= w;

    d0	= n       &	0x1F;
    d1	= (n>>5)  &	0xF;

    d0	= 2 * d1 + d0;
    q	= (d0 * (u16)0x67) >> 10;
    d0	= d0 - 10*q;

    d1	= 3 * d1 + q;
    q	= (d1 * (u16)0x67) >> 10;
    d1	= d1 - 10*q;

    if (w && (m>2 || q))	{ *out++ = q + '0';	--w;}
    if (w && (m>1 || d1))	{ *out++ = d1 + '0';--w;}
    if (w && (m>0 || d0))	{ *out++ = d0 + '0';}
//	*out		= 0;
	return		out;
}

// Reciprocal Multiplication
// http://homepage.cs.uiowa.edu/~jones/bcd/divide.html

__attribute__((unused))
static u32 div6_u32(u32 A) {
	// either Q = A/6 or Q+1 = A/6 for all 32-bit unsigned A
	u32 Q;					// approximate A/6
	Q = (A >>  3)			;
	Q = (A >>  1) + Q		;	// Q = A*0.101
	Q = ((Q >>  4) + Q)     ;	// Q = A*0.1010101
	Q = ((Q >>  8) + Q)		;	// Q = A*0.101010101010101
	Q = ((Q >> 16) + Q) >> 2;	// Q = A*0.001010101010101010101010101...
	return Q;
}								

__attribute__((unused))
static u16 div3_u16(u16 A, u8 *r) {
	// approximate A/3
	// either Q = A/3 or Q+1 = A/3 for all A < 792,771
	u16 Q;
	Q = ((A >>  2) + A) >> 2;	// Q = A*0.0101
	Q = ((Q      ) + A) >> 1;	// Q = A*0.10101
	Q = ((Q >>  6) + Q)     ;	// Q = A*0.10101010101
	Q = ((Q >> 12) + Q) >> 1;	// Q = A*0.01010101010101010...
	// correction
	u8 R = ((Q << 1) + Q);
	R = A - R;					// R = A - 6*Q
	if (R >= 3) {
		R	-= 3;
		++Q;
	}
	*r	= R;
	return Q;
}

__attribute__((unused))
static u16 div6_u16(u16 A) {
	// either Q = A/6 or Q+1 = A/6 for all A < 222,438
	u16 Q;						// approximate A/6
	Q = (A >>  3)			;
	Q = (A >>  1) + Q		;	// Q = A*0.101
	Q = (Q >>  4) + Q		;	// Q = A*0.1010101
	Q = ((Q >> 8) + Q) >> 2	;	// Q = A*0.00101010101010101
	// correction
	u16 R = ((Q << 2) + (Q << 1));
	R = A - R;					// R = A - 6*Q
	if (R >= 6) {
//		R = R - 6;
		Q = Q + 1;
	}
	return Q;
}								

__attribute__((unused))
static u16 div10_u16(u16 A) {
	// either Q = A/10 or Q+1 = A/10 for all A < 534,890
	u16 Q;						// approximate A/10
	Q = (A >> 2)			;
	Q = (A >> 1)  + Q		;	// Q = A*0.11
	Q = ((Q >> 4) + Q)		;	// Q = A*0.110011
	Q = ((Q >> 8) + Q) >> 3	;	// Q = A*0.00011001100110011
	// correction
	u16 R;
	R = ((Q << 2) + Q) << 1	;
	R = A - R;					// R = A - 10*Q
	if (R >= 10) {
//		R = R - 10;
		Q = Q + 1;
	}
	return Q;
}	

__attribute__((unused))
static u32 div10_u32(u32 A) {
    // either Q = A/10 or Q+1 = A/10 for all 32-bit unsigned A
	u32 Q;						// the quotient
	Q = (A >> 2)			;
	Q = (A >> 1)  + Q		;	// Q = A*0.11
	Q = ((Q >>  4) + Q)     ;	// Q = A*0.110011
	Q = ((Q >>  8) + Q)     ;	// Q = A*0.11001100110011
	Q = ((Q >> 16) + Q) >> 3;	// Q = A*0.000110011001100110011001100110011
	// correction
	u16 R;
	R = ((Q << 2) + Q) << 1	;
	R = A - R;					// R = A - 10*Q
	if (R >= 10) {
//		R = R - 10;
		Q = Q + 1;
	}
	return Q;
}	

__attribute__((noinline,unused))
static u8 div_bf(u16 val, u8 divider) { // brute force
	u8 res		= 0;
	while (val >= divider) {
		val		-= divider;
		++res;
	}
	return		res;
}	

__attribute__((unused))
static s16 parse_int(pcstr text, u8 maxLength, u8 frac) {
	pcstr ptr		= text;
	pcstr ptr_end	= text + maxLength;
	s16 val			= 0;
	u8 neg			= 0;
	if (*ptr	== '-') {
		neg			= 1;
		ptr++;
	}
	while (ptr != ptr_end && is_digit(*ptr)) {
		val			*= 10;
		val			+= *ptr++ - '0';
	}
	
	if (frac) {
		if (*ptr == '.') ptr++;
		while (frac--) {
			val		*= 10;
			if (is_digit(*ptr)) {
				val	+= *ptr++ - '0';
			}
		}
	}		
	return neg ? -val : val;
}
/*	
static s32 parseFloat(const char* const text, u8 maxLength) {
	u32 val	= 0;
	u8 neg		= 0;
	u8 i		= 0;
	if (text[0] == '-') {
		++i;
		neg			= 1;
	}
	for (; i < maxLength; ++i) {
		if (is_digit(text[i])) {
			val		*= 10;
			val		+= (text[i]) - '0';
		} else if (text[i] != '.') {
			break;
		}			
	}
	return val * (neg ? -1 : 1);
}
*/

__attribute__((unused))
static u8 parse_hex(pcstr text, u8 maxLength) {
	u8 val			= 0;
	pcstr ptr		= text;
	pcstr ptr_end	= text + maxLength;
	while (*ptr != 0 && ptr != ptr_end) {
		val			<<= 4;
		char ch	= *ptr++;	
		if (ch >= 'A' && ch <= 'F') {
			val		+= 10 + ch - 'A';
		} else if (ch >= 'a' && ch <= 'f') {
			val		+= 10 + ch - 'a';
		} else if (ch >= '0' && ch <= '9') {
			val		+= ch - '0';
		}
	}
	return val;
}

__attribute__((unused))
static void deg_to_dm(s32 deg, gps_coord *coord) {
	u8		h, m, n	= 0; 
	u16		dm		= 0;
	if (deg < 0) {
		deg	= -deg;
		n	= 1;
	}
	// degrees
	h		= deg / 1000000;
	deg		= deg - h * 1000000;
	// minutes
	deg		*= 6;
	deg		/= 10;//div10_u32(deg);
	m		= deg / 10000;
	// sec*100
	dm		= deg % 10000;
	dm		*= 6; 
	dm		/= 10;//div10_u16(dm);
	
	// copy
	coord->hour		= h;
	coord->min		= m;
	coord->sec100	= dm;
	coord->neg		= n;
}

static s32 nmea_to_sec100(const gps_coord *coord) {
	u8	h		= coord->hour;
	u8	m		= coord->min; 
	u16	sec100	= coord->sec100;
	s8	neg		= coord->neg;

	s32 result	= h * 60;
	result		+= m;
	result		*= 60;
	result		*= 100;
	result		+= sec100;
	
	return		neg ? -result : result;
}

__attribute__((noinline,unused))
static s32 meter_to_feet(s32 v) {
	v			*= 3281;
	v			/= 1000;
	return		v;
}
__attribute__((unused))
static u16 knot_to_mph(u32 v) {
	v			*= 1151;
	v			/= 1000;
	return		v;
}
__attribute__((unused))
static u16 kmph_to_mph(u32 v) {
	v			*= 621;
	v			/= 1000;
	return		v;
}
__attribute__((unused))
static u16 knot_to_kmph(u32 v) {
	v			*= 463;
	v			/= 250;
	return		v;
}

__attribute__((noinline,unused))
static void parse_gps_coord(gps_coord *coord, const char* const text, u16 corr) {
	u8		h, m, n	= 0; 
	u16		dm	= 0;
	pcstr str	= text;

	if (*str == '-') {
		n	= 1;
		str++;
	}

	// hour
	h		=	(*str++ - '0');	h *= 10;
	h		+=	(*str++ - '0');
	if (*(str+3) == '.') {
		h	*=	10;
		h	+=	(*str++ - '0');
	}		
	// minutes
	m		=	(*str++ - '0');	m *= 10;
	m		+=	(*str++ - '0');
	// skip '.'
	str++;
	// dm
	for (u8 k = 0; k < 4; ++k) {
		dm	*=	10;
		dm	+=	((is_digit(*str) ? *str++ : '0') - '0');
	}
	
	// apply correction
	dm		+= corr;
	if (dm > 9999) {
		dm	-= 10000;
		m	++;
	}		
	if (m > 59) {
		m	-= 60;
		h	++;
	}		
	
	// convert to sec*100
	dm		*= 6; 
	dm		/= 10;//div10_u16(dm);
	
	// copy
	coord->hour		= h;
	coord->min		= m;
	coord->sec100	= dm;
	coord->neg		= n;
}

#endif /* UTILS_H_ */