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
#ifndef _INCLUDE_CLGAMEPAD_H
#define _INCLUDE_CLGAMEPAD_H

#include "SDL.h"
#include "../Core/CLTypes.h"

/**
*   A gamepad controller with analog, d-pad, select, start, A,B,X,Y,L1,L2,R1,R2
*/
class CLGamepad
{
public:

	DLLEXPORT CLGamepad(int index);
	DLLEXPORT ~CLGamepad();

private:

    SDL_GameController* m_pSDLController;
    int                 m_Index;

};

#endif // _INCLUDE_CLGAMEPAD_H