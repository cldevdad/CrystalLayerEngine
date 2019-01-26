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

#include "CLActionCallFunc.h"
#include "..\Actors\CLAActor.h"

/**
*   Copy constructor
*       @param action The source CLActionCallFunc action
*/
CLActionCallFunc::CLActionCallFunc(const CLActionCallFunc& action)
    : CLAction(action)
{
    m_FunctionCB = action.m_FunctionCB;
    m_bDone      = false;
}

/**
*   Constructor that initializes the action with a callback function and
*   duration of zero.
*       @param function The function to call when this action is done
*/
CLActionCallFunc::CLActionCallFunc(CLVoidFunction function) :
    CLAction(0.f),
    m_FunctionCB(function)
{
}

/**
*   Destructor that sets the action to not running and done executing
*/
CLActionCallFunc::~CLActionCallFunc()
{
    m_bRunning = false;
    m_bDone = true;
}

/**
*   Starts the delay action by setting the time passed to 0
*/
void CLActionCallFunc::Start()
{
    m_bRunning = true;
    m_bDone = false;
}

/**
*   Finishes the delay action by setting running to false and done executing to true,
*   then calling the function
*/
void CLActionCallFunc::Finish()
{
    m_FunctionCB();
    m_bRunning = false;
    m_bDone = true;
}

/**
*   "Updates the action by just calling Finish, since all this action needs to do is
*   call a function and its done
*       @param dt Time in seconds since previous update
*/
void CLActionCallFunc::Update(float dt)
{
    if (!m_bDone)
    {
        Finish();
    }
}

