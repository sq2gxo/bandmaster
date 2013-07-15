#ifndef _BM_DISPLAY_H
#define _BM_DISPLAY_H

#include <stdint.h>

void showSplashScreen();

void displayStatus();

void displayAntShort(uint8_t id, const char* name, const char* decor);

void displayMsg(const char* msg1, const char* msg2, uint8_t time_s);

#endif
