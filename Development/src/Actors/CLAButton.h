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
#ifndef _INCLUDE_CLABUTTON_H_
#define _INCLUDE_CLABUTTON_H_
#include "CLAActor.h"
#include "CLASprite.h"

/**
*   A button actor that consists of 3 sprites for each state and a callback
*   for when the button is pressed
*/
class CLAButton : public CLAActor
{
public:
    //! Allows cloning through NewCopy()
	DLLEXPORT CL_CLONEABLE(CLAButton)
    //! Default Constructor
    DLLEXPORT CLAButton();

    //! Creates a button with normal, hover, and down state sprites
	DLLEXPORT void Create(CLASprite* normalSprite,
                CLASprite* hoverSprite, 
                CLASprite* downSprite, 
                CLPos      position, 
                void       (*pressFunction)());
    
	DLLEXPORT void Pressed() { m_OnPressFunction(); }

private:

    CLASprite* m_pNormalSprite;
    CLASprite* m_pHoverSprite;
    CLASprite* m_pDownSprite;

    void (*m_OnPressFunction)();
};

#endif // _INCLUDE_CLABUTTON_H_