/*
    USART - Serial port library.
    Copyright (C) 2005-2017 Enrico Rossi

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

#ifdef _USE_USART1_
#include "usart_base_p2.h"

Usart1_Base usart;

#else
#include "usart_base.h"

Usart0_Base usart;

#endif

int main() {
	uint8_t c;
	uint8_t hello[] {'h', 'e', 'l', 'l', 'o'};

	usart.resume();

	for (int i=0; i<5; i++)
		usart.put(hello[i]);

	while(true)
		if (usart.get(&c, true))
			usart.put(c);

	return 0;
}
