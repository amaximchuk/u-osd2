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

#ifndef SETUP_H_
#define SETUP_H_

//62 is caused by the voltage divider on the pcb.
//There is a 5.1k and a 1k resistor in the voltage divider.
//Using ohms law we get that Vout = (Z_2 / (Z_1 + Z_2)) * Vin.
//Flipping some numbers (Vout = 5, Z_1 = 5.1k, Z_2 = 1k) we get Vin = 6,1 * Vout.
//Since we don't want floats we use 61 and divide with 10 later. But due to some testing I found that 62 was better. :-) 
//temp = ADCW;
//#ifdef ADC_ENABLE_RAW
//gAnalogInputsRaw[i] = temp;
//#endif
//temp = (temp * 5 * 62) / 10;
//temp *= gAnalogMult[i];
//temp /= gAnalogDiv[i];
//adcHigh = temp / 1024;
//temp -= (u16)(adcHigh) * 1024;
//adcLow = (temp * 100) / 1024;
//
//gAnalogInputs[i].low = adcLow;
//gAnalogInputs[i].high = adcHigh;
//
//
//As per Atmel spec for 10 bit conversions:
//
//ADC value = Vin * 1024 / Vref
//
//Vref is set to AVCC (5 Volt IIRC) on setupAdc().
//
//So for the RSSI pin (G-OSD) we can get 0-5V for Vin. The gAnalogDiv[] value for this input is 6 (hardware.h, ADC_DIV definition).
//
//For 1V the ADC value would be 0xCC => 204 which is available in the variable temp.
//
//What the code does is this:
//
//temp = (204 * 5 * 62) / 10 = 6324
//temp = 6324 / 6 = 1054 // 6 comes from gAnalogDiv[i]
//adcHigh = 1054 / 1024 = 1 => 1V for display
//temp = 1054 - 1024 = 30
//adcLow = 30 * 100 / 1024 = 2 => .02V for display
// ADC prescaler selections.
// ADPS2	ADPS1	ADPS0	Division factor
// 0		0		0		2
// 0		0		1		2
// 0		1		0		4
// 0		1		1		8
// 1		0		0		16
// 1		0		1		32
// 1		1		0		64
// 1		1		1		128
static void setup_adc() {
	// ADC setup
	ADMUX	|= (1<<REFS0);				// Ref is AVCC
	ADCSRA	|= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); 
//	DIDR0	= 0x00;
//	ADCSRB	= 0;						// Free running
//	ACSR	= (1<<ACD);					// Comparator disabled
}

#define GPS_UBRR_INT (F_CPU/16/100)

static void setup_gps() {
	u16 gps_ubrr	= 0;
	switch (cfg.gps.baud) {
	case GPS_4800:	gps_ubrr	= GPS_UBRR_INT / 48 - 1; break;
	case GPS_9600:
	default:		gps_ubrr	= GPS_UBRR_INT / 96 - 1; break;
	}
	
	// USART setup
#if (defined __AVR_ATmega8__)
	UBRRH	= (u8)(gps_ubrr>>8);// set baud
	UBRRL	= (u8)gps_ubrr;
	UCSRC	= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); // 8N1 (8 bit, 1 stop bit)
	UCSRB	= (1<<RXEN);		// Enable RX
#else
 	UBRR0H	= (u8)(gps_ubrr>>8);// set baud
 	UBRR0L	= (u8)gps_ubrr;
	UCSR0C	= (1<<UCSZ01)|(1<<UCSZ00);		// 8N1 (8 bit, 1 stop bit)
 	UCSR0B	= (1<<RXEN0);		// Enable RX
#endif	
}

static void setup_layout() {
	g_length_unit	= cfg.units == UNITS_IMPERIAL ? 'F' : 'm';
	if (MODE_NTSC == cfg.video_mode)	cfg.layout.rows_offset	+= SERVICE_LINES_NTSC;
	else								cfg.layout.rows_offset	+= SERVICE_LINES_PAL;
//	cfg.layout.rows_offset	+= SERVICE_LINES_NTSC;
//	g_radar_last_pos		= cfg.layout.radar.c;
//	g_home_arrow_last_pos	= cfg.layout.home_arrow.c;
	g_radar_last_pos.x		= 0;
	g_radar_last_pos.y		= 0;
	g_home_arrow_last_pos.x = 0;
	g_home_arrow_last_pos.y = 0;

//	print_text		(&cfg.layout.sign, cfg.callsign, TRUE);
}	

static void setup_line() {
	SHADOW_OFF;
	DDRB	= (1<<WHITE_OUT) | (1<<SS_OUT);

	// ISC01	ISC00	Description
	// ISC11	ISC10	Description
	// 0		0		The low level of INT0 generates an interrupt request
	// 0		1		Any logical change on INT0 generates an interrupt request
	// 1		0		The falling edge of INT0 generates an interrupt request
	// 1		1		The rising edge of INT0 generates an interrupt request
	// Line trigger
#if (defined __AVR_ATmega8__)
	MCUCR	= (0<<ISC01) | (1<<ISC00) | (0<ISC11);// | (0<ISC10);
	GICR	= (1<<INT0) | (1<<INT1);	// enable INT0:1 interrupts
#else
	EICRA	= (1<<ISC00) | (1<<ISC01);	// set INT0 as rising edge trigger
	EIMSK	= (1<<INT0);				// enable INT0 in global interrupt mask
#endif

	// SPI setup
	// SPDR:
	// The SPI Data Register is a read/write register used for data transfer between the Register File and the SPI Shift Register. 
	// Writing to the register initiates data transmission. Reading the register causes the Shift Register receive buffer to be read.
	// Finally, here is a code snippet to generate a data transfer between a Master and a Slave. 
	SPDR	= 0x00;						// Clear SPI reg or thrash will show on video
	
	// SPSR:
	// SPI2x (Double SPI Speed) bit:	This feature is not implemented in all AVRs (check the particular data sheet). 
	//									When this bit is set to one, the SPI speed will be doubled when the SPI is in Master mode.
	// SPIF (SPI Interrupt Flag) bit:	This is a read only bit. It is set by hardware when a serial transfer is complete. 
	//									SPIF is cleared by hardware when the SPI interrupt handling vector is executed, or when the SPIF bit and the SPDR register are read.

	// SPCR:
	// Bit 7 	Bit 6 	Bit 5 	Bit 4 	Bit 3 	Bit 2 	Bit 1 	Bit 0
	// SPIE		SPE		DORD	MSTR	CPOL	CPHA	SPR1	SPR0	
	// SPIE (SPI Interrupt Enable) bit: Set SPIE to one if you want the SPI interrupt to be executed when a serial transfer is completed.
	// SPE (SPI Enable) bit: If you want to use the SPI, you must set this bit.
	// DORD (Data Order) bit: You can choose in which order the data will be transmitted. Set DORD to one to send the least significant bit (LSB) first. Set DORD to zero to send the most significant bit (MSB) first.
	// MSTR (Master/Slave Select) bit: Set MSTR to configure the AVR as a Master SPI device. Clear MSTR to configure it as a Slave.
	// CPOL (Clock Polarity) and CPHA (Clock Phase) bits: As stated previously, Master and Slave must agree on how to interpret the clock signal. The first thing to do is to configure which logic level the clock will be in when the SPI is idle. If CPOL is set to one, SCK is high when idle, and if CPOL is set to zero, SCK is low when idle. The second thing is to configure during which clock transition the data will be sampled. Set CPHA to sample the data on the trailing (last) edge, and clear CPHA to sample the data in the leading (first) edge.
	// So, there are four different ways of configuring the clock generation, which are known as 'SPI modes'. The following table summarizes the four SPI modes.
	// SPI Mode CPOL 	CPHA 	Sample
	// 0 		0 		0 		Leading (Rising) Edge
	// 1 		0 		1 		Trailing (Falling) Edge
	// 2 		1 		0 		Leading (Falling) Edge
	// 3 		1 		1 		Trailing (Rising) Edge	
	// SPR1 and SPR2 (SPI Clock Rate Select) bits: The SPR bits configure the frequency of the clock signal. Since the Slave reads the clock from an input pin, the SPR bits have no effect on the Slave. The frequency of the SPI clock is related to the frequency of the AVR oscillator. The faster the SPI clock signal is, the faster the data trasfer will be, however, you must respect the maximum clock frequency specified by the Slave (as usual, read the datasheet). The following table summarizes the relationship between the SCK frequency and the SPR bits:
	// SPR1 SPR0	SCK frequency
	// 0 	0 		fosc/4
	// 0 	1 		fosc/16
	// 1 	0 		fosc/64
	// 1 	1 		fosc/128
	SPCR	= (1<<SPE) | (1<<MSTR) | (1<<CPHA);
}			

static void setup(void)
{
	//CS12 	CS11 	CS10
	//0 	0 		1		CK
	//0 	1		0		CK / 8
	//0 	1		1		CK / 64
	//1 	0 		0 		CK / 256
	//1 	0 		1 		CK / 1024	
#if (defined __AVR_ATmega8__)
	TIMSK	|= (1 << TOIE1);				// Enable overflow interrupt
#else
 	TIMSK1	|= (1 << TOIE1);				// Enable overflow interrupt
#endif 
	TCCR1B	|= ((1 << CS10) | (1 << CS11));	// Start timer at Fcpu/64 according to TIMER_FPS
//	TCCR1B	|= (1 << CS12);	// Start timer at Fcpu/64 according to TIMER_FPS

	//CS22 	CS21 	CS20
	//0 	0 		1		CK
	//0 	1		0		CK / 8
	//0 	1		1		CK / 32
	//1 	0		0		CK / 64
	//1 	0		1		CK / 128
	//1 	1		0 		CK / 256
	//1 	1 		1 		CK / 1024	
//	TIMSK2	|= (1 << TOIE2);				// Enable overflow interrupt
//	TCCR2B	|= ((1 << CS22) | (1 << CS21) | (1 << CS20));	
	
	// load from EEPROM
	eeprom_read_block(&cfg,	(const void*)0,sizeof(config));

	// Port setup
#ifndef	_SXOSD
	PORTD		= cfg.pin.key_mask;			// key pullup & led off
	DDRD		= cfg.pin.led_mask;			// led output
#endif // _SXOSD

	setup_line	();
	setup_layout();
	setup_adc	();
	setup_gps	();
	
	sei			();
}

#endif /* SETUP_H_ */