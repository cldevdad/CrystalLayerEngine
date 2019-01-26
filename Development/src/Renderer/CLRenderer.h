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
#ifndef _INCLUDE_CLRENDERER_H_
#define _INCLUDE_CLRENDERER_H_

#include "SDL.h"
#include "..\Core\CLTypes.h"

#define CLRENDERER_LAYERS_DEFAULT 9

class CLWindow;
class CLTexture;

/**
*   A rendering wrapper for SDL_Renderer. This is a singleton.
*/
class CLRenderer
{
public:

    // Our friend CLTexture needs to access our SDL renderer, but
    // nobody else should be able to get it. A public accessor would
    // expose it to objects that shouldn't mess with it.
    friend class CLTexture;

    //! Destructor
	DLLEXPORT ~CLRenderer();

    //! Attaches the renderer to a window
	DLLEXPORT void            AttachToWindow(CLWindow* pWindow);
    //! Returns the number of z layers
	DLLEXPORT uint8_t         GetLayerCount()  const { return m_LayerCount; }
    //! Returns the rendering screen size
	DLLEXPORT CLSize2D        GetScreenSize()  const { return m_ScreenSize; }
    //! Sets the number of z layers
	DLLEXPORT void            SetLayerCount(uint8_t count) { m_LayerCount = count; }
    //! Clears the rendering target
	DLLEXPORT void            Clear();
    //! Present the rendered scene
	DLLEXPORT void            Present();

private:

    //! Constructor
	DLLEXPORT CLRenderer();

    static CLRenderer*  m_pRenderer;    //!< The single renderer instance
    SDL_Renderer*       m_pSDLRenderer; //!< The internal SDL renderer
    CLSize2D            m_ScreenSize;   //!< The screen size in width and height
    uint8_t             m_LayerCount;   //!< The number of z layers

public:

    //! Returns the single static renderer instance
	DLLEXPORT
    static CLRenderer* GetRenderer()
    {
        if (m_pRenderer == nullptr)
        {
            m_pRenderer = new CLRenderer();
        }
        return m_pRenderer;
    }
};

#endif // _INCLUDE_CLRENDERER_H_