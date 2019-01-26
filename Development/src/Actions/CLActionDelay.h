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
#ifndef _INCLUDE_CLACTIONDELAY_H_
#define _INCLUDE_CLACTIONDELAY_H_

#include "..\Actions\CLAction.h"
#include "..\Core\CLTypes.h"

/**
*   An action that simply waits a specified period of time. This is useful
*   in a CLActionSequence when you want the actor to pause doing something
*   else.
*/
class CLActionDelay : public CLAction
{
public:
    
	DLLEXPORT CL_CLONEABLE(CLActionDelay)                      //!< Allows cloning through NewCopy()
    DLLEXPORT CLActionDelay(const CLActionDelay& action);      //!< Copy constructor
	DLLEXPORT ~CLActionDelay();                                //!< Destructor

	DLLEXPORT CLActionDelay(float duration); //!< Constructor that takes a duration

	DLLEXPORT void Start();           //!< Starts the delay action
	DLLEXPORT void Finish();          //!< Finishes the delay action
	DLLEXPORT void Update(float dt);  //!< Updates the delay action

private:

    float m_TimePassed;     //!< Time passed since delay started
};

#endif // _INCLUDE_CLACTIONDELAY_H_