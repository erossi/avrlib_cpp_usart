/*
    USART - Serial port library.
    Copyright (C) 2005-2019 Enrico Rossi

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
#include <avr/interrupt.h>
#include <avr/io.h>
#include "usart_rxcbuffer_p2.h"

#ifdef ISR_USART1_RX
/*! Interrupt rx.
 *
 * \see ISR(USART0_RX_vect)
 */
ISR(USART1_RX_vect)
{
	uint8_t rxc;

	rxc = UDR1; // Get the char from the device
	Usart1_RxCBuffer::rxbuffer.push(rxc); // push it into the buffer

	// This part can be cut out if unused in your code.
	if (Usart1_RxCBuffer::eom_enable && (rxc == Usart1_RxCBuffer::eom))
		Usart1_RxCBuffer::eom_counter++;
}
#endif

/*! Out of class cbuffer constructor.
 *
 * \see CBuffer<uint8_t, uint8_t> Usart0_RxCBuffer::rxbuffer
 */
CBuffer<uint8_t, uint8_t> Usart1_RxCBuffer::rxbuffer;
uint8_t Usart1_RxCBuffer::eom { '\n' }; // EndOfMessage
uint8_t Usart1_RxCBuffer::eom_counter { 0 }; // Number of Message in the buffer
bool Usart1_RxCBuffer::eom_enable { false }; // Use the EOM

/*! Start the usart port.
 *
 * \see Usart0_RxCBuffer::resume()
 */
void Usart1_RxCBuffer::resume()
{
	rxbuffer.clear();

  // These are common to all 9600 bps settings.
  UCSR1A = (1 << U2X1);
  UBRR1H = 0;

#if F_CPU == 1000000UL
#if defined(U2X0)
  UBRR1L = 12;
#else
	UBRR1L = 6;
#endif
#elif F_CPU == 8000000UL
#if defined(U2X0)
  UBRR1L = 103;
#else
	UBRR1L = 51;
#endif
#elif F_CPU == 16000000UL
#if defined(U2X0)
  UBRR1L = 207;
#else
	UBRR1L = 103;
#endif
#error "CPU speed not supported"
#endif

	UCSR1C = (1 << UCSZ10) | (1 << UCSZ11); // 8n1

	// Rx with interrupt and Tx normal
	UCSR1B = (1 << RXCIE1) | (1 << RXEN1) | (1 << TXEN1);   // tx/rx
}

/*! Disable the usart port. */
void Usart1_RxCBuffer::suspend()
{
	Usart1_Base::suspend();
	rxbuffer.clear();
}

uint8_t Usart1_RxCBuffer::get(uint8_t *data, const uint8_t sizeofdata)
{
	return(rxbuffer.pop(data, sizeofdata));
}

/*! get the message from the RX buffer of a given maxsize.
 *
 * \see Usart0_RxCBuffer::getmsg
 */
uint8_t Usart1_RxCBuffer::getmsg(uint8_t *data, const size_t size)
{
	uint8_t n;

	n = rxbuffer.popm(data, size, eom);

	if (n && eom_counter)
		eom_counter--;

	return(n);
}

void Usart1_RxCBuffer::clear()
{
	rxbuffer.clear();
	eom_counter = 0; // clear the counter
}
