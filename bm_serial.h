#ifndef _BM_SERIAL_H
#define _BM_SERIAL_H

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define SER_TRANSMIT_BUF_SIZE 50
#define SER_RX_BUF_SIZE 530


#include <stdint.h>

typedef enum {
    DEVICE2DEVICE,
    PC_PROGRAM,
} serial_mode_t;

typedef enum {
    DEVICE,
    PC,
	NUM_SENDER_TYPES
} msg_sender_t;

typedef enum {
    STATUS,
    READ_REQUEST,
	WRITE_REQUEST,
	NUM_MSG_TYPES
} msg_type_t;

extern serial_mode_t serial_mode;

void doSerialInterrupt();

void serial_push_byte_out();

void serial_direct_send_byte(uint8_t byte);

void serial_clear_buffer();

void sendSerialMessage(const char* msg);

void reportBandAntSerial();

void processSerialMsg();

void processDeviceMsg(msg_type_t msgType);

void processPCMsg(msg_type_t msgType);

uint8_t getValIdx(const unsigned char val, const unsigned char* arr, uint8_t size);

#endif
