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
#include "CLTexture.h"
#include "SDL.h"
#include "..\core\d_printf.h"

/**
*   Constructor that initializes the internal SDL surface to null
*/
CLTexture::CLTexture()
    : m_pSDLTexture(nullptr)
{
}

/**
*   Destructor that frees the internal SDL texture
*/
CLTexture::~CLTexture()
{
    if (m_pSDLTexture != nullptr)
    {
        SDL_DestroyTexture(m_pSDLTexture);
        m_pSDLTexture = nullptr;
    }
}

/**
*   Returns the texture's alpha value from 0 - 255
*/
uint8_t CLTexture::GetAlphaValue()
{
    uint8_t AlphaValue = 0;

    int Result = SDL_GetTextureAlphaMod(m_pSDLTexture, &AlphaValue);
    if (Result < 0)
    {
        d_printerror("[%s][ERROR!] Couldn't get SDL texture alpha mod. SDL Error:%s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
    }

    return AlphaValue;
}

/**
*   Creates a texture from a CLSurface and assigns the texture's renderer.
*       @param pSourceSurface The source CLSurface to create from
*       @param pRenderer The renderer to attach to the texture
*/
void CLTexture::CreateFromSurface(CLSurface* pSurface, CLRenderer* pRenderer)
{
    // Set the renderer
    m_pRenderer = pRenderer;

    // Create the SDL texture from the source surface's SDL surface
    m_pSDLTexture = SDL_CreateTextureFromSurface(m_pRenderer->m_pSDLRenderer, pSurface->m_pSDLSurface);
    if (m_pSDLTexture == nullptr)
    {
        d_printerror("[%s][ERROR!] Couldn't create SDL texture. SDL Error: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
        return;
    }

    // Query the SDL texture's width and height
    int w = 0;
    int h = 0;
    if (SDL_QueryTexture(m_pSDLTexture, NULL, NULL, &w, &h) < 0)
    {
        d_printerror("[%s][ERROR!] Couldn't query SDL texture. SDL Error: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
        return;
    }

    // Set our width and height to the ones from the SDL texture
    m_Size = { static_cast<float>(w), static_cast<float>(h) };

    SDL_SetTextureBlendMode(m_pSDLTexture, SDL_BLENDMODE_BLEND);
}

/**
*   Queries the attributes of this texture by querying the attributes of the
*   internal SDL texture
*       /param format A pointer to store the type of pixel format (SDL_PixelFormatEnum)
*       /param access A pointer to store the type of texture access (SDL_TextureAccess)
*       /param width A pointer to store the texture's width
*       /param height A pointer to store the texture's height
*       /return Less than 0 on error
*/
int CLTexture::Query(uint32_t* format, int* access, float* width, float* height)
{
    int Result = 0;

    if (m_pSDLTexture != nullptr)
    {
        // Query the SDL texture. Format and access can be stored straight from the call
        // into the pointer arguments. However, the width and height will need to be casted later.
        int w = 0;
        int h = 0;
        if (SDL_QueryTexture(m_pSDLTexture, format, access, &w, &h) < 0)
        {
            d_printerror("[%s][ERROR!] Couldn't query SDL texture. SDL_Error: %s\n", _FUNC, SDL_GetError());
            SDL_ClearError();
            Result = -1;
        }
        else
        {
            // Store the width and height
            *width = static_cast<float>(w);
            *height = static_cast<float>(h);
        }
    }
    else
    {
        Result = -1;
        d_printerror("[%s][ERROR!] CLTexture is null\n", _FUNC);
    }
    
    return Result;
}

/**
*   Copies this texture to the renderer at a specified area.
*       /param rect The destination area on the renderer
*/
void CLTexture::RenderCopy(CLRect& rect, double angle, CLVector2 scale)
{
    SDL_Rect SDLRect;
    SDLRect.x = static_cast<int>(rect.x);
    SDLRect.y = static_cast<int>(rect.y);
    SDLRect.w = static_cast<int>(rect.w);
    SDLRect.h = static_cast<int>(rect.h);

    SDL_RenderSetScale(m_pRenderer->m_pSDLRenderer, scale.x, scale.y);
    SDL_RenderCopyEx(m_pRenderer->m_pSDLRenderer, m_pSDLTexture, NULL, &SDLRect, angle, NULL, SDL_FLIP_NONE);
    SDL_RenderSetScale(m_pRenderer->m_pSDLRenderer, 1.f, 1.f);
}

/**
*   Sets the texture's alpha value for rendering with transparency
*       /param alpha An alpha value from 0 - 255
*/
void CLTexture::SetAlphaValue(uint8_t alpha)
{
    SDL_SetTextureAlphaMod(m_pSDLTexture, alpha);
}

/**
*   Sets a color modulation multiplier that will be multiplied into render operations
*       /param color An CLColor3 object with RGB values from 0-255
*/
void CLTexture::SetColorMod(CLColor3 color)
{
    if (m_pSDLTexture != nullptr)
    {
        if (SDL_SetTextureColorMod(m_pSDLTexture, color.r, color.g, color.b) < 0)
        {
            d_printerror("[%s][ERROR!] Couldn't set SDL_Texture color mod. SDL Error:%s\n", _FUNC, SDL_GetError());
            SDL_ClearError();
        }
    }
}