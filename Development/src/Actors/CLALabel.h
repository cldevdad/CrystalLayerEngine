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
#ifndef _INCLUDE_CLALABEL_H_
#define _INCLUDE_CLALABEL_H_

#include "CLAActor.h"
#include <string>

/**
*   A label actor that displays text with a specified TTF font 
*/
class CLALabel : public CLAActor
{
public:
    //! Allows cloning through NewCopy()
	DLLEXPORT CL_CLONEABLE(CLALabel)
    //! Default constructor
    DLLEXPORT CLALabel();

    //! Creates the actor's texture and surface from a string and font
	DLLEXPORT void Create(const char* text,
                          const char* font,
                          float       size,
                          CLColor3    color,
                          CLPos       position,
                          bool        blended = true);

    //! Creates the actor's texture and surface from an integer and font
	DLLEXPORT void Create(const int   number,
                          const char* font,
                          float       size,
                          CLColor3    color,
                          CLPos       position,
                          bool        blended = true);

    //! Changes label's text to a specified string
	DLLEXPORT void ChangeText(const char* text);
    //! Changes label's text to a specified integer
	DLLEXPORT void ChangeText(int number);

private:
    std::string m_FontName; //!< Name of the font file
    float       m_FontSize; //!< Font size
    bool        m_bBlended; //!< Use texture blending
};

#endif // _INCLUDE_CLALABEL_H_