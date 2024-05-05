/*
    USART - Serial port library.
    Copyright (C) 2020 Enrico Rossi

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

/** @file
 *
 * Read a char from a serial port and send it on the other port.
 */

#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifdef _USE_USART1_
#include "usart_rxcbuffer_p2.h"

Usart1_RxCBuffer usart; // IRQ global

#else
#include "usart_rxcbuffer.h"

Usart0_RxCBuffer usart; // IRQ global

#endif


int main() {
	uint8_t c[6] = {'h', 'e', 'l', 'l', 'o', '\n'};
	uint8_t i;

	usart.resume(); // setup the port
	sei(); // enable IRQ

	// print hello
	for (i = 0; i < 6; i++)
		usart.put(c[i]);

	while(true) {
		_delay_ms(5000); // act every 5sec.

		// try a single char
		if (usart.get(c))
			usart.put(c[0]);

		// get at most 4 char from the buffer
		i = usart.get(c, 4);

		for (uint8_t j = 0; j < i; j++)
			usart.put(c[j]);
	}

	return 0;
}
