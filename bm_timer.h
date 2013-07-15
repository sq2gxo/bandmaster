#ifndef _BM_TIMER_H
#define _BM_TIMER_H

#include "bm_uc_cfg.h"

//#define SW_PRESS_TIME (50 / 10)
#define SW_PRESS_TIME (100 / 10)
#define SW_HOLD_TIME (2000 / 10)
#define LED_BLINK_TIME (300 / 10)

extern uint8_t sw_state[SW_COUNT];
extern uint8_t refresh_disp_needed;
extern uint8_t refresh_ptt_needed;
extern uint8_t refresh_bandout_needed;
extern uint8_t man_diode_blink;
extern uint8_t ptt_lock_timed;
extern uint8_t link_active;


void doTimerInterrupt();

void scheduleDispRefresh(uint8_t time_s);

void startPttLockTimed(uint8_t time_s);

void stopPttLockTimed();

void enableLinkTimer();

#endif
