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

#include "CLActionCallFuncEx.h"
#include "..\Actors\CLAActor.h"

/**
*   Copy constructor. Shallow copies the callback, int user data, and
*   allocates the memory for void* to user data.
*       @param action The source CLActionCallFunc action
*/
CLActionCallFuncEx::CLActionCallFuncEx(const CLActionCallFuncEx& action)
    : CLAction(action)
{
    m_pVoidData     = action.m_pVoidData;
    m_FunctionCB    = action.m_FunctionCB;
    m_IntData       = action.m_IntData;
    m_bDone         = false;
}

/**
*   Constructor that initializes the action with a callback function, int user data, and
*   duration of zero. Allocates the memory for void* to user data.
*       @param function The function to call when this action is done
*       @param pData Pointer to user data
*       @param nData Integer user data
*/
CLActionCallFuncEx::CLActionCallFuncEx(CLVoidFunctionEx function, void* pData, int nData) :
    CLAction(0.f),
    m_FunctionCB(function),
    m_IntData(nData)
{
    m_pVoidData = pData;
}

/**
*   Destructor that sets the action to not running and done executing
*/
CLActionCallFuncEx::~CLActionCallFuncEx()
{
    m_bRunning = false;
    m_bDone = true;
}

/**
*   Starts the delay action by setting the time passed to 0
*/
void CLActionCallFuncEx::Start()
{
    m_bRunning = true;
    m_bDone = false;
}

/**
*   Finishes the delay action by setting running to false and done executing to true,
*   then calling the function
*/
void CLActionCallFuncEx::Finish()
{
    m_FunctionCB(m_pVoidData, m_IntData);
    m_bRunning = false;
    m_bDone = true;
}

/**
*   "Updates the action by just calling Finish, since all this action needs to do is
*   call a function and its done
*       @param dt Time in seconds since previous update
*/
void CLActionCallFuncEx::Update(float dt)
{
    if (!m_bDone)
    {
        Finish();
    }
}

