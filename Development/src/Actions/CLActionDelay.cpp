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

#include "CLActionDelay.h"
#include "..\Actors\CLAActor.h"

/**
*   Copy constructor
*       @param action The source CLActionDelay action
*/
CLActionDelay::CLActionDelay(const CLActionDelay& action)
    : CLAction(action)
{
    m_TimePassed = action.m_TimePassed;
}

/**
*   Constructor that calls the CLAction constructor with the actor and duration,
*   then initializes the time passed to zero.
*       @param duration How long this action should take
*/
CLActionDelay::CLActionDelay(float duration) :
    CLAction(duration),
    m_TimePassed(0.f)
{
}

/**
*   Destructor that sets the action to not running and done executing
*/
CLActionDelay::~CLActionDelay()
{
    m_bRunning = false;
    m_bDone = true;
}

/**
*   Starts the delay action by setting the time passed to 0
*/
void CLActionDelay::Start()
{
    // If the duration is 0 or less just finish
    if (m_Duration <= 0)
    {
        Finish();
    }
    else
    {
        m_TimePassed = 0.f;
    }
}

/**
*   Finishes the delay action by setting running to false and done executing to true
*/
void CLActionDelay::Finish()
{
    m_TimePassed = 0.f;
    m_bRunning = false;
    m_bDone = true;
}

/**
*   Updates the delay action by incrementing the delay's time passed by 
*   the delta time since the previous update
*       @param dt Time in seconds since previous update
*/
void CLActionDelay::Update(float dt)
{
    m_TimePassed += dt;

    if (m_TimePassed >= m_Duration)
    {
        Finish();
    }
}

