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
#ifndef _INCLUDE_CLACTION_H_
#define _INCLUDE_CLACTION_H_

#include "..\Core\CLTypes.h"

class CLAActor;

/**
*   Base class for an action that can be performed on an actor
*/
class CLAction
{
public:
        
        DLLEXPORT CL_CLONEABLE_BASE(CLAction)         //!< Allows cloning through NewCopy()
        DLLEXPORT CLAction(const CLAction& action);   //!< Copy constructor
		DLLEXPORT virtual ~CLAction() {};             //!< Destructor

		DLLEXPORT virtual void    Pause()  { m_bRunning = false; } //!< Pause action
		DLLEXPORT virtual void    Resume() { m_bRunning = true; }  //!< Resume action

		DLLEXPORT float     GetDuration() const { return m_Duration; }        //!< Returns this action's running duration
		DLLEXPORT bool      IsDone()      const { return m_bDone; }           //!< Returns true if this action is done executing
		DLLEXPORT bool      IsRunning()   const { return m_bRunning; }        //!< Returns true if action is running
		DLLEXPORT CLAActor* GetActor()    const { return m_pActor; }          //!< Gets the action's actor
		DLLEXPORT void      SetActor(CLAActor* pActor) { m_pActor = pActor; } //!< Sets the action's actor

		virtual void Start() = 0;          //!< Start action
		virtual void Update(float dt) = 0; //!< Update action
		virtual void Finish() = 0;         //!< Finish action
        
protected:
        
        DLLEXPORT CLAction() {};              //!< Default constructor
		DLLEXPORT CLAction(float duration);   //!< Constructor that takes the action's duration
        
        CLAActor* m_pActor;         //!< The actor this action is controlling
        float     m_Duration;       //!< This action's total duration. Do not change once set!             
        bool      m_bRunning;       //!< Whether or not this action is running
        bool      m_bDone;          //!< Whether or not this action is done executing
};

#endif // _INCLUDE_CLACTION_H_