#include "bm_auto_band.h"

/*
	ICOM
	Band Voltage -> with divider /2
	30m  0V-1V -> <0.5
	XXX  1V-2V -> <1.0
	10m  2V-3V -> <1.5
	15m  3V-4V -> <2.0
	20m  4V-5V -> <2.5 
	40m  5V-6V -> <3.0
	80m  6V-7V -> <3.5
	160m 7V-8V -> <4.0
*/

/*
	Club ICOM746
	Band Voltage -> with divider /2
	30m  0V-1V -> <0.5
	XXX  1V-1.5V -> <0.75
	10m  1.5V-2V -> <1.0
	15m  2V-3V -> <1.5
	20m  3V-4V -> <2.0 
	40m  4V-5V -> <2.5     
	80m  5V-6V -> <3.0     2.56
	160m 6V-8V -> <4.0     3.08
*/
	
//const unsigned char icom_v_thr[9] =
//	{26, 51, 77, 102, 128, 154, 179, 205, 255};

const unsigned char icom_v_thr[9] =
	{26, 38, 61,  85, 102, 128, 154, 205, 255};

const unsigned char icom_band[9] =
{
	  BAND_INVALID, //  10MHz but we do not support WARC on ICOM
	  BAND_INVALID, //  Invalid
	  BAND_10m, //   28MHz
	  BAND_15m, //   21MHz
	  BAND_20m, //   14MHz
	  BAND_40m, //    7MHz
	  BAND_80m, //  3.5MHz
	  BAND_160m, // 1.8MHz
	  BAND_INVALID // Invalid
};

unsigned char current_auto_band;

void checkAutoBand()
{
	unsigned char yaesu_data = PINB & 0x0F;
	if (yaesu_data == 0)
	{
		// 5MHz band not supported
		current_auto_band = BAND_INVALID;
	}
	else
	{
		current_auto_band = yaesu_data - 1;
	}

	if (current_auto_band > BAND_MAX)
	{
		// Try icom data if no data on 4bit band data input
		unsigned char volt_val = ADCH;
		unsigned char volt_id;
		for (volt_id = 0; volt_id < 9; volt_id++)
		{
			if (volt_val <= icom_v_thr[volt_id])
			{
				break;
			}
		}
		current_auto_band = icom_band[volt_id];
	}
	
	if (current_auto_band > BAND_MAX)
	{
		current_auto_band = BAND_INVALID;
	}
}
