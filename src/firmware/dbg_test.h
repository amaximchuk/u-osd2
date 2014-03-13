/*u-osd - A simple open source osd for G-OSD
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

Credit to Carl Ljungström.
*/

#ifndef TEST_H_
#define TEST_H_

#ifdef DEBUG
static pcstr gps[][4] = {
	{"5029.5140", "3029.6280", "5031.1580", "3030.7740"},	// 3338m	24 deg
	{"5029.5150", "3029.6280", "5031.1580", "3030.7740"},	// 3338m	24 deg
	{"5029.6650", "3029.8968", "5031.2756", "3030.8180"},	// 3180m	21 deg
	{"5049.5500", "3029.5200", "5049.5500", "3029.5250"},	// 11m		90 deg
	//{ {{58,23,0,4380},  {15,35,0,3780}, 0},{58329389, {15535946},		// 25km		45 deg
	//{ {{58234380,  15353780, 58423961, 16119758},		// 50km		45 deg
//	{"5823.4380",  "1535.3780", "5823.7300", "1535.5660"},		// 570.75m		19 deg (!)
	//{"5824.4360",  "1537.7910", "5835.7150", "1611.2030"},		// 38633m	57 deg
	//{ {{58234380,  15353780, 58229075, 15355561},		// 998m		170 deg
//	{"5823.4380",  "1535.3780", "5823.4570", "1535.4143"},		// 50m		45 deg
	//{"5823.4380",  "1535.3780", "5823.4418", "1535.3853"},	// 10m		45 deg
	//{"-2301.0000", "-4601.0000","-2302.0000","-4601.0000"},	// 1853m	180 deg
//	{"-2302.0000", "-4601.0000","-2302.0000","-4602.0000"},	// 1706m	270 deg
//	{"-2302.0000", "-4602.0000","-2302.0000","-4601.0000"},	// 1706m	90 deg
	//{-23020000,-46010000,-23010000,-46010000},	// 1853m	360 deg
	{0}	
};

static u8 s2[40];
static TGpsPos home;
static TGpsPos curr;
static u16 dd0,dd1,dd2,dd3,dd4,dd5;
static s32 v0,v1,v2,v3,v4,v5;
static s16 r0,r1, r2,r3, r4, ii;

static u32 gHomeDistance2;
static u16 gHomeBearing2;
#endif // DEBUG


__attribute__((unused))
static void testCalcHome() {
#ifdef DEBUG
	for (u8 ii = 0; gps[ii][0]; ++ii) {
		parse_gps_coord	(&home.latitude,  gps[ii][0],GPS_CORR_DM_LAT);
		parse_gps_coord	(&home.longitude, gps[ii][1],GPS_CORR_DM_LON);
		parse_gps_coord	(&curr.latitude,  gps[ii][2],GPS_CORR_DM_LAT);
		parse_gps_coord	(&curr.longitude, gps[ii][3],GPS_CORR_DM_LON);

		calc_home		(&curr, &home, &g_home_distance, &g_home_bearing); 
		calc_home		(&home, &curr, &gHomeDistance2, &gHomeBearing2); 
	}
#endif //DEBUG

#if 0
	// center around 58234380, 15353780. 1000m out
	calc_home(58234380, 15353780, 58239767, 15353780, &g_home_distance, &g_home_bearing); // 0 deg
	calc_home(58234380, 15353780, 58239442, 15357289, &g_home_distance, &g_home_bearing); // 20 deg 
	calc_home(58234380, 15353780, 58238189, 15361035, &g_home_distance, &g_home_bearing); // 45 deg
	calc_home(58234380, 15353780, 58237073, 15362665, &g_home_distance, &g_home_bearing); //60 deg
	calc_home(58234380, 15353780, 58233444, 15363882, &g_home_distance, &g_home_bearing); //100
	calc_home(58234380, 15353780, 58229075, 15355561, &g_home_distance, &g_home_bearing); //170
	calc_home(58234380, 15353780, 58229318, 15350272, &g_home_distance, &g_home_bearing); //200
	calc_home(58234380, 15353780, 58236222, 15344139, &g_home_distance, &g_home_bearing); //290
	calc_home(58234380, 15353780, 58239685, 15351998, &g_home_distance, &g_home_bearing); //350
#endif
	
#if 0
	// center around -23010000, -46010000. 500m out
	calc_home(-23010000, -46010000, -23007291, -46010000, &g_home_distance, &g_home_bearing); // 0 deg
	calc_home(-23010000, -46010000, -23007454, -46008999, &g_home_distance, &g_home_bearing); // 20 deg 
	calc_home(-23010000, -46010000, -23008084, -46007931, &g_home_distance, &g_home_bearing); // 45 deg
	calc_home(-23010000, -46010000, -23008645, -46007466, &g_home_distance, &g_home_bearing); //60 deg
	calc_home(-23010000, -46010000, -23010470, -46007118, &g_home_distance, &g_home_bearing); //100
	calc_home(-23010000, -46010000, -23012668, -46009492, &g_home_distance, &g_home_bearing); //170
	calc_home(-23010000, -46010000, -23012546, -46011001, &g_home_distance, &g_home_bearing); //200
	calc_home(-23010000, -46010000, -23009073, -46012750, &g_home_distance, &g_home_bearing); //290
//	calc_home(-23010000, -46010000, -23007332, -46010508, &g_home_distance, &g_home_bearing); //350
#endif
	
#if 0
	// center around 46399970, -97259920, 150m out
	calc_home(46399970, -97259920, 46400780, -97259920, &g_home_distance, &g_home_bearing); // 0 deg
	calc_home(46399970, -97259920, 46400731, -97259518, &g_home_distance, &g_home_bearing); // 20 deg
	calc_home(46399970, -97259920, 46400542, -97259088, &g_home_distance, &g_home_bearing); // 45 deg
	calc_home(46399970, -97259920, 46400375, -97258902, &g_home_distance, &g_home_bearing); // 60 deg
	calc_home(46399970, -97259920, 46399829, -97258762, &g_home_distance, &g_home_bearing); // 100 deg
	calc_home(46399970, -97259920, 46399173, -97259716, &g_home_distance, &g_home_bearing); // 170 deg
	calc_home(46399970, -97259920, 46399209, -97260322, &g_home_distance, &g_home_bearing); // 200 deg
	calc_home(46399970, -97259920, 46400247, -97261025, &g_home_distance, &g_home_bearing); // 290 deg
	calc_home(46399970, -97259920, 46400767, -97260124, &g_home_distance, &g_home_bearing); // 350 deg
#endif

#if 0
	// center around 58234380, 15353780. 45 deg out
	calc_home(58234380, 15353780, 58234418, 15353853, &g_home_distance, &g_home_bearing); // 10m
	calc_home(58234380, 15353780, 58234570, 15354143, &g_home_distance, &g_home_bearing); // 50m
	calc_home(58234380, 15353780, 58234761, 15354505, &g_home_distance, &g_home_bearing); // 100m
	calc_home(58234380, 15353780, 58235332, 15355594, &g_home_distance, &g_home_bearing); // 250m
	calc_home(58234380, 15353780, 58236284, 15357407, &g_home_distance, &g_home_bearing); // 500m
	calc_home(58234380, 15353780, 58240093, 15364664, &g_home_distance, &g_home_bearing); // 1500m
	calc_home(58234380, 15353780, 58243900, 15371923, &g_home_distance, &g_home_bearing); // 2500m
	calc_home(58234380, 15353780, 58253416, 15390082, &g_home_distance, &g_home_bearing); // 5000m
	calc_home(58234380, 15353780, 58272436, 15426450, &g_home_distance, &g_home_bearing); // 10km
	calc_home(58234380, 15353780, 58291437, 15462883, &g_home_distance, &g_home_bearing); // 15km
	calc_home(58234380, 15353780, 58310422, 15499381, &g_home_distance, &g_home_bearing); // 20km
	calc_home(58234380, 15353780, 58329389, 15535946, &g_home_distance, &g_home_bearing); // 25km
	calc_home(58234380, 15353780, 58423961, 16119758, &g_home_distance, &g_home_bearing); // 50km
	calc_home(58234380, 15353780, 59011769, 16492394, &g_home_distance, &g_home_bearing); // 100km
#endif

#if 0
	calc_home(58234380, 15353780, 58237300, 15355660, &g_home_distance, &g_home_bearing); //572m 19 deg (!)
	calc_home(58244360, 15377910, 58357150, 16112030, &g_home_distance, &g_home_bearing); //38633m 57 deg
	calc_home(58357150, 16112030, 58244360, 15377910, &g_home_distance, &g_home_bearing); //38633m 237 deg
	calc_home(-23010000, -46010000, -23020000, -46010000, &g_home_distance, &g_home_bearing); // 1853m 180 deg
	calc_home(-23020000, -46010000, -23020000, -46020000, &g_home_distance, &g_home_bearing); // 1706m	270 deg
	calc_home(-23020000, -46020000, -23020000, -46010000, &g_home_distance, &g_home_bearing); // 1706m	90 deg
	calc_home(-23020000, -46010000, -23010000, -46010000, &g_home_distance, &g_home_bearing); // 1853m	360 deg
#endif
}

#endif /* TEST_H_ */