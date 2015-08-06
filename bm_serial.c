#include <avr/io.h>
#include <util/atomic.h>
#include "bm_config_data.h"

#include "bm_serial.h"
#include "bm_timer.h"
#include "bm_display.h"
#include "bm_output.h"
#include "str_utils.h"

#include <stdio.h>

uint8_t needOutUpdate = 0;

serial_mode_t serial_mode = DEVICE2DEVICE;

const unsigned char msg_sender_val[NUM_SENDER_TYPES] =
{
    'D',  //DEVICE,
	'P',  //PC,
	'C',  //CONTROLLER,
	' '   //UNKNOWN,
};

const unsigned char msg_type_val[NUM_MSG_TYPES] =
{
    'S', //STATUS,
    'C', //CONFIG,
    'R', //READ_REQUEST,
	'W', //WRITE_REQUEST,
	'O', //OK,
	'A', //APPLY,
	' '  //INVALID,
};



uint8_t send_buf[SER_BUF_SIZE];
uint8_t send_buf_idx = 0;
uint8_t send_buf_size = 0;

uint8_t rcv_buf[SER_BUF_SIZE];
uint8_t rcv_buf_idx = 0;

uint8_t msg_buf[SER_BUF_SIZE];
uint8_t rcv_msg_ready = 0;

void doSerialInterrupt()
{
	char ReceivedByte;
	ReceivedByte = UDR;
	if (rcv_buf_idx >= SER_BUF_SIZE)
	{
		return;
	}
	rcv_buf[rcv_buf_idx] = ReceivedByte;
	rcv_buf_idx++;
	if (ReceivedByte == '\n')
	{
		if (rcv_msg_ready == 0)
		{
			// Copy
			for (uint8_t i = 0; i < rcv_buf_idx; i++)
			{
				msg_buf[i] = rcv_buf[i];
			}
			// replace \n with 0
			msg_buf[rcv_buf_idx - 1] = 0;
			rcv_msg_ready = 1;
			rcv_buf_idx = 0;
		}
	}

	// overflow scenario
	if (rcv_buf_idx >= SER_BUF_SIZE)
	{
		rcv_buf_idx = 0;
	}
}


// send one byte from buffer
void serial_push_byte_out()
{
	if (send_buf_size == 0)
	{
		// nothing to send
		return;
	}
	while ((UCSRA & (1 << UDRE)) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
	UDR = send_buf[send_buf_idx]; // send byte
	send_buf_idx++;
	if (send_buf_idx == send_buf_size)
	{
		// buffer was cleared, all sent
		send_buf_idx = 0;
		send_buf_size = 0;		
	}
}

void serial_direct_send_byte(uint8_t byte)
{
	while ((UCSRA & (1 << UDRE)) == 0) {}; // Do nothing until UDR is ready for more data to be written to it
	UDR = byte; // send byte
}

void serial_clear_buffer()
{
	do
	{
		serial_push_byte_out();
	}
	while (send_buf_size > 0);
}

void sendSerialMessage(const char* msg)
{
	// check size of msg to send
	uint8_t size = strLen(msg);

	// compare with buffer available
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		if (size < (SER_BUF_SIZE - send_buf_size))
		{
			// copy data to buffer
			for (uint8_t i = 0; i < size; i++)
			{
				send_buf[send_buf_size] = msg[i];
				send_buf_size++;
			}
		}
	}
}

void reportBandAntSerial()
{
	char msg[10];
	
	uint16_t antBits = 0;
	uint8_t antId = bm_cfg.AntSelBandCfg[BandSelected];
	if (antId != NO_ANTENNA)
	{
		antBits = bm_cfg.AntOutCfg[antId];
	}
	// 40m, ant bits 0x000000001000:
	// DS020008
	snprintf(msg, 10, "%c%c%02hhX%04hX\n",
		msg_sender_val[DEVICE],
		msg_type_val[STATUS],
		BandSelected,
		antBits);

	// only send in device to device mode
	if (serial_mode == DEVICE2DEVICE)
	{
		sendSerialMessage(msg);
	}
}


void processSerialMsg()
{
	if (rcv_msg_ready)
	{
		uint8_t len = strLen((char *)msg_buf);
		if (len < 2)
		{
			rcv_msg_ready = 0;
			return;
		}

		uint8_t msgValid = 1;
		// get sender
		msg_sender_t sender = getValIdx(msg_buf[0], msg_sender_val, sizeof(msg_sender_val));
		// get msg type
		msg_type_t msgType = getValIdx(msg_buf[1], msg_type_val, sizeof(msg_type_val));
		// process msg
		switch (sender)
		{
			case DEVICE:
				serial_mode = DEVICE2DEVICE;
				processDeviceMsg(msgType);
				break;
			case PC:
				serial_mode = PC_PROGRAM;
				processPCMsg(msgType);
				break;
			default:
				msgValid = 0;
				break;
		}

		// flag processing complete
		rcv_msg_ready = 0;
		if (msgValid)
		{
			ATOMIC_BLOCK(ATOMIC_FORCEON)
			{
				enableLinkTimer();
				link_active = 1;
			}
		}
		if (needOutUpdate)
		{
			applyPttStatus();
			displayStatus();
			needOutUpdate = 0;
		}
	}
}

void processDeviceMsg(msg_type_t msgType)
{
	if (msgType == STATUS)
	{
		// check size - should be total = 8
		uint8_t len = strLen((char *)msg_buf);
		if (len != 8)
		{
			return;
		}
		unsigned char band;
		uint16_t antCfg;
		// parse data
		uint8_t parsedCount = sscanf((char *)msg_buf, "DS%02hhX%04X\n", &band, &antCfg);
		if (parsedCount == 2 && band <= BAND_MAX)
		{
			// refresh output state (ptt lock) and display if peer status changed or peer just connected
			if ((PeerBand != band) || (PeerAntCfg != antCfg) || (link_active == 0))
			{
				needOutUpdate = 1;
			}
			PeerBand = band;
			PeerAntCfg = antCfg;
		}

	}
	
}

void processPCMsg(msg_type_t msgType)
{
	serial_clear_buffer();
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		enableLinkTimer();
		link_active = 1;

	}

	if (msgType == READ_REQUEST)
	{
		// check size - should be total = 2
		uint8_t len = strLen((char *)msg_buf);
		if (len != 2)
		{
			return;
		}

		// Disable device status reporting / interrupts		
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			//** send_buf array

			// send short antenna names
			for (uint8_t i = 0; i < MAX_ANT_TOTAL; i++)
			{
				//Device send data:
				//DC[p][t][idx][bbbbbbb]\n (variable len)
				//p - profile ID (hex 0 or 1)
				//t - config data type [N, S, A, B, P, L, T]
				//[idx] optional data index
				// TODO prepareCfgMsg(profile, DATA_ANT_SHORT_NAME, i, shortname);
			}

			// send confirmation
		}
	}
	else if (msgType == WRITE_REQUEST)
	{
	    // TODO
		// write current config to EEPROM
		updateStaticEEConfig(profile_Id);
		// TODO send response DO
	}
}

uint8_t getValIdx(const unsigned char val, const unsigned char* arr, uint8_t size)
{
	uint8_t i = 0;
	for (; i < size; i++)
	{
		if (arr[i] == val)
		{
			break;
		}
	}
	return i;
}
