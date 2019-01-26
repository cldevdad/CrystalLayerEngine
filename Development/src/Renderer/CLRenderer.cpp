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

#include "CLRenderer.h"
#include "CLWindow.h"
#include "SDL.h"
#include "..\Core\CLTypes.h"
#include "..\Core\d_printf.h"

// Initialize the static renderer pointer
CLRenderer* CLRenderer::m_pRenderer = nullptr;

/**
*   Constructor that initializes the renderer's members.
*/
CLRenderer::CLRenderer() :
    m_LayerCount(CLRENDERER_LAYERS_DEFAULT),
    m_pSDLRenderer(nullptr),
    m_ScreenSize(CLSIZE_ZERO)
{
}

/**
*   Attaches the renderer to a window
*       @param window The window to render to
*/
void CLRenderer::AttachToWindow(CLWindow* pWindow)
{
    if (pWindow == nullptr)
    {
        d_printerror("[%s][ERROR!] Window is null.\n", _FUNC);
        return;
    }

    // Set renderer size to window size
    m_ScreenSize = pWindow->GetSize();

    // Destroy SDL renderer if it already exists
    if (m_pSDLRenderer != nullptr)
    {
        SDL_DestroyRenderer(m_pSDLRenderer);
        m_pSDLRenderer = nullptr;
    }
    
    // Create SDL renderer attached to window's SDL window
    int RenderDriver = -1;
    m_pSDLRenderer = SDL_CreateRenderer(pWindow->m_pSDLWindow, RenderDriver, SDL_RENDERER_ACCELERATED);

    if (m_pSDLRenderer == nullptr)
    {
        d_printerror("[%s][ERROR!] Couldn't create SDL Renderer: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
    }
    else
    {
        d_printgood("[%s] SDL Renderer initialized\n", _FUNC);
    }
}

/**
*   Destructor that destroys the internal SDL renderer
*/
CLRenderer::~CLRenderer()
{
    d_printfunc;
    if (m_pSDLRenderer != nullptr)
    {
        SDL_DestroyRenderer(m_pSDLRenderer);
        m_pSDLRenderer = nullptr;
    }
}

/**
*   Clears the rendering target in preparation for presenting a new scene
*/
void CLRenderer::Clear()
{
    SDL_RenderClear(m_pSDLRenderer);
}

/**
*   Presents everything rendered since the previous call
*/
void CLRenderer::Present()
{
    SDL_RenderPresent(m_pSDLRenderer);
}