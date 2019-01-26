/*
Crystal Layer
Copyright (C) 2018 Colin Payette <colin@cpayette.com>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "CLEvent.h"
#include "d_printf.h"
#include "SDL.h"

CLEvent::CLEvent() : m_Type(CL_FIRSTEVENT), m_KeyCode(CLK_UNKNOWN)
{
}

CLEvent::~CLEvent()
{
}

void CLEvent::Poll()
{
	SDL_PollEvent(&m_SDLEvent);
	uint32_t typeCode = static_cast<uint32_t>(m_SDLEvent.type);
	m_Type = static_cast<CLEventType>(typeCode);

	if (m_Type == CL_KEYDOWN || m_Type == CL_KEYUP)
	{
		uint32_t keySymbol = m_SDLEvent.key.keysym.sym;
		m_KeyCode = static_cast<CLKeyCode>(keySymbol);
	}
}
