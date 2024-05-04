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
#include "usart_rxcbuffer.h"

#ifdef ISR_USART0_RX
/*! Interrupt rx.
 *
 * IRQ functions triggered every incoming char from the serial
 * ports.
 * If USARTn_EOL defined, every incoming USARTn_EOL chars increments
 * the message counter in the usartn struct.
 *
 * \bug if the buffer is full, the char is not inserted, but if it
 * is an EOM then the counter is incremented anyway leading to
 * having counted EOM superior to those actually present in the
 * buffer.
 *
 * @warning #define ISR_USART0_RX or write your own ISR call.
 */
ISR(USART0_RX_vect)
{
	uint8_t rxc;

	rxc = UDR0; // Get the char from the device
	Usart0_RxCBuffer::rxbuffer.push(rxc); // push it into the buffer

	// This part can be cut out if unused in your code.
	if (Usart0_RxCBuffer::eom_enable && (rxc == Usart0_RxCBuffer::eom))
		Usart0_RxCBuffer::eom_counter++;
}
#endif

/*! Out of class cbuffer constructor.
 *
 * Since we use a static class, allocate the space for
 * the buffer.
 */
CBuffer<uint8_t, uint8_t> Usart0_RxCBuffer::rxbuffer;
uint8_t Usart0_RxCBuffer::eom { '\n' }; // EndOfMessage
uint8_t Usart0_RxCBuffer::eom_counter { 0 }; // Number of Message in the buffer
bool Usart0_RxCBuffer::eom_enable { false }; // Use the EOM

/*! Start the usart port.
 *
 * Override the base resume, we need to use the RX IRQ.
 */
void Usart0_RxCBuffer::resume()
{
	rxbuffer.clear();

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

	// Rx with interrupt and Tx normal
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
}

/*! Disable the usart port. */
void Usart0_RxCBuffer::suspend()
{
	Usart0_Base::suspend();
	rxbuffer.clear();
}

uint8_t Usart0_RxCBuffer::get(uint8_t *data, const uint8_t sizeofdata)
{
	return(rxbuffer.pop(data, sizeofdata));
}

/*! get the message from the RX buffer of a given maxsize.
 *
 * Caller function should check that a message is present before.
 *
 * \param s the string to copy the message to.
 * \param size the sizeof(s).
 * \return the size of message retreived.
 * \note s must have the allocated size, safety termination is in place.
 */
uint8_t Usart0_RxCBuffer::getmsg(uint8_t *data, const size_t size)
{
	uint8_t n;

	n = rxbuffer.popm(data, size, eom);

	if (n && eom_counter)
		eom_counter--;

	return(n);
}

void Usart0_RxCBuffer::clear()
{
	rxbuffer.clear();
	eom_counter = 0; // clear the counter
}
