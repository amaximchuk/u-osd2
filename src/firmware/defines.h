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

#ifndef DEFINES_H_
#define DEFINES_H_

#define TRUE 1
#define FALSE 0

typedef	char			*pstr;
typedef	const char		*pcstr;

typedef	signed char		s8;
typedef	unsigned char	u8;

typedef	signed short	s16;
typedef	unsigned short	u16;

typedef	signed long		s32;
typedef	unsigned long	u32;

#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((u32)(u8)(ch0) | ((u32)(u8)(ch1) << 8) | ((u32)(u8)(ch2) << 16) | ((u32)(u8)(ch3) << 24 ))

#endif /* DEFINES_H_ */