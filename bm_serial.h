#ifndef _BM_SERIAL_H
#define _BM_SERIAL_H

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define SER_BUF_SIZE 50

#include <stdint.h>

typedef enum {
    DEVICE2DEVICE,
    PC_PROGRAM,
    PC_CONTROL,
} serial_mode_t;

typedef enum {
    DEVICE,
    PC,
    CONTROLLER,
	UNKNOWN,
	NUM_SENDER_TYPES
} msg_sender_t;

typedef enum {
    STATUS,
    CONFIG,
    READ_REQUEST,
	WRITE_REQUEST,
	OK,
	APPLY,
	INVALID,
	NUM_MSG_TYPES
} msg_type_t;

extern serial_mode_t serial_mode;

void doSerialInterrupt();

void serial_push_byte_out();

void serial_direct_send_byte(uint8_t byte);

void serial_clear_buffer();

void sendSerialMessage(const char* msg);

void reportBandAntSerial();

uint16_t byteToHex(uint8_t byte);

uint8_t getHexdigit(uint8_t data4bit);

void processSerialMsg();

void processDeviceMsg(msg_type_t msgType);

void processPCMsg(msg_type_t msgType);

uint8_t getValIdx(const unsigned char val, const unsigned char* arr, uint8_t size);

#endif
