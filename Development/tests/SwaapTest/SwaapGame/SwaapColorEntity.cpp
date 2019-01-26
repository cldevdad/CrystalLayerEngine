/*
Swaap
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

#include "SwaapColorEntity.h"
#include <cstdlib>
#include <iostream>
#include "CrystalLayer.h"

/**
*   Picks a random SwaapColor and calls SetColor with it to set the entity's color
*/
void SwaapColorEntity::SetRandomColor()
{
    if (m_bCanChangeColor)
    {
        SwaapColor color = static_cast<SwaapColor>(1 + std::rand() / ((RAND_MAX + 1) / COLORS_MAX));
        SetColor(color);
    }
}

void SwaapColorEntity::BlockColorChanges(float duration)
{
    CLActionDelay    ActDelay(duration);
    CLActionCallFunc ActUnblock(std::bind(&SwaapColorEntity::AllowColorChanges, this));
    CLActionSequence ActTempBlock( {&ActDelay, &ActUnblock} );

    BlockColorChanges();
    this->m_pSprite->RunAction(ActTempBlock);
}

/**
*   Sets the entity's SwaapColor and applies an RGB color to the entity's sprite based on it
*/
void SwaapColorEntity::SetColor(SwaapColor color)
{ 
    if (m_bCanChangeColor)
    {
        m_Color = color;

        CLColor3 RGBColor = CLCOLOR_BLACK;
        switch (m_Color)
        {
            case ColorOrange: RGBColor = { 255, 130, 40 }; break;
            case ColorGreen:  RGBColor = { 60, 230, 70 }; break;
            case ColorBlue:   RGBColor = { 86, 130, 250 }; break;
            case ColorPurple: RGBColor = { 140, 100, 170 }; break;
        }

        m_pSprite->SetColorMod(RGBColor);
    }
}