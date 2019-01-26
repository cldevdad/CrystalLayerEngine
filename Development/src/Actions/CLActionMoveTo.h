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
#ifndef _INCLUDE_CLACTIONMOVETO_H_
#define _INCLUDE_CLACTIONMOVETO_H_

#include "CLAction.h"
#include "..\Core\CLTypes.h"

/**
*   An action that moves an actor to a target position
*/
class CLActionMoveTo : public CLAction
{
public:

	DLLEXPORT CL_CLONEABLE(CLActionMoveTo)                    //!< Allows cloning through NewCopy()
    DLLEXPORT CLActionMoveTo(const CLActionMoveTo& action);   //!< Copy constructor
	DLLEXPORT ~CLActionMoveTo();                              //!< Destructor

    //! Constructor that takes an x and y target and duration
	DLLEXPORT CLActionMoveTo(float x, float y, float duration);
    //! Constructor that takes a position target and duration
	DLLEXPORT CLActionMoveTo(CLPos position, float duration);

	DLLEXPORT void Start();           //!< Starts the move action
	DLLEXPORT void Finish();          //!< Finishes the move action
	DLLEXPORT void Update(float dt);  //!< Updates the move action
    
private:

    CLPos       m_StartPosition;    //!< The actor's position before moving
    CLPos       m_EndPosition;      //!< The actor's position after moving
    CLVector2   m_Distance;         //!< Distance the actor travels
};

#endif // _INCLUDE_CLACTIONMOVETO_H_