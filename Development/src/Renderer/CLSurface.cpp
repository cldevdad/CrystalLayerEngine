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
#include "CLSurface.h"
#include "CLTexture.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "..\Core\d_printf.h"

/**
*   Constructor that initializes the internal SDL surface to null
*/
CLSurface::CLSurface()
    : m_pSDLSurface(nullptr)
{
    // Initialize SDL_image to create surfaces from image files
    int imageFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imageFlags) & imageFlags))
    {
        d_printf("[%s][ERROR!] Couldn't initialize SDL_image: SDL_image Error: %s\n", _FUNC, IMG_GetError());
        SDL_ClearError();
    }

    // Initialize SDL_ttf to create surfaces from text
    if (TTF_Init() < 0)
    {
        d_printf("[%s][ERROR!] Couldn't initialize SDL_TTF: TTF_Error: %s\n", _FUNC, TTF_GetError());
        SDL_ClearError();
    }
}

/**
*   Destructor that frees the internal SDL surface and quits SDL_ttf and SDL_image
*/
CLSurface::~CLSurface()
{
    if (m_pSDLSurface != nullptr)
    {
        SDL_FreeSurface(m_pSDLSurface);
        m_pSDLSurface = nullptr;
    }

    TTF_Quit();
    IMG_Quit();
}

/**
*   Creates a surface from existing pixel data by creating the internal SDL surface with that data.
*       /param pixels The existing pixel data
*       /param width Width of the surface
*       /param height Height of the surface
*       /param depth Depth of the surface in bits
*       /param pitch Length of a row of pixels in bytes
*       /param rMask Red mask for the pixels
*       /param gMask Green mask for the pixels
*       /param bMask Blue mask for the pixels
*       /param aMask Alpha mask for the pixels
*/
void CLSurface::Create(void* pixels, float width, float height, int depth, int pitch, 
                       uint32_t rMask, uint32_t gMask, uint32_t bMask, uint32_t aMask)
{
    // Cast dimensions to integers to create the SDL surface
    int WidthSDL = static_cast<int>(width);
    int HeightSDL = static_cast<int>(height);

    // Create the SDL surface
    m_pSDLSurface = SDL_CreateRGBSurfaceFrom(pixels, 
        WidthSDL, 
        HeightSDL, 
        depth, 
        pitch, 
        rMask, 
        gMask, 
        bMask, 
        aMask);

    if (m_pSDLSurface == nullptr)
    {
        d_printf("[%s][ERROR!] Couldn't create surface: SDL_Error: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
    }
}

/**
*   Creates a surface from an image file using the SDL_image library to create the 
*   internal SDL surface.
*       /param imageFile The image file name
*/
void CLSurface::CreateFromFile(const char* imageFile)
{
    // Don't re-create surface, which would cause a memory leak
    if (m_pSDLSurface != nullptr)
    {
        d_printf("[%s][ERROR!] Surface already created.\n", _FUNC);
        return;
    }

    // Load the image file onto the SDL surface
    m_pSDLSurface = IMG_Load(imageFile);
    
    if (m_pSDLSurface == nullptr)
    {
        d_printf("[%s][ERROR!] Couldn't create from file. SDL_Error: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
        return;
    }
}

/**
*   Creates a surface from another surface by blitting the contents of another surface
*   onto it.
*       /param pSourceSurface The source surface to copy from
*/
void CLSurface::CreateFromSurface(const CLSurface* pSourceSurface)
{
    // Don't re-create surface, which would cause a memory leak
    if (m_pSDLSurface != nullptr)
    {
        d_printerror("[%s][ERROR!] Surface already created.\n", _FUNC);
        return;
    }

    // Get a pointer to the source SDL surface
    SDL_Surface* pSourceSDLSurface = pSourceSurface->m_pSDLSurface;
    if (pSourceSDLSurface == nullptr)
    {
        d_printerror("[%s][ERROR!] Source surface invalid. SDL Error: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
        return;
    }

    // Create our SDL surface by copying the source SDL surface's format
    m_pSDLSurface = SDL_ConvertSurface(pSourceSDLSurface, pSourceSDLSurface->format, SDL_RLEACCEL);
    if (m_pSDLSurface == NULL)
    {
        d_printerror("[%s][ERROR!] Couldn't create SDL Surface. SDL Error: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
        return;
    }

    // Blit the source SDL surface onto our SDL surface
    int Result = SDL_BlitSurface(pSourceSurface->m_pSDLSurface, NULL, m_pSDLSurface, NULL);
    if (Result < 0)
    {
        d_printwarn("[%s][WARNING] Couldn't Blit surface. SDL_Error: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
    }
}

/**
*   Creates a surface from text and a font file along with font attributes. Uses the SDL_TTF library
*   to create the internal SDL surface.
*       /param fontFile The truetype font file to use
*       /param size Size of the text
*       /param color RGB color of the text
*       /param text The text to render
*/
void CLSurface::CreateFromText(const char* fontFile, float size, CLColor3 color, const char* text)
{
    // Don't re-create surface, which would cause a memory leak
    if (m_pSDLSurface != nullptr)
    {
        d_printerror("[%s][ERROR!] Surface already created.\n", _FUNC);
        return;
    }

    // Open the font
    TTF_Font* pTTFFont = TTF_OpenFont(fontFile, static_cast<int>(size));
    if (pTTFFont == nullptr)
    {
        d_printerror("[%s][ERROR!] Couldn't open TTF font \"%s\". TTF_Error: %s", _FUNC, fontFile, TTF_GetError());
        SDL_ClearError();
    }

    // Create the SDL surface by rendering with SDL_TTF
    SDL_Color SDLColor = { color.r, color.g, color.b };
    m_pSDLSurface = TTF_RenderText_Blended(pTTFFont, text, SDLColor);

    // Close the font
    TTF_CloseFont(pTTFFont);
}

/**
*   Sets a color modulation multiplier that will be multiplied into render operations
*       /param color An CLColor3 object with RGB values from 0-255
*/
void CLSurface::SetColorMod(CLColor3 color)
{
    if (m_pSDLSurface != nullptr)
    {
        // Set the SDL surface's color modulation multiplier
        int Result = SDL_SetSurfaceColorMod(m_pSDLSurface, color.r, color.g, color.b);
        if (Result < 0)
        {
            d_printerror("[%s][ERROR!] Couldn't set color mod. SDL Error: %s\n", _FUNC, SDL_GetError());
            SDL_ClearError();
        }
    }
}