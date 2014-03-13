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

#ifndef GPS_DIYD_H_
#define GPS_DIYD_H_

// structs and vars for MTK DIYD GPS
typedef struct {
	s32		latitude;
	s32		longitude;
	s32		altitude;
	s32		ground_speed;
	s32		ground_course;
	u8		satellites;
	u8		fix_type;
	u32		utc_date;
	u32		utc_time;
	u16		hdop;
} TDiydMtkMsg;

typedef enum {
	FIX_NONE= 1,
	FIX_2D	= 2,
	FIX_3D	= 3
} TDiydMtkFix;

typedef enum {
	PREAMBLE1 = 0xd0,
	PREAMBLE2 = 0xdd,
} TDiydMtkProtocolPreamble;

// Packet checksum accumulators
static u8 gDiydChecksum1;
static u8 gDiydChecksum2;

// State machine state
static u8 gDiydStep = 0;
static u8 gDiydPayloadCounter;

// Receive buffer
static union {
	TDiydMtkMsg msg;
	u8	bytes[sizeof(TDiydMtkMsg)];
} gDiydBuffer;

static void resetDecodeStep() {
	gDiydStep = 0;
}

static void decode_gps(char data) {
	u8 finished = 0;
	while(!finished) {
		finished = 1;
		switch(gDiydStep){

			// Message preamble, class, ID detection
			//
			// If we fail to match any of the expected bytes, we
			// reset the state machine and re-consider the failed
			// byte as the first byte of the preamble.  This 
			// improves our chances of recovering from a mismatch
			// and makes it less likely that we will be fooled by
			// the preamble appearing as data in some other message.
			//
		case 0:
			if(PREAMBLE1 == data) {
				gDiydStep++;
			}
			break;
		case 1:
			if (PREAMBLE2 == data) {
				gDiydStep++;
				break;
			}
			gDiydStep = 0;
			finished = 0;
			break;
		case 2:
			if (sizeof(gDiydBuffer) == data) {
				gDiydStep++;
				gDiydChecksum2 = gDiydChecksum1 = data;				// reset the checksum accumulators
				gDiydPayloadCounter = 0;
			} 
			else {
				gDiydStep = 0;							// reset and wait for a message of the right class
				finished = 0;
				break;
			}
			break;

			// Receive message data
			//
		case 3:
			gDiydBuffer.bytes[gDiydPayloadCounter++] = data;
			gDiydChecksum2 += (gDiydChecksum1 += data);
			if (gDiydPayloadCounter == sizeof(gDiydBuffer)) {
				gDiydStep++;
			}
			break;

			// Checksum and message processing
			//
		case 4:
			gDiydStep++;
			if (gDiydChecksum1 != data) {
				gDiydStep = 0;
			}
			break;
		case 5:
			gDiydStep = 0;
			if (gDiydChecksum2 != data) {
				break;
			}

			/* Fill in the vars we need for cl-osd */
			g_gps_data.fix            = gDiydBuffer.msg.fix_type == FIX_3D;
			g_gps_data.pos.latitude   = gDiydBuffer.msg.latitude;
			g_gps_data.pos.longitude  = gDiydBuffer.msg.longitude;
			g_gps_data.pos.altitude   = (s16) (gDiydBuffer.msg.altitude / 100);
			g_gps_data.speed          = (u16) ((gDiydBuffer.msg.ground_speed * 36) / 1000);
			g_gps_data.angle          = (u16) (gDiydBuffer.msg.ground_course / 100);
			g_gps_data.sats           = gDiydBuffer.msg.satellites;
			g_gps_data.date           = gDiydBuffer.msg.utc_date;
			g_gps_data.g_time          = gDiydBuffer.msg.utc_time / 1000;
			g_gps_data.checksumValid  = 1;

			finish_gps_decoding();
			break;
		}
	}
} 

#endif /* GPS_DIYD_H_ */