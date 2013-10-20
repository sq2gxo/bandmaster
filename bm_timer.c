#include "bm_timer.h"
#include "bm_serial.h"
#include "bm_output.h"
#include "bm_config_data.h"
#include <util/atomic.h>

uint8_t sw_state[SW_COUNT] = {SW_OPEN, SW_OPEN, SW_OPEN, SW_OPEN, SW_OPEN, SW_OPEN};
uint8_t refresh_disp_needed = 0;
uint8_t refresh_ptt_needed = 0;
uint8_t refresh_bandout_needed = 0;
uint8_t man_diode_blink = 0;
uint8_t ptt_lock_timed = 0;
uint8_t link_active = 0;

static uint16_t sw_press_timer[SW_COUNT] = {0, 0, 0, 0, 0, 0};
static uint8_t sw_mask[SW_COUNT] = {ANT1_SW_MASK, ANT2_SW_MASK, ANT3_SW_MASK, MODE_SW_MASK, DWN_SW_MASK, UP_SW_MASK};
static uint16_t disp_clr_timer = 0;
static uint8_t diode_blink_timer = 0;
static uint16_t ptt_lock_timer = 0;
static uint8_t serial_ping_timer = 0;
static uint16_t serial_link_timer = 0;


void doTimerInterrupt()
{
	// Check PTT lock timer
	if (ptt_lock_timed == 1)
	{	
		//if PTT pressed during band out delay - interrupt timer
		if ((IS_TX) && (BandSelected != BandOutVal))
		{
			ptt_lock_timer = 0;
		}

		
		if (ptt_lock_timer == 0)
		{
			ptt_lock_timed = 0;

			// check if band out delay was executed, if yes apply out and start again with band change delay
			if (BandSelected == BandOutVal)
			{
				unblockPTT();
			}
			else
			{
				BandOutVal = BandSelected;
				refresh_bandout_needed = 1;
				blockPTTTimed(bm_cfg.BandChngDelay);
			}
		}
		else
		{
			ptt_lock_timer--;
		}
	}

	// check status reporting
	serial_ping_timer++;

	// report status every 2s
	if (serial_ping_timer >= 200)
	{
		reportBandAntSerial();
		serial_ping_timer = 0;
	}

	// light rs232 link status LED
	if (serial_link_timer > 0)
	{
		serial_link_timer--;
		if (serial_link_timer == 0)
		{
			disableLinkLED();
			link_active = 0;
			refresh_ptt_needed = 1;
			// set serial mode to default
			serial_mode = DEVICE2DEVICE;
		}
	}
	// if needed - send serial byte from buffer
	serial_push_byte_out();

	// Check switches
	uint8_t i;
	for (i = 0; i < SW_COUNT; i++)
	{
		if (IS_SW_PRESSED(sw_mask[i]))
		{
			sw_press_timer[i] += 1;
			if (sw_press_timer[i] == SW_PRESS_TIME)
			{
				sw_state[i] = SW_PRESS;
			}
			if (sw_press_timer[i] == SW_HOLD_TIME)
			{
				sw_state[i] = SW_HOLD;
			}
		}
		else
		{
			//if (sw_press_timer[i] > SW_PRESS_TIME && sw_press_timer[i] < SW_HOLD_TIME)
			//{
			//	sw_state[i] = SW_PRESS;
			//}
			sw_press_timer[i] = 0;
		}
	}


	// Check clear user message timer
	if (disp_clr_timer > 0)
	{
		if (disp_clr_timer == 1)
		{
			refresh_disp_needed = 1;
		}
		disp_clr_timer--;
	}

	// Manual diode blink timer
	diode_blink_timer++;

	if (diode_blink_timer == LED_BLINK_TIME)
	{
		if (man_diode_blink > 0)
		{
			enableManLED();
		}
	}
	if (diode_blink_timer == 2 * LED_BLINK_TIME)
	{
		diode_blink_timer = 0;
		if (man_diode_blink > 0)
		{
			disableManLED();
		}
	}
}


void scheduleDispRefresh(uint8_t time_s)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		disp_clr_timer = 100 * time_s;
	}
}

void startPttLockTimed(uint8_t time_s)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		if (time_s > 0)
		{
			ptt_lock_timed = 1;
			ptt_lock_timer = 100 * time_s;
		}
	}
}

void stopPttLockTimed()
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		ptt_lock_timed = 0;
		ptt_lock_timer = 0;
	}
}

void enableLinkTimer()
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		serial_link_timer = 500;
		enableLinkLED();
	}
}
