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
#ifndef _INCLUDE_CLSURFACE_H_
#define _INCLUDE_CLSURFACE_H_

#include "..\Core\CLTypes.h"
#include "SDL.h"

class CLTexture;

/**
*   A software rendering surface that can be created from pixel data, an
*   image file, text, or another surface
*/
class CLSurface
{
    // Allow CLTexture access to the internal SDL_Surface
    friend class CLTexture;

public:
	DLLEXPORT CLSurface();    //!< Constructor
	DLLEXPORT ~CLSurface();   //!< Destructor

    //! Create a surface from existing pixel data
	DLLEXPORT void Create(void* pixels, float width, float height, int depth, int pitch, uint32_t rMask, uint32_t gMask, uint32_t bMask, uint32_t aMask);
    //! Create a surface from an image file
	DLLEXPORT void CreateFromFile(const char* imageFile);
    //! Create a surface from another surface
	DLLEXPORT void CreateFromSurface(const CLSurface* pSourceSurface);
    //! Create a surface from text
	DLLEXPORT void CreateFromText(const char* fontFile, float size, CLColor3 color, const char* text);
    //! Set the surface's color modulation multiplier
	DLLEXPORT void SetColorMod(CLColor3 color);

private:
    //! The internal SDL surface
    SDL_Surface* m_pSDLSurface;
};

#endif // _INCLUDE_CLSURFACE_H_