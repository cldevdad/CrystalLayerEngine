/*
CLALabel (Actor)
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
#include "CLALabel.h"
#include "..\Core\d_printf.h"

using namespace std;

/**
*   Default constructor that calls CLAActor's default constructor and initializes
*/
CLALabel::CLALabel() : 
    CLAActor(),
    m_FontName(""), 
    m_FontSize(0), 
    m_bBlended(true)
{
}

/**
*   Takes a string, along with font and color attributes, and uses the SDL_ttf library to create
*   the actor's texture.
*       @param text The text to print on the label
*       @param font The font file
*       @param size The size of the font
*       @param color RGB text color
*       @param position The label's x,y position and z rendering depth
*       @param bBlended Use alpha blending
*/
void CLALabel::Create(const char* text, const char* font, float size, CLColor3 color, CLPos position, bool blended)
{
    // Open font
    char FontFullPath[512] = "";
    sprintf_s(FontFullPath, 512, "content/Fonts/%s", font);

    // Render text to surface
    CLSurface* pSurface = new CLSurface();
    pSurface->CreateFromText(const_cast<const char*>(FontFullPath), size, color, text);
    SetActorSurface(pSurface);

    if (GetSurface() == NULL)
    {
        d_printf("[%s][ERROR!] Couldn't create surface. SDL Error: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
        return;
    }

    // Create texture from surface
    CLTexture* pTexture = new CLTexture();
    pTexture->CreateFromSurface(pSurface, GetRenderer());
    SetActorTexture(pTexture);
    SetColorMod(color);
    if (GetTexture() == NULL)
    {
        d_printf("[%s][ERROR!] Couldn't create texture. SDL Error: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
        return;
    }
    
    // Get texture dimensions and set actor rectangle
    CLSize2D TextureSize = GetTexture()->GetSize();
    SetActorRenderRect({ position.x, position.y, TextureSize.w, TextureSize.h});

    // Save attributes so text can be changed on the fly
    m_FontName  = font;
    m_FontSize  = size;
    m_bBlended  = blended;
    SetPosition(position);
}

/**
*   Takes a number, along with font and color attributes, generates a string from the
*   number, and calls the string version of create
*       @param number The number to print on the label
*       @param font The font file
*       @param size The size of the font
*       @param color RGB text color
*       @param position The label's x,y position and z rendering depth
*       @param bBlended Use alpha blending
*/
void CLALabel::Create(const int number, const char* font, float size, CLColor3 color, CLPos position, bool blended)
{
    char Buffer[256] = { 0 };
    sprintf_s(Buffer, 256, "%i", number);
    Create(Buffer, font, size, color, position, blended);
}

/**
*   Changes text displayed by the label to a specified string
*       @param text The text to display on the label
*/
void CLALabel::ChangeText(const char* text)
{
    FreeActor();
    Create(text, m_FontName.c_str(), m_FontSize, GetColor(), GetPosition(), m_bBlended);
}

/**
*   Changes text displayed by the label to a specified integer number
*       @param number The number to display on the label
*/
void CLALabel::ChangeText(int number)
{
    char buffer[256];
    _itoa_s(number, buffer, sizeof(buffer), 10);
    ChangeText(buffer);
}