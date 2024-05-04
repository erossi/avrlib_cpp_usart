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

/*! \file
 *
 * Read a char from a serial port and send it on the other port.
 */

#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart_rxcbuffer.h"
#include "usart_rxcbuffer_p2.h"

Usart0_RxCBuffer usart0;
Usart1_RxCBuffer usart1;

int main() {
	uint8_t c;

	usart0.resume(); // setup the port
	usart1.resume(); // setup the port
	sei(); // enable IRQ

	while(true) {
		if (usart0.get(&c))
			usart1.put(c);

		if (usart1.get(&c))
			usart0.put(c);
	}

	return 0;
}
