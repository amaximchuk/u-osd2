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

#ifndef SENSORS_H_
#define SENSORS_H_

//__attribute__((noinline,unused))
//static void calc_adc(analog_value *r, u8 input) {
	//u16 raw			= g_adc_raw[input];
	//u8 scale			= g_adc_scale[input];
	//raw				*= scale;
	//u8 high			= raw >> 10;			// / 1024	
	//raw				= raw - ((high) << 10);	// * 1024
	//u8 low			= (raw * 25) >> 8;		// * 100 / 1024	
	//r->high			= high;
	//r->low			= low;
//}

__attribute__((noinline,unused))
static u16 calc_level(adc_def *def) {
	u8 input		= def->input;
	u32 level		= g_adc_raw[input];
	level			*= g_adc_scale[input];
	level			>>= HW_ADC_BITS;
	if (level > def->in_max) {
		level		= def->out_max;
	} else if (level > def->in_min) {
		level		-= def->in_min;
		level		*= def->out_max - def->out_min;
		level		/= def->in_max - def->in_min;
		level		+= def->out_min;
	} else {
		level		= def->out_min;
	}
	if (def->reversed)
		level		= def->out_max - level + def->out_min;
	return			level;
}

__attribute__((noinline,unused))
static void convert_to_analog(u16 level, analog_value *res) {
	res->val100		= level;
	res->high		= level / 100;
	res->low		= (level - (res->high * 100));
}

static void update_sensors() {
    convert_to_analog(calc_level(&cfg.vol1), &g_sensor_voltage1);
    convert_to_analog(calc_level(&cfg.vol2), &g_sensor_voltage2);

	// current
	u16 curr		= calc_level(&cfg.current);
	// non-linear correction (black magic numbers)
	//curr			= curr ? (((u32)curr * 93 / 100) + 18) : 0;
	g_sensor_power_usage	+= (((u32)(curr) * 10 << 10) / SENSOR_UPDATE_RATE / 3600);
    convert_to_analog(curr, &g_sensor_current);
	
	g_sensor_rssi	= calc_level(&cfg.rssi);
}  

static void measure_analog() {
	for (u8 i = 0; i < HW_ADC_NUMBERS; ++i) {
		ADMUX		&= 0xF0;				// Clear mux
		ADMUX		|= i;					// Setup adc mux
		ADCSRA		|= (1<<ADEN);			// ADC enable & ADC auto trigger enable
		ADCSRA		|= (1<<ADSC);			// Start measure
		while ((ADCSRA & (1<<ADIF)) == 0);	// Wait to finish
		ADCSRA		|= (1<<ADIF);			// Clear ADC interrupt flag with a 1
		ADCSRA		&= ~(1<<ADEN);			// ADC disabled & ADC auto trigger disabled
		g_adc_raw[i]= ADCW;
	}    
}

#endif /* SENSORS_H_ */