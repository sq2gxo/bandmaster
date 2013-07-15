#include "bm_config_data.h"
#include "bm_auto_band.h"
#include "bm_timer.h"


bm_config_t bm_cfg;

bm_config_t EEMEM cfg[MAX_PROFILES] = 
{
	{
		{'S', 'T', 'N', ' ', 'A', 0, 0},

		{
		{'V', 'E', 'R', 'T',   0, 0},
		{'4', 'S', 'Q',   0,   0, 0},
		{'D', 'E', 'L', 'T', 'A', 0},
		{'Y', 'A', 'G', 'I',   0, 0},
		{'D', 'E', 'L', 'T', 'A', 0},
		{'Y', 'A', 'G', 'I',   0, 0},
		{'Y', 'A', 'G', 'I',   0, 0},
		{'S', 'T', 'A', 'C', 'K', 0},
		{'Y', 'A', 'G', 'I',   0, 0},
		{'S', 'T', 'A', 'C', 'K', 0}
		},
		
		{
		{'V', 'E', 'R', 'T', 'I', 'C', 'A', 'L',   0,   0, 0},
		{'4', 'S', 'Q', 'U', 'A', 'R', 'E', ' ', '8', '0', 0},
		{'D', 'E', 'L', 'T', 'A', ' ', '8', '0',   0,   0, 0},
		{'Y', 'A', 'G', 'I', ' ', '4', '0',   0,   0,   0, 0},
		{'D', 'E', 'L', 'T', 'A', ' ', '3', '0',   0,   0, 0},
		{'Y', 'A', 'G', 'I', ' ', '2', '0',   0,   0,   0, 0},
		{'Y', 'A', 'G', 'I', ' ', 'W', 'A', 'R', 'C',   0, 0},
		{'S', 'T', 'A', 'C', 'K', ' ', '1', '5',   0,   0, 0},
		{'Y', 'A', 'G', 'I', ' ', 'W', 'A', 'R', 'C',   0, 0},
		{'S', 'T', 'A', 'C', 'K', ' ', '1', '0',   0,   0, 0},
		},

		{
		0b000000000001,
		0b000001000000,
		0b000000100000,
		0b000000010000,
		0b000100001000,
		0b000000000100,
		0b001000001000,
		0b000000000010,
		0b010000001000,
		0b000010000000
		},

		{
		0b0000000001,
		0b0000000010,
		0b0000000100,
		0b0000000000,
		0b0000001000,
		0b0000000000,
		0b0000010000,
		0b0000000000,
		0b0000100000,
		0b0000000000
		},

		{
		{0, NO_ANTENNA, NO_ANTENNA},
		{1, 2, NO_ANTENNA},
		{3, NO_ANTENNA, NO_ANTENNA},
		{4, NO_ANTENNA, NO_ANTENNA},
		{5, NO_ANTENNA, NO_ANTENNA},
		{6, NO_ANTENNA, NO_ANTENNA},
		{7, NO_ANTENNA, NO_ANTENNA},
		{8, NO_ANTENNA, NO_ANTENNA},
		{9, NO_ANTENNA, NO_ANTENNA},
		{NO_ANTENNA, NO_ANTENNA, NO_ANTENNA}
		},

		12,

		6,

		1,

		{0, 1, 3, 4, 5, 6, 7, 8, 9, NO_ANTENNA},

	}
	/*,

	{
		{'S', 'T', 'N', ' ', 'A', 0, 0},

		{
		{'V', 'E', 'R', 'T',   0, 0},
		{'4', 'S', 'Q',   0,   0, 0},
		{'D', 'E', 'L', 'T', 'A', 0},
		{'Y', 'A', 'G', 'I',   0, 0},
		{'D', 'E', 'L', 'T', 'A', 0},
		{'Y', 'A', 'G', 'I',   0, 0},
		{'Y', 'A', 'G', 'I',   0, 0},
		{'S', 'T', 'A', 'C', 'K', 0},
		{'Y', 'A', 'G', 'I',   0, 0},
		{'S', 'T', 'A', 'C', 'K', 0}
		},
		
		{
		{'V', 'E', 'R', 'T', 'I', 'C', 'A', 'L',   0,   0, 0},
		{'4', 'S', 'Q', 'U', 'A', 'R', 'E', ' ','8', '0', 0},
		{'D', 'E', 'L', 'T', 'A', ' ', '8', '0',   0,   0, 0},
		{'Y', 'A', 'G', 'I', ' ', '4', '0',   0,   0,   0, 0},
		{'D', 'E', 'L', 'T', 'A', ' ', '3', '0',   0,   0, 0},
		{'Y', 'A', 'G', 'I', ' ', '2', '0',   0,   0,   0, 0},
		{'S', 'T', 'A', 'C', 'K', ' ', '1', '5',   0,   0, 0},
		{'A', 'N', 'T', 'E', 'N', 'N', 'A', ' ', '8',   0, 0},
		{'Y', 'A', 'G', 'I', ' ', 'W', 'A', 'R', 'C',   0, 0},
		{'S', 'T', 'A', 'C', 'K', ' ', '1', '0',   0,   0, 0},
		},

		{
		0b000000000001,
		0b000001000000,
		0b000000100000,
		0b000000010000,
		0b000100001000,
		0b000000000100,
		0b001000001000,
		0b000000000010,
		0b010000001000,
		0b000010000000
		},

		{
		0b0000000001,
		0b0000000010,
		0b0000000100,
		0b0000000000,
		0b0000001000,
		0b0000000000,
		0b0000010000,
		0b0000000000,
		0b0000100000,
		0b0000000000
		},

		{
		{0, NO_ANTENNA, NO_ANTENNA},
		{1, 2, NO_ANTENNA},
		{3, NO_ANTENNA, NO_ANTENNA},
		{4, NO_ANTENNA, NO_ANTENNA},
		{5, NO_ANTENNA, NO_ANTENNA},
		{6, NO_ANTENNA, NO_ANTENNA},
		{7, NO_ANTENNA, NO_ANTENNA},
		{8, NO_ANTENNA, NO_ANTENNA},
		{9, NO_ANTENNA, NO_ANTENNA},
		{NO_ANTENNA, NO_ANTENNA, NO_ANTENNA}
		},

		12,

		6,

		1,

		{0, 1, 3, 4, 5, 6, 7, 8, 9, NO_ANTENNA},

	}*/
};

uint8_t profile_Id;

uint8_t EEMEM EE_profile_Id = 0;

// Mode (AUTO /MANUAL)
uint8_t BandDecode_Mode;
uint8_t EEMEM EE_BandDecode_Mode = BAND_MODE_MAN;

// Manual band selected (0 - 9)
uint8_t BandSelected;
uint8_t EEMEM EE_BandSelected = BAND_160m;

// BAND OUT value
uint8_t BandOutVal = 0;

// Peer status
uint8_t PeerBand = BAND_INVALID;
uint16_t PeerAntCfg = 0;


// READ config from EEPROM to RAM
void readEEConfig(void)
{
	profile_Id = eeprom_read_byte(&EE_profile_Id);
	readEEConfigProfile(profile_Id);
}

void readEEConfigProfile(uint8_t pr_Id)
{
	if (pr_Id >= MAX_PROFILES)
	{
		pr_Id = 0;
	}
	eeprom_read_block((void*)bm_cfg.ProfileName, (const void*)cfg[pr_Id].ProfileName, sizeof(bm_cfg.ProfileName));
	eeprom_read_block((void*)bm_cfg.AntShortNames , (const void*)cfg[pr_Id].AntShortNames , sizeof(bm_cfg.AntShortNames));
	eeprom_read_block((void*)bm_cfg.AntLongNames , (const void*)cfg[pr_Id].AntLongNames , sizeof(bm_cfg.AntLongNames));
	eeprom_read_block((void*)bm_cfg.AntOutCfg , (const void*)cfg[pr_Id].AntOutCfg , sizeof(bm_cfg.AntOutCfg));
	eeprom_read_block((void*)bm_cfg.BandOutCfg , (const void*)cfg[pr_Id].BandOutCfg , sizeof(bm_cfg.BandOutCfg));
	eeprom_read_block((void*)bm_cfg.AntAvailBandCfg , (const void*)cfg[pr_Id].AntAvailBandCfg , sizeof(bm_cfg.AntAvailBandCfg));
	eeprom_read_block((void*)bm_cfg.AntSelBandCfg , (const void*)cfg[pr_Id].AntSelBandCfg , sizeof(bm_cfg.AntSelBandCfg));
	bm_cfg.BandChngDelay = eeprom_read_byte(&(cfg[pr_Id].BandChngDelay));
	bm_cfg.BandOutDelay = eeprom_read_byte(&(cfg[pr_Id].BandOutDelay));
	bm_cfg.SameBandPttBlock = eeprom_read_byte(&(cfg[pr_Id].SameBandPttBlock));
	BandSelected = eeprom_read_byte(&EE_BandSelected);
	BandOutVal = BandSelected;
	BandDecode_Mode = eeprom_read_byte(&EE_BandDecode_Mode);
	
	// sanity check
	
	if (BandSelected > BAND_MAX)
	{
		BandSelected = BAND_160m;
	}

	for (uint8_t i = 0; i <= BAND_MAX; i++)
	{
		uint8_t hasAntennaForBand = 0;
		for (uint8_t j = 0; j < MAX_ANT_PER_BAND; j++)
		{
			if (bm_cfg.AntAvailBandCfg[i][j] >= MAX_ANT_TOTAL)
			{
				bm_cfg.AntAvailBandCfg[i][j] = NO_ANTENNA;
			}
			else
			{
				hasAntennaForBand = 1;
			}
		}

		if (bm_cfg.AntSelBandCfg[i] >= MAX_ANT_TOTAL)
		{
			bm_cfg.AntSelBandCfg[i] = bm_cfg.AntAvailBandCfg[i][0];
		}

		if (hasAntennaForBand == 0)
		{
			bm_cfg.AntSelBandCfg[i] = NO_ANTENNA;
		}
	}
}

// Update static config from RAM to EEPROM
void updateStaticEEConfig(uint8_t pr_Id)
{
	if (pr_Id >= MAX_PROFILES)
	{
		return;
	}
	eeprom_update_block((const void*)bm_cfg.ProfileName , (void*)cfg[pr_Id].ProfileName , sizeof(bm_cfg.ProfileName));
	eeprom_update_block((const void*)bm_cfg.AntShortNames , (void*)cfg[pr_Id].AntShortNames , sizeof(bm_cfg.AntShortNames));
	eeprom_update_block((const void*)bm_cfg.AntLongNames , (void*)cfg[pr_Id].AntLongNames , sizeof(bm_cfg.AntLongNames));
	eeprom_update_block((const void*)bm_cfg.AntOutCfg , (void*)cfg[pr_Id].AntOutCfg , sizeof(bm_cfg.AntOutCfg));
	eeprom_update_block((const void*)bm_cfg.BandOutCfg , (void*)cfg[pr_Id].BandOutCfg , sizeof(bm_cfg.BandOutCfg));
	eeprom_update_block((const void*)bm_cfg.AntAvailBandCfg , (void*)cfg[pr_Id].AntAvailBandCfg , sizeof(bm_cfg.AntAvailBandCfg));
	eeprom_update_byte(&(cfg[pr_Id].BandChngDelay), bm_cfg.BandChngDelay);
	eeprom_update_byte(&(cfg[pr_Id].BandOutDelay), bm_cfg.BandOutDelay);
	eeprom_update_byte(&(cfg[pr_Id].SameBandPttBlock), bm_cfg.SameBandPttBlock);
}

// Update current config (when band/antenna changed) from RAM to EEPROM
void updateCurrentEEConfig()
{
	eeprom_update_block((const void*)bm_cfg.AntSelBandCfg , (void*)cfg[profile_Id].AntSelBandCfg, sizeof(bm_cfg.AntSelBandCfg));
	eeprom_update_byte(&EE_BandDecode_Mode, BandDecode_Mode);
	eeprom_update_byte(&EE_BandSelected, BandSelected);
	eeprom_update_byte(&EE_profile_Id, profile_Id);
}

uint8_t isPeerBand()
{
	if (link_active == 0)
	{
		return 0;
	}
	if (bm_cfg.SameBandPttBlock == 0)
	{
		return 0;
	}
	if (PeerBand == BAND_INVALID)
	{
		return 0;
	}
	return BandSelected == PeerBand ? 1 : 0;
}

uint8_t isPeerAntConflict(uint8_t antId)
{
	if (link_active == 0)
	{
		return 0;
	}
	if (antId == NO_ANTENNA)
	{
		return 0;
	}
	return bm_cfg.AntOutCfg[antId] & PeerAntCfg;
}
