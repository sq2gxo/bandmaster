/*

v0.9.2 28.06.2013
 * PTT press finish band out delay timer
 * Chenged key press detection behaviour
   (press event generated after defined time, not after releasing key)

v0.9.1 26.06.2013
 * changed MAX_PROFILES to 1
 * added configurable delay before switching BAND OUT data

v0.9 configured outputs for SP2YWL club
*/

// TODO
// serial programming

#include "bm_uc_cfg.h"
#include "bm_config_data.h"
#include "bm_init.h"
#include "bm_timer.h"
#include "bm_serial.h"
#include "bm_auto_band.h"
#include "bm_display.h"
#include "bm_messages.h"
#include "bm_output.h"
#include "bm_serial.h"

#include "lcd.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include <stdio.h>

void changeBand()
{
	if (bm_cfg.BandOutDelay == 0)
	{
		BandOutVal = BandSelected;
		blockPTTTimed(bm_cfg.BandChngDelay);
	}
	else
	{
		blockPTTTimed(bm_cfg.BandOutDelay);
	}
	applyOutPins();
	updateCurrentEEConfig();
	displayStatus();	
}

int main (void)
{
	// Initialize
	//---------------------------------------
	// 1) set band data inputs
	initPORTS();
	// 2) confgure ADC for ICOM band data
	initADC(); 
	// 3) Read EEPROM data
	readEEConfig();
	// 4) set output
	applyOutPins();
	// 5) Enable 10ms timer
	initTIMER();
	// 6) Enable serial port communication
	initSERIAL();
   	// 7) Initialize LCD module
   	LCDInit(LS_NONE);
	// 8) display welcome message
	showSplashScreen();
	// 9) show status on screen
	displayStatus();
	// 10) set LEDs
	if (BandDecode_Mode == BAND_MODE_AUTO)
	{
		enableAutoLED();
		disableManLED();
	}
	else
	{
		enableManLED();
		disableAutoLED();
	}
	disableLinkLED();


	// Main loop
	//---------------------------------------
	while(1)
	{
		// Get auto band
		//---------------------------------------
		if (BandDecode_Mode == BAND_MODE_AUTO)
		{
			checkAutoBand();
			// check if changed and refresh screen...
			if (current_auto_band != BAND_INVALID)
			{			
				if (BandSelected != current_auto_band)
				{
					if (IS_TX)
					{
						displayMsg(m1_band_change_when_tx_warn, m2_when_tx, 2);
					}
					BandSelected = current_auto_band;

					changeBand();
				}
				disableManLEDBlink();
			}
			else
			{
				enableManLEDBlink();
			}
		}
	
		// Selecting antennas
		//---------------------------------------
		int8_t ant_chng = -1;
		if (sw_state[ANT1_SW_ID] == SW_PRESS)
		{
			sw_state[ANT1_SW_ID] = SW_OPEN;
			ant_chng = 0;
		}
		else if (sw_state[ANT2_SW_ID] == SW_PRESS)
		{
			sw_state[ANT2_SW_ID] = SW_OPEN;
			ant_chng = 1;
		}
		else if (sw_state[ANT3_SW_ID] == SW_PRESS)
		{
			sw_state[ANT3_SW_ID] = SW_OPEN;
			ant_chng = 2;
		}
		if (ant_chng != -1)
		{
			uint8_t new_ant_id = bm_cfg.AntAvailBandCfg[BandSelected][ant_chng];
			if (new_ant_id != NO_ANTENNA && bm_cfg.AntSelBandCfg[BandSelected] != new_ant_id)
			{
				if (IS_TX)
				{
					displayMsg(m1_ant_change_press_when_tx, m2_when_tx, 2);
				}
				else
				{
					bm_cfg.AntSelBandCfg[BandSelected] = new_ant_id;
					applyOutPins();
					updateCurrentEEConfig();
					displayStatus();
				}
			}
		}	
		
		// Selecting band change mode
		//---------------------------------------
		if (sw_state[MODE_SW_ID] == SW_PRESS)
		{
			sw_state[MODE_SW_ID] = SW_OPEN;
			if (BandDecode_Mode == BAND_MODE_AUTO)
			{
				BandDecode_Mode = BAND_MODE_MAN;
				disableManLEDBlink();
				enableManLED();
				disableAutoLED();
				displayMsg(m1_man_mode, m_empty, 2);
			}
			else
			{
				BandDecode_Mode = BAND_MODE_AUTO;
				enableAutoLED();
				disableManLED();
				displayMsg(m1_auto_mode, m_empty, 2);
			}
			updateCurrentEEConfig();
		}

		// Selecting band manual
		//---------------------------------------
		else if (sw_state[UP_SW_ID] == SW_PRESS)
		{
			sw_state[UP_SW_ID] = SW_OPEN;
			if (BandDecode_Mode == BAND_MODE_MAN)
			{
				if (IS_TX)
				{
					displayMsg(m1_band_change_press_when_tx, m2_when_tx, 2);
				}
				else
				{
					if (BandSelected < BAND_MAX)
					{
						BandSelected++;
					}
					else
					{
						BandSelected = 0;
					}
					
					changeBand();
				}
			}
			else
			{
				displayMsg(m1_band_change_press_when_auto, m2_band_change_press_when_auto, 3);
			}
		}

		else if (sw_state[DWN_SW_ID] == SW_PRESS)
		{
			sw_state[DWN_SW_ID] = SW_OPEN;

			if (BandDecode_Mode == BAND_MODE_MAN)
			{
				if (IS_TX)
				{
					displayMsg(m1_band_change_press_when_tx, m2_when_tx, 2);
				}
				else
				{
					if (BandSelected > 0)
					{
						BandSelected--;
					}
					else
					{
						BandSelected = BAND_MAX;
					}
					
					changeBand();
				}
			}
			else
			{
				displayMsg(m1_band_change_press_when_auto, m2_band_change_press_when_auto, 2);
			}
		}

		// Change profile
		//---------------------------------------
		/*
		else if (sw_state[MODE_SW_ID] == SW_HOLD)
		{
			sw_state[MODE_SW_ID] = SW_OPEN;
		
			if (IS_TX)
			{
				displayMsg(m1_profile_change_press_when_tx, m2_when_tx, 2);
			}
			else
			{
				if (profile_Id >= MAX_PROFILES - 1)
				{
					profile_Id = 0;
				}
				else
				{
					profile_Id++;
				}
				char msg[24];
				snprintf(msg, 24, m1_profile, bm_cfg.ProfileName);
				displayMsg(msg, m2_activated, 2);
				readEEConfigProfile(profile_Id);
				//blockPTTTimed();
				applyOutPins();
				updateCurrentEEConfig();
				//displayStatus();
			}
		}
		*/

		// Refresh display if needed after displaying error message
		//---------------------------------------
		if (refresh_disp_needed == 1)
		{
			refresh_disp_needed = 0;
			displayStatus();
		}

		// Refresh output/display if needed after loosing peer link
		//---------------------------------------
		if (refresh_ptt_needed == 1)
		{
			refresh_ptt_needed = 0;
			applyPttStatus();
			displayStatus();
		}

		if (refresh_bandout_needed == 1)
		{
			refresh_bandout_needed = 0;
			applyOutPins();
		}

		// Check and process serial port messages
		//---------------------------------------
		processSerialMsg();

	}
	// Main loop end


}


ISR ( TIMER1_COMPA_vect )
{
	doTimerInterrupt();
}

ISR(USART_RXC_vect)
{
	doSerialInterrupt();
}
