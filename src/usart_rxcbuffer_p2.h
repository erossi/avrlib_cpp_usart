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

/*! \file usart_rxcbuffer.h
 * \brief RS232 - IO functions.
 *
 * \note: Use CBUF_SIZE to change the default size.
 */

#ifndef _USART_RXCBUFFER_P2_H_
#define _USART_RXCBUFFER_P2_H_

#include <stdint.h>
#include "usart_base_p2.h"
#include "avr_circular_buffer.h"

// For Usart1 duplicate the code with the proper registers:
class Usart1_RxCBuffer : public Usart1_Base {
        public:
		static CBuffer<uint8_t, uint8_t> rxbuffer;
		static uint8_t eom; // EndOfMessage
		static uint8_t eom_counter; // Number of Message in the buffer
		static bool eom_enable; // Use the EOM
		static void resume(); // override with IRQ RX
		static void suspend();
		static uint8_t get(uint8_t*, const uint8_t = 1);
		static uint8_t getmsg(uint8_t*, const size_t = 1);
		static void clear();
};

#endif
