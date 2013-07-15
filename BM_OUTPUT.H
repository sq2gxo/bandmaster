#ifndef _BM_OUTPUT_H
#define _BM_OUTPUT_H

#include <avr/io.h>

#define LED_AUTO_ID  0
#define LED_MAN_ID   1
#define LED_BLOCK_ID 2
#define LED_NUM      3

#define SD_DATA_ON  PORTA |= 0b00100000
#define SD_CLOCK_ON PORTA |= 0b01000000
#define SD_LATCH_ON PORTA |= 0b10000000

#define SD_DATA_OFF  PORTA &= 0b11011111
#define SD_CLOCK_OFF PORTA &= 0b10111111
#define SD_LATCH_OFF PORTA &= 0b01111111


void enableAutoLED();
void disableAutoLED();

void enableManLED();
void disableManLED();

void enableManLEDBlink();
void disableManLEDBlink();

void enableLinkLED();
void disableLinkLED();

void blockPTT();
void unblockPTT();
void blockPTTTimed(uint8_t sec);

void applyOutPins();

void applyPttStatus();

#endif
