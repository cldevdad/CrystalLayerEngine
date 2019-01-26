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

#include "CLActionMoveTo.h"
#include "..\Actors\CLAActor.h"
#include "..\Core\d_printf.h"

/**
*   Copy constructor
*       @param action The source CLActionMoveTo action
*/
CLActionMoveTo::CLActionMoveTo(const CLActionMoveTo& action)
    : CLAction(action)
{
    m_StartPosition = action.m_StartPosition;
    m_EndPosition   = action.m_EndPosition;
    m_Distance      = action.m_Distance;
}

/**
*   Constructor that takes an actor, horizontal and vertical position targets,
*   and the action duration.
*       @param x The horizontal destination
*       @param y The vertical destination
*       @param duration How long the action should take
*/
CLActionMoveTo::CLActionMoveTo(float x, float y, float duration) :
    CLAction(duration),
    m_StartPosition(CLPOS_ZERO),
    m_EndPosition({ x, y })
{
    
}

/**
*   Constructor that takes a CLPos and duration
*       @param position A target position
*       @param duration How long the action should take
*/
CLActionMoveTo::CLActionMoveTo(CLPos position, float duration) :
    CLAction(duration),
    m_StartPosition(CLPOS_ZERO),
    m_EndPosition(position)
{
}

/**
*   Destructor that finishes the action in case the action is destroyed
*   before finishing
*/
CLActionMoveTo::~CLActionMoveTo()
{
    m_bRunning = false;
    m_bDone = true;
}

/**
*   Starts the move action by setting the start and end positions, setting the actor's
*   velocity, and setting the action's running to true.
*/
void CLActionMoveTo::Start()
{
    // Set start and end positions
    if (m_pActor != nullptr)
    {
        // Set positions and distance
        m_StartPosition = m_pActor->GetPosition();
        m_EndPosition   = { m_EndPosition.x, m_EndPosition.y, m_pActor->GetRenderLayer() };
        m_Distance      = { m_EndPosition.x - m_StartPosition.x, m_EndPosition.y - m_StartPosition.y };
        m_bRunning      = true;

        // If the duration is 0 or less finish immediately
        if (m_Duration <= 0)
        {
            Finish();
        }
        else
        {
            CLVector2 NewVelocity = { (m_Distance.x / m_Duration), (m_Distance.y / m_Duration) };
            m_pActor->SetVelocity(NewVelocity);
        }
    }
    else
    {
        d_printf("[%s][ERROR!] Couldn't start action. Actor was null.\n", _FUNC);
    }
}

/**
*   Finishes the move action by setting running to false and done executing to true, stopping
*   the actor and setting its position to the target position (in case there were any last
*   second rounding errors)
*/
void CLActionMoveTo::Finish()
{
    m_pActor->SetVelocity(CLVECTOR_ZERO);
    m_pActor->SetPosition(m_EndPosition);

    m_bRunning = false;
    m_bDone = true;
}

/**
*   Updates the actors position based on its current position & velocity in
*   relation to the action's end position.
*       @param dt Time in seconds since previous update
*/
void CLActionMoveTo::Update(float dt)
{
    if (!IsRunning()) return;

    CLVector2 ActorVelocity = m_pActor->GetVelocity();
    CLPos     ActorPosition = m_pActor->GetPosition();

    // If the actor has reached or passed its target
    if (   (ActorVelocity.x < 0) && (ActorPosition.x <= m_EndPosition.x)  // Moving Left
        || (ActorVelocity.x > 0) && (ActorPosition.x >= m_EndPosition.x)  // Moving Right
        || (ActorVelocity.y < 0) && (ActorPosition.y <= m_EndPosition.y)  // Moving Up
        || (ActorVelocity.y > 0) && (ActorPosition.y >= m_EndPosition.y)) // Moving Down
    {
        // Done moving
        Finish();
    }
}

