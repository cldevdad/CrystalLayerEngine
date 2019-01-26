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
#include "CLAButton.h"
#include "..\Core\d_printf.h"

using namespace std;

/**
*   Default constructor that call's CLAActor's default constructor and initializes
*/
CLAButton::CLAButton() :
    CLAActor(),
    m_pNormalSprite(nullptr),
    m_pHoverSprite(nullptr),
    m_pDownSprite(nullptr)
{
}

/**
*   Creates a button from three sprites for a normal (idle) state, hover state, and down
*   (pressed) state, along with a callback function for when the button is pressed.
*       @param normalSprite Sprite to use for the button's idle state
*       @param hoverSprite Sprite to use for the button's hover state
*       @param downSprite Sprite to use for the button's pressed state
*       @param position The button's position on the scene
*       @param pressFunction Function to call when the button is pressed
*/
void CLAButton::Create(CLASprite* normalSprite, 
                       CLASprite* hoverSprite,
                       CLASprite* downSprite, 
                       CLPos      position, 
                       void       (*pressFunction)())
{
    m_pNormalSprite = normalSprite;
    m_pHoverSprite = hoverSprite;
    m_pDownSprite = downSprite;
    
    // Set this actor's position
    this->SetPosition(position);

    // TODO: Move sprites' positions to button position
}