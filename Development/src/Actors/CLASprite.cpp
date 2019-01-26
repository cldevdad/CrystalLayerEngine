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
#include "CLASprite.h"
#include "..\Core\d_printf.h"

using namespace std;

/**
*   Default constructor that calls CLAActor's default constructor
*/
CLASprite::CLASprite() :
    CLAActor()
{
}

/**
*   Takes an image file name and position and creates the actor's surface and texture
*       @param fileName The image file name
*       @param position The sprite's x and y position 
*       @param color    The sprite's color
*/
void CLASprite::Create(const char* fileName, CLPos position, CLColor3 color)
{
    // Create full path to sprite file
    char FileFullPath[512] = "";
    sprintf_s(FileFullPath, 512, "content/Sprites/%s", fileName);

    // Load file and set surface
    CLSurface* pSurface = new CLSurface();
    pSurface->CreateFromFile(const_cast<const char*>(FileFullPath));
    SetActorSurface(pSurface);

    if (GetSurface() == NULL)
    {
        d_printf("[%s][ERROR!] Couldn't create surface. SDL Error: %s\n", _FUNC, SDL_GetError());
        return;
    }

    // Set texture from surface and apply color
    CLTexture* pTexture = new CLTexture();
    pTexture->CreateFromSurface(pSurface, GetRenderer());
    SetActorTexture(pTexture);
    SetColorMod(color);

    if (GetTexture() == NULL)
    {
        d_printf("[%s][ERROR!] Couldn't create texture. SDL Error: %s\n", _FUNC, SDL_GetError());
        return;
    }

    // Set this actor's position
    SetPosition(position);

    // Set this actor's render rect
    CLSize2D TextureSize = this->GetTexture()->GetSize();
    SetActorRenderRect({ position.x, position.y, TextureSize.w, TextureSize.h });
}

/**
*   Change the sprite's image to another image by erasing and re-creating the actor
*       @param fileName The filename of the image to change to
*/
void CLASprite::ChangeImage(const char* fileName)
{
    FreeActor();
    Create(fileName, { GetRect().x, GetRect().y }, GetColor());
}