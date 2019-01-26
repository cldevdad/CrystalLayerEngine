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
#ifndef _INCLUDE_CLACTIONSEQUENCE_H_
#define _INCLUDE_CLACTIONSEQUENCE_H_

#include "CLAction.h"
#include "..\Core\CLTypes.h"
#include "..\Core\CLActorPool.h"
#include <queue>

/**
*   An action that performs other actions in sequence
*/
class CLActionSequence : public CLAction
{
public:

    DLLEXPORT CL_CLONEABLE(CLActionSequence)                      //!< Allows cloning through NewCopy()
    DLLEXPORT CLActionSequence(const CLActionSequence& action);   //!< Copy constructor
	DLLEXPORT CLActionSequence(CLAActor* pActor);                 //!< Constructor that takes an actor
	DLLEXPORT ~CLActionSequence();                                //!< Destructor

    //! Constructor that takes a list of actions to perform in sequence
	DLLEXPORT CLActionSequence(std::initializer_list<CLAction*> actions, bool destroyActor = false, CLActorPool* pActorPool = nullptr);
    //! Adds an action to the sequence
	DLLEXPORT void AddAction(CLAction* pAction);

	DLLEXPORT void Start();           //!< Start the sequence action
	DLLEXPORT void Finish();          //!< Finish the sequence action
	DLLEXPORT void Update(float dt);  //!< Update the sequence action

private:
    
    std::deque<CLAction*>  m_ActionQueue; //!< The queue of actions to run in sequence
    bool                   m_bDestroyActor;
    CLActorPool*           m_pActorPool;   //!< The actor pool containing the actor, in case we need to destroy it
};

#endif // _INCLUDE_CLACTIONSEQUENCE_H_