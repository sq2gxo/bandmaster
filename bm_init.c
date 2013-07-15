#include "bm_init.h"
#include "bm_serial.h"
#include "bm_uc_cfg.h"
#include "lcd.h"

#include <avr/io.h>
#include <avr/interrupt.h>


void initADC()
{
	// ADC config http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=56429

	// ADPS = 101 -> division 32 -> 4MHz/32 = 125kHz
	ADCSRA |= (1 << ADPS2) | (1 << ADPS0);
	// V REF = AVCC
	ADMUX |= (1 << REFS0); 
	// free running mode
	ADCSRA |= (1 << ADATE); 
	// limit result to 8bit
	ADMUX |= (1 << ADLAR); 
	// enable and start ADC
	ADCSRA |= (1 << ADEN); 
	ADCSRA |= (1 << ADSC);
}

void initPORTS()
{
    DDRB  &= 0b11100000; //PORTB 0-3 band data inputs, 4 PTT in
    PORTB |= 0b00011111; //Enable Pull-up on the input port
    
	DDRD  &= 0b00000011; //PORTD 2-7 switches
    PORTD |= 0b11111100; //Enable Pull-up on the input port

	// LED and ser reg out - Port A 2-7
	DDRA  |= 0b11111100;

	// PTT Block out - Port C 3
	DDRC  |= 0b00001000;
}

void initTIMER()
{
	TCCR1B |= (1 << WGM12 ); // Configure timer 1 for CTC mode
	TIMSK |= (1 << OCIE1A ); // Enable CTC interrupt
	sei (); // Enable global interrupts
	OCR1A = 49999; // Set CTC compare value to 100Hz at 4MHz AVR clock
	TCCR1B |= (1 << CS10 ); // Start timer at Fcpu

}


void initSERIAL()
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	// Turn on the transmission and reception circuitry
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Use 8-bit character sizes
	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRL = BAUD_PRESCALE; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	UCSRB |= (1 << RXCIE); // Enable the USART Recieve Complete interrupt (USART_RXC)
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
}
