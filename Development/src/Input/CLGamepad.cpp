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

#include "CLGamepad.h"
#include "..\Core\d_printf.h"

CLGamepad::CLGamepad(int index) : m_Index(index)
{
    if (index > SDL_NumJoysticks())
    {
        d_printerror("[%s][ERROR!] No gamepad connected at index %i", _FUNC, index);
        m_pSDLController = nullptr;
    }
    else
    {
        m_pSDLController = SDL_GameControllerOpen(m_Index);

        if (m_pSDLController == nullptr)
        {
            d_printerror("[%s][ERROR!] Couldn't open SDL game controller. SDL Error: %s\n", _FUNC, SDL_GetError());
            SDL_ClearError();
        }
    }
}

CLGamepad::~CLGamepad()
{
    if (m_pSDLController != nullptr)
    {
        SDL_GameControllerClose(m_pSDLController);
        m_pSDLController = nullptr;
    }
}