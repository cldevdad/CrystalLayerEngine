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
#ifndef _INCLUDE_SWAAPCOLORENTITY_H_
#define _INCLUDE_SWAAPCOLORENTITY_H_

#include "CrystalLayer.h"
#include <string>

/**
*   All possible colors that a SwaapColorEntity can be
*/
enum SwaapColor
{
    ColorNone,
    ColorOrange,
    ColorGreen,
    ColorBlue,
    ColorPurple,
    COLORS_MAX = ColorPurple
};

/**
*   Base class for entities in the game that can change color such as
*   the player's ship and enemies
*/
class SwaapColorEntity
{
public:

    //! Constructor that sets the default color to none
    SwaapColorEntity() : m_Color(ColorNone), m_bCanChangeColor(true) {}

    //! Returns the entity's SwaapColor
    SwaapColor  GetColor() const { return m_Color; }
    //! Returns a pointer to the entity's sprite
    CLASprite*  GetSprite() const { return m_pSprite; }
    //! Sets the entity's color
    void        SetColor(SwaapColor color);
    //! Sets the entity to a random color
    void        SetRandomColor();
    //! Allows the entity to change colors
    void        AllowColorChanges() { m_bCanChangeColor = true; }
    //! Prevents the entity from changing colors
    void        BlockColorChanges() { m_bCanChangeColor = false; }
    //! Prevents color changes temporarily
    void        BlockColorChanges(float duration);
    
    //! Returns the string name of a color from the SwaapColor enum
    std::string GetColorName()
    {
        switch (m_Color)
        {
            case ColorOrange: return "Orange"; break;
            case ColorGreen:  return "Green"; break;
            case ColorBlue:   return "Blue"; break;
            case ColorPurple: return "Purple"; break;
            default: return "None"; break;
        }
    }

protected:

    //!< The entity's sprite
    CLASprite*  m_pSprite;

private:

    //!< The entity's color
    SwaapColor  m_Color;
    //!< If false then color changes have no effect
    bool        m_bCanChangeColor;
};

#endif // _INCLUDE_SWAAPCOLORENTITY_H_