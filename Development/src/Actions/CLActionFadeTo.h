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
#ifndef _INCLUDE_CLACTIONFADETO_H_
#define _INCLUDE_CLACTIONFADETO_H_

#include "..\Actions\CLAction.h"
#include "..\Core\CLTypes.h"

/**
*   An action that fades an actor to a target alpha
*/
class CLActionFadeTo : public CLAction
{
public:

	DLLEXPORT CL_CLONEABLE(CLActionFadeTo)                   //!< Allows cloning through NewCopy()
    DLLEXPORT CLActionFadeTo(const CLActionFadeTo& action);  //!< Copy constructor
	DLLEXPORT ~CLActionFadeTo();                             //!< Destructor

    //! Constructor that takes a target opacity and fade duration
	DLLEXPORT CLActionFadeTo(uint8_t opacity, float duration);

	DLLEXPORT void Start();           //!< Starts the fade action
	DLLEXPORT void Finish();          //!< Finishes the fade action
	DLLEXPORT void Update(float dt);  //!< Updates the fade action
    
private:

    // For clarity, violate the coding standard and use prefixes
    // here so it's clear in the implementation whether an integer
    // or float is being used.

    uint8_t m_u8StartAlpha;   //!< The alpha before fading (0-255)
    uint8_t m_u8CurrentAlpha; //!< The alpha value each frame (0-255)
    uint8_t m_u8EndAlpha;     //!< The target alpha value (0-255)

    float m_fCurrentAlpha;    //!< The action's current alpha (0.f - 255.f)
    float m_FadeRate;         //!< Rate of fade (alpha per second)
    bool  m_bFadeOut;         //!< True if actor is fading out
};

#endif // _INCLUDE_CLACTIONFADETO_H_