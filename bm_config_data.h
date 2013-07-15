#ifndef _EEPROM_DATA_H
#define _EEPROM_DATA_H

#include <avr/eeprom.h>
#include "bm_auto_band.h"
#define MAX_ANT_PER_BAND 3
#define MAX_ANT_TOTAL 10
#define MAX_PROFILES 1
#define ANT_SHORT_NAME_LEN 5
#define ANT_LONG_NAME_LEN 10
#define PROFILE_NAME_LEN 6

#define BAND_MODE_AUTO 1
#define BAND_MODE_MAN  2
#define NO_ANTENNA 99

// Static config - provisioned from PC:

typedef struct profile_config {
	// Profile name - station ID
	char ProfileName[PROFILE_NAME_LEN + 1];

	// Antenna short names (name len + terminating \0)
	char AntShortNames[MAX_ANT_TOTAL][ANT_SHORT_NAME_LEN + 1];

	// Antenna long names
	char AntLongNames[MAX_ANT_TOTAL][ANT_LONG_NAME_LEN + 1];

	// Antenna ANTOUT config bit config (12 lowest bit)
	uint16_t AntOutCfg[MAX_ANT_TOTAL];

	// Band config bit config (10 lowest bit)
	uint16_t BandOutCfg[BAND_MAX + 1];

	// Supported antenna ID per band (max 4 per band, ID = 99 -> not used )
	uint8_t AntAvailBandCfg[BAND_MAX + 1][MAX_ANT_PER_BAND];

	// Delay before setting BAND OUT value (sec) - PTT is locked during delay
	uint8_t BandOutDelay;

	// PTT block after band out change (sec)
	uint8_t BandChngDelay;

	// Block PTT on same band (0 - no, 1 yes)
	uint8_t SameBandPttBlock;

	//Current settings:

	// Last antenna seleted per band (0, 1 ... MAX_ANT_TOTAL - 1)
	uint8_t AntSelBandCfg[BAND_MAX + 1];

} bm_config_t;


// Current config
extern bm_config_t bm_cfg;

// Current profile ID
extern uint8_t profile_Id;

// Mode (AUTO /MANUAL)
extern uint8_t BandDecode_Mode;

// Manual band selected (0 - 9)
extern uint8_t BandSelected;

// BAND OUT value
extern uint8_t BandOutVal;

// Peer status
extern uint8_t PeerBand;
extern uint16_t PeerAntCfg;


// READ config from EEPROM to RAM
void readEEConfig(void);
void readEEConfigProfile(uint8_t pr_Id);

// Update static config from RAM to EEPROM
void updateStaticEEConfig(uint8_t pr_Id);

// Update current config (when band/antenna changed) from RAM to EEPROM
void updateCurrentEEConfig();


uint8_t isPeerBand();
uint8_t isPeerAntConflict(uint8_t antId);

#endif
