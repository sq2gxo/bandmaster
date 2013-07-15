#include "bm_display.h"
#include "bm_config_data.h"
#include "lcd.h"
#include "bm_timer.h"
#include "bm_messages.h"
#include "str_utils.h"


static const char *band_str[10] =
{
	"1.8MHz",
	"3.5MHz",
	"  7MHz",
	" 10MHz",
	" 14MHz",
	" 18MHz",
	" 21MHz",
	" 24MHz",
	" 28MHz",
	" 50MHz"
};

void showSplashScreen()
{
   	//Clear the screen
   	LCDClear();
	displayMsg(m1_splash, m2_splash, 0);
   	_delay_ms(2000);
}

void displayStatus()
{
   	LCDClear();
	LCDWriteStringXY(0, 0, bm_cfg.ProfileName);
	uint8_t sel_antenna_Id = bm_cfg.AntSelBandCfg[BandSelected];
	const char * msg = bm_cfg.AntLongNames[sel_antenna_Id];
	if (sel_antenna_Id == NO_ANTENNA)
	{
		msg = err_no_ant;
	}
	else if (isPeerAntConflict(sel_antenna_Id))
	{
		msg = err_ant_conflict;
	}
	uint8_t len = strLen(msg);
	uint8_t x = 7 + (10 - len) / 2;		
	LCDWriteStringXY(x, 0, msg);

	LCDWriteStringXY(18, 0, band_str[BandSelected]);

	// display antennas to select:
	for (uint8_t i = 0; i < MAX_ANT_PER_BAND; i++)
	{
		uint8_t antenna_Id = bm_cfg.AntAvailBandCfg[BandSelected][i];
		if (antenna_Id == NO_ANTENNA)
		{
			displayAntShort(i, "", "-");
		}
		else
		{
			const char* decor = " ";
			if (antenna_Id == sel_antenna_Id)
			{
				decor = "*";
			}
			if (isPeerAntConflict(antenna_Id))
			{
				decor = "X";
			}

			displayAntShort(i, bm_cfg.AntShortNames[antenna_Id], decor);		
		}
	}
}

void displayAntShort(uint8_t id, const char* name, const char* decor)
{
	uint8_t x = (id * 8);
	uint8_t len = strLen(name);
	x += (6 - len) / 2;
	LCDGotoXY(x,1);
	LCDWriteString(decor);
 	LCDWriteString(name);
	LCDWriteString(decor);
}


void displayMsg(const char* msg1, const char* msg2, uint8_t time_s)
{
	uint8_t len1 = strLen(msg1);
	if (len1 > 24)
	{
		return;
	}
	uint8_t len2 = strLen(msg2);
	if (len2 > 24)
	{
		return;
	}
   	LCDClear();
	LCDWriteStringXY(12 - (len1 / 2), 0, msg1);
	LCDWriteStringXY(12 - (len2 / 2), 1, msg2);
	if (time_s > 0)
	{
		scheduleDispRefresh(time_s);
	}
}
