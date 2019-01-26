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
#ifndef _INCLUDE_CLTEXTURE_H_
#define _INCLUDE_CLTEXTURE_H_

#include "..\Core\CLTypes.h"
#include "SDL.h"
#include "CLSurface.h"
#include "CLRenderer.h"

/**
*   A GPU rendering texture that can be created from a software CLSurface
*/
class CLTexture
{
public:
    //! Constructor
	DLLEXPORT CLTexture();
    //! Destructor
	DLLEXPORT ~CLTexture();

    //! Create a texture from a software surface
	DLLEXPORT void CreateFromSurface(CLSurface* pSurface, CLRenderer* pRenderer);
    //! Copy this texture to the renderer
	DLLEXPORT void RenderCopy(CLRect& rect, double angle, CLVector2 scale = CLVECTOR_ONE);
    
    //! Returns the texture's alpha value
	DLLEXPORT uint8_t     GetAlphaValue();
    //! Returns a pointer to the texture's renderer
	DLLEXPORT CLRenderer* GetRenderer() const { return m_pRenderer; }
    //! Returns the textures size
	DLLEXPORT CLSize2D    GetSize() const { return m_Size; }
    //! Query texture's attributes
	DLLEXPORT int         Query(uint32_t* format, int* access, float* width, float* height);
    //! Set the texture's alpha value from 0-255
	DLLEXPORT void        SetAlphaValue(uint8_t alpha);
    //! Set the texture's color modulation multiplier
	DLLEXPORT void        SetColorMod(CLColor3 color);

private:
    SDL_Texture*    m_pSDLTexture; //!< Internal SDL texture
    CLRenderer*     m_pRenderer;   //!< Renderer to copy texture to
    CLSize2D        m_Size;        //!< Texture's size
};

#endif // _INCLUDE_CLTEXTURE_H_