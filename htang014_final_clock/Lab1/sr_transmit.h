#ifndef SR_TRANSMIT_H
#define SR_TRANSMIT_H

#define SR_IN PORTC
#define SR_DDR DDRC

#define SRCLR_HI SR_IN | 0x01
#define SRCLR_LO SR_IN & ~0x01
#define SRCLK_HI SR_IN | 0x02
#define SRCLK_LO SR_IN & ~0x02
#define SER_HI SR_IN | 0x08
#define SER_LO SR_IN & ~0x08
#define RCLK_HI SR_IN | 0x04
#define RCLK_LO SR_IN & ~0x04

char trns_i;

void sr_init()
{
	SR_DDR = 0xFF;
}

void transmit_data(unsigned char data) {
	for (trns_i = 7; trns_i >= 0; trns_i--) {
		SR_IN = SRCLR_HI;
		SR_IN = SRCLK_LO;
		SR_IN = ((data >>trns_i) & 0x01) ? SER_HI:SER_LO;
		SR_IN = SRCLK_HI;
	}
	SR_IN = RCLK_HI;
	SR_IN = 0x00;
}

#endif