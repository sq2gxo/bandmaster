
#include "bm_timer.h"
#include "bm_output.h"
#include "bm_serial.h"
#include "bm_config_data.h"
#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>


void disableAutoLED()
{
	PORTA |= 0b00000100;
}
void enableAutoLED()
{
	PORTA &= 0b11111011;
}


void disableManLED()
{
	PORTA |= 0b00001000;
}

void enableManLED()
{
	PORTA &= 0b11110111;
}


void disableManLEDBlink()
{
	man_diode_blink = 0;
	disableManLED();
}

void enableManLEDBlink()
{
	man_diode_blink = 1;
}


void disableLinkLED()
{
	PORTA |= 0b00010000;
}

void enableLinkLED()
{
	PORTA &= 0b11101111;
}


void blockPTT()
{
	PORTC |= 0b00001000;
}

void unblockPTT()
{
	// do not unblock if PTT is locked timed or PTT lock is permanent
	uint8_t permanentPttLock = isPermanentPttLock();
	if (permanentPttLock == 1)
	{
		blockPTT();
		return;
	}

	if (ptt_lock_timed == 1)
	{
		blockPTT();
		return;
	}

	PORTC &= 0b11110111;
}

void blockPTTTimed(uint8_t sec)
{
	if (sec > 0)
	{
		blockPTT();
		startPttLockTimed(sec);
	}
}


void applyOutPins()
{
	blockPTT();

	reportBandAntSerial();

	// Prepare 32 bit data
	// 31        20 19      10 9        0
	// AAAAAAAAAAAA|CCCCCCCCCC|BBBBBBBBBB
	// A - antena out
	// C - per band configurable
	// B - band data
	uint32_t serialData = 0;

	// add band data
	serialData |= (0x0001 << BandOutVal);

	// add configurable per band data
	uint32_t perBandData = bm_cfg.BandOutCfg[BandSelected];
	perBandData &= 0x03FF;
	perBandData = perBandData << 10;

	serialData |= perBandData;

	// add ant out data
	uint8_t antId = bm_cfg.AntSelBandCfg[BandSelected];
	if (antId != NO_ANTENNA)
	{
		uint32_t antData = bm_cfg.AntOutCfg[antId];
		antData &= 0x0FFF;
		antData = antData << 20;

		serialData |= antData;
	}

	
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		SD_CLOCK_OFF;
		_delay_us(50);
		SD_LATCH_OFF;
		_delay_us(50);

		for (uint8_t i = 0; i < 32; i++)
		{
			// set data
			if ((serialData >> (31 - i)) & 0x01)
			{
				SD_DATA_ON;
			}
			else
			{
				SD_DATA_OFF;
			}

			_delay_us(50);
		
			// tick clock
			SD_CLOCK_ON;
			_delay_us(50);

			SD_CLOCK_OFF;
		}
	
		// apply data
		SD_LATCH_ON;
		_delay_us(150);
		SD_DATA_OFF;
		SD_LATCH_OFF;
	}

	_delay_ms(15);

	applyPttStatus();
}

void applyPttStatus()
{
	uint8_t permanentPttLock = isPermanentPttLock();

	if (permanentPttLock == 0)
	{
		unblockPTT();
	}
	else
	{
		blockPTT();
	}
}
