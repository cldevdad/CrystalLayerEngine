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

#include "CLActionMoveBy.h"
#include "..\Actors\CLAActor.h"
#include "..\Core\d_printf.h"

/**
*   Copy constructor
*       @param action The source CLActionMoveBy action
*/
CLActionMoveBy::CLActionMoveBy(const CLActionMoveBy& action)
    : CLAction(action)
{
    m_StartPosition = action.m_StartPosition;
    m_EndPosition   = action.m_EndPosition;
    m_Distance      = action.m_Distance;
}

/**
*   Constructor that takes an x and y movement distance, and duration
*       @param x The horizontal move distance
*       @param y The vertical move distance
*       @param duration How long the action should take
*/
CLActionMoveBy::CLActionMoveBy(float x, float y, float duration) :
    CLAction(duration),
    m_StartPosition(CLPOS_ZERO),
    m_EndPosition(CLPOS_ZERO),
    m_Distance({ x, y })
{
}

/**
*   Destructor that finishes the action in case the action is destroyed
*   before finishing
*/
CLActionMoveBy::~CLActionMoveBy()
{
    m_bRunning = false;
    m_bDone = true;
}

/**
*   Starts the move action by setting the start and end positions, setting the actor's
*   velocity, and setting the action's running to true.
*/
void CLActionMoveBy::Start()
{
    if (m_pActor != nullptr)
    {
        // Set start and end position
        m_StartPosition = m_pActor->GetPosition();
        m_EndPosition   = { m_StartPosition.x + m_Distance.x, m_StartPosition.y + m_Distance.y, m_pActor->GetRenderLayer() };
        m_bRunning      = true;

        // If the duration is 0 or less just finish
        if (m_Duration <= 0)
        {
            Finish();
        }
        else
        {
            CLVector2 NewVelocity = { (m_Distance.x / m_Duration), (m_Distance.y / m_Duration)};
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
void CLActionMoveBy::Finish()
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
void CLActionMoveBy::Update(float dt)
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

