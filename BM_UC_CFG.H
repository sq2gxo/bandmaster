#ifndef __UC_CFG_H
#define __UC_CFG_H

#include <avr/io.h>

#define ANT1_SW_ID    0
#define ANT2_SW_ID    1
#define ANT3_SW_ID    2
#define MODE_SW_ID    3
#define DWN_SW_ID     4
#define UP_SW_ID      5
#define SW_COUNT      6

#define ANT1_SW_MASK    0x04
#define ANT2_SW_MASK    0x08
#define ANT3_SW_MASK    0x10
#define MODE_SW_MASK    0x20
#define DWN_SW_MASK     0x40
#define UP_SW_MASK      0x80


#define IS_SW_PRESSED(keymask) (PIND & keymask) == 0
#define IS_TX                  (PINB & 0b00010000) == 0

#define SW_OPEN      0
#define SW_PRESS     1
#define SW_HOLD      2

#endif
