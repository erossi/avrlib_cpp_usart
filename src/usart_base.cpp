/*
    USART - Serial port library.
    Copyright (C) 2005-2021 Enrico Rossi

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA  02110-1301  USA
 */

#include <stdint.h>
#include <avr/io.h>
#include "usart_base.h"

/*! Start the usart port.
 *
 * See datasheet for more info, generally it is better to put fixed
 * parameter from the datasheet instead of calculating it.
 *
 * Default: 9600 bps
 *
 * if it is possible to improve baud rate error by using 2x clk
 *
 * @note Overload this methods to change speed/IRQ etc.

#if defined(U2X0)
  UCSR0A = (1 << U2X0);
	UBRR0H = (uint8_t)((F_CPU / 8UL / USART0_BAUD - 1) >> 8);
	UBRR0L = (uint8_t)(F_CPU / 8UL / USART0_BAUD - 1);
#else
	UBRR0H = (uint8_t)((F_CPU / 16UL / USART0_BAUD - 1) >> 8);
	UBRR0L = (uint8_t)(F_CPU / 16UL / USART0_BAUD - 1);
#endif

 * Speed: Clock 1Mhz - 9600 bps

#if defined(U2X0)
  UCSR0A = (1 << U2X0);
  UBRR0H = 0;
  UBRR0L = 12;
#else
	UBRR0H = 0;
	UBRR0L = 6;
#endif

 * Speed: Clock 8Mhz - 9600 bps

#if defined(U2X0)
  UCSR0A = (1 << U2X0);
  UBRR0H = 0;
  UBRR0L = 103;
#else
	UBRR0H = 0;
	UBRR0L = 51;
#endif

 * Speed: Clock 8Mhz - 115200 bps

#if defined(U2X0)
  UCSR0A = (1 << U2X0);
  UBRR0H = 0;
  UBRR0L = 8;
#else
	UBRR0H = 0;
	UBRR0L = 3;
#endif

 * Speed: Clock 16Mhz - 9600 bps

#if defined(U2X0)
  UCSR0A = (1 << U2X0);
  UBRR0H = 0;
  UBRR0L = 207;
#else
	UBRR0H = 0;
	UBRR0L = 103;
#endif

 * Speed: Clock 16Mhz - 115200 bps

#if defined(U2X0)
  UCSR0A = (1 << U2X0);
  UBRR0H = 0;
  UBRR0L = 16;
#else
	UBRR0H = 0;
	UBRR0L = 8;
#endif

 * example of initializations
 * Tx only without Rx
 * UCSR0B = _BV(TXEN0);
 * Tx and Rx only without interrupt
 * UCSR0B = _BV(TXEN0) | _BV(RXEN0);
 * Rx only with interrupt
 * UCSR0B = _BV(RXCIE0) | _BV(RXEN0);
 * Rx with interrupt and Tx normal
 * UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0);
 * Rx and Tx with interrupt
 * UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXCIE0) | _BV(TXEN0);
 */
void Usart0_Base::resume()
{
  // These are common to all 9600 bps settings.
  UCSR0A = (1 << U2X0);
  UBRR0H = 0;

#if F_CPU == 1000000UL
#if defined(U2X0)
  UBRR0L = 12;
#else
	UBRR0L = 6;
#endif
#elif F_CPU == 8000000UL
#if defined(U2X0)
  UBRR0L = 103;
#else
	UBRR0L = 51;
#endif
#elif F_CPU == 16000000UL
#if defined(U2X0)
  UBRR0L = 207;
#else
	UBRR0L = 103;
#endif
#error "CPU speed not supported"
#endif

	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // 8n1
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);   // tx/rx
}

/*! Disable the usart port. */
void Usart0_Base::suspend()
{
	UCSR0B = 0;
	/* I do not care about resetting all the parameters.
	   UCSR0C = 0;
	   UBRR0L = 0;
	   UCSR0A &= ~_BV(U2X0);
	   */
}

/*! Get a byte directly from the usart port. */
bool Usart0_Base::get(uint8_t* data, const bool locked)
{
	if (locked) {
		loop_until_bit_is_set(UCSR0A, RXC0);
		*data = UDR0;
		return(true);
	} else {
		if (bit_is_set(UCSR0A, RXC0)) {
			*data = UDR0;
			return(true);
		} else {
			return(false);
		}
	}
}

/*! Send character c down the USART Tx
 *
 * \parameter c the data to send.
 */
void Usart0_Base::put(const uint8_t c)
{
	// Wait for empty transmit buffer
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}
