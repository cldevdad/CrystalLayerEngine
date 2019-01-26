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
#ifndef _INCLUDE_CLACTIONSCALETO_H_
#define _INCLUDE_CLACTIONSCALETO_H_

#include "CLAction.h"
#include "..\Core\CLTypes.h"

/**
*   An action that scales an actor to a target size
*/
class CLActionScaleTo : public CLAction
{
public:

	DLLEXPORT CL_CLONEABLE(CLActionScaleTo)                     //!< Allows cloning through NewCopy()
    DLLEXPORT CLActionScaleTo(const CLActionScaleTo& action);   //!< Copy constructor
	DLLEXPORT ~CLActionScaleTo();                               //!< Destructor

    //! Constructor that takes an target scale and duration
	DLLEXPORT CLActionScaleTo(CLVector2 scale, float duration);

	DLLEXPORT void Start();           //!< Starts the scale action
	DLLEXPORT void Finish();          //!< Finishes the scale action
	DLLEXPORT void Update(float dt);  //!< Updates the scale action
    
private:

    CLVector2 m_StartScale;
    CLVector2 m_EndScale;
    CLVector2 m_ScaleDelta;

    bool m_bScalingXUp;
    bool m_bScalingXDown;
    bool m_bScalingYUp;
    bool m_bScalingYDown;
};

#endif // _INCLUDE_CLACTIONSCALETO_H_