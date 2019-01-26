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
#ifndef _INCLUDE_CLASPRITE_H_
#define _INCLUDE_CLASPRITE_H_
#include "CLAActor.h"

/**
*   A sprite actor that can be loaded from an image file
*/
class CLASprite : public CLAActor
{
public:
    //! Allows cloning through NewCopy()
	DLLEXPORT CL_CLONEABLE(CLASprite)
    //! Default constructor
    DLLEXPORT CLASprite();

    //! Creates the actor's surface and texture from an image file
	DLLEXPORT void Create(const char* fileName, CLPos position, CLColor3 color = { 255,255,255 });
	DLLEXPORT void ChangeImage(const char* fileName);
};

#endif // _INCLUDE_CLASPRITE_H_