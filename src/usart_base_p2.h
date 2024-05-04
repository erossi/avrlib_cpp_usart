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

/*! \file
 * \brief RS232 - IO Base functions with 2 ports
 */

#ifndef _USART_BASE2_H_
#define _USART_BASE2_H_

#include <stdint.h>
#include "usart_base.h"

// For Usart1 duplicate the code with the proper registers:
class Usart1_Base {
	public:
		static void resume();
		static void suspend();
		static bool get(uint8_t*, const bool);
		static void put(const uint8_t);
};

#endif
