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

#include "CLAction.h"
#include "..\Actors\CLAActor.h"

/**
*   Copy constructor
*       @param action The source action
*/
CLAction::CLAction(const CLAction& action)
{
    m_pActor   = action.m_pActor;
    m_Duration = action.m_Duration;
    m_bRunning = action.m_bRunning;
}

/**
*   Constructor that saves the action's duration and initializes the action
*       @param pActor The actor this action is controlling
*       @param duration How long this action should take
*/
CLAction::CLAction(float duration) :
    m_Duration(duration),
    m_bRunning(false),
    m_bDone(false)
{
}

