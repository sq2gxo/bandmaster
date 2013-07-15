#ifndef _BM_AUTO_BAND_H
#define _BM_AUTO_BAND_H

#include "bm_uc_cfg.h"

#define BAND_160m    0
#define BAND_80m     1
#define BAND_40m     2
#define BAND_30m     3
#define BAND_20m     4
#define BAND_17m     5
#define BAND_15m     6
#define BAND_12m     7
#define BAND_10m     8
#define BAND_6m      9
#define BAND_MAX     9
#define BAND_INVALID 99

extern unsigned char current_auto_band;

void checkAutoBand();

#endif
