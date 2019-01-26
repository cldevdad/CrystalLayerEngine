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

#include "CLActionScaleTo.h"
#include "..\Actors\CLAActor.h"
#include "..\Core\d_printf.h"

/**
*   Copy constructor
*       @param action The source CLActionScaleTo action
*/
CLActionScaleTo::CLActionScaleTo(const CLActionScaleTo& action)
    : CLAction(action)
{
    m_StartScale    = action.m_StartScale;
    m_EndScale      = action.m_EndScale;
    m_ScaleDelta    = action.m_ScaleDelta;
    m_bScalingXUp   = action.m_bScalingXUp;
    m_bScalingXDown = action.m_bScalingXDown;
    m_bScalingYUp   = action.m_bScalingYUp;
    m_bScalingYDown = action.m_bScalingYDown;
}

/**
*   Constructor that takes a target scale and duration
*       @param scale The value to scale to
*       @param duration How long the action should take
*/
CLActionScaleTo::CLActionScaleTo(CLVector2 scale, float duration) :
    CLAction(duration),
    m_StartScale(CLVECTOR_ZERO),
    m_EndScale(scale),
    m_bScalingXUp(false),
    m_bScalingXDown(false),
    m_bScalingYUp(false),
    m_bScalingYDown(false)
{
}

/**
*   Destructor that finishes the action in case the action is destroyed
*   before finishing
*/
CLActionScaleTo::~CLActionScaleTo()
{
    m_bRunning = false;
    m_bDone = true;
}

/**
*   Starts the scale action by setting the start and end values, setting the actor's
*   velocity, and setting the action's running to true.
*/
void CLActionScaleTo::Start()
{
    // Set start and end scale
    if (m_pActor != nullptr)
    {
        m_bRunning = true;
        m_bDone = false;

        CLVector2 ActorScale = m_pActor->GetScale();
        m_StartScale.x = ActorScale.x;
        m_StartScale.y = ActorScale.y;

        // X Scale up
        if (m_EndScale.x > m_StartScale.x)
        {
            m_ScaleDelta.x = (m_EndScale.x - m_StartScale.x) / m_Duration;
            m_bScalingXUp = true;
        }
        // X Scale down
        else if (m_EndScale.x < m_StartScale.x)
        {
            m_ScaleDelta.x = (m_StartScale.x - m_EndScale.x) / m_Duration;
            m_ScaleDelta.x *= -1.f;
            m_bScalingXDown = true;
        }

        // Y Scale up
        if (m_EndScale.y > m_StartScale.y)
        {
            m_ScaleDelta.y = (m_EndScale.y - m_StartScale.y) / m_Duration;
            m_bScalingYUp = true;
        }
        // Y Scale down
        else if (m_EndScale.y < m_StartScale.y)
        {
            m_ScaleDelta.y = (m_StartScale.y - m_EndScale.y) / m_Duration;
            m_ScaleDelta.y *= -1.f;
            m_bScalingYDown = true;
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
void CLActionScaleTo::Finish()
{
    m_pActor->SetScale(m_EndScale);

    m_bRunning = false;
    m_bDone = true;
}

/**
*   Updates the actors position based on its current position & velocity in
*   relation to the action's end position.
*       @param dt Time in seconds since previous update
*/
void CLActionScaleTo::Update(float dt)
{
    if (!IsRunning() || m_pActor == nullptr)
    {
        return;
    }

    CLVector2 CurrentScale = m_pActor->GetScale();
    bool bXDone = (!m_bScalingXDown && !m_bScalingXUp || m_bScalingXUp && CurrentScale.x >= m_EndScale.x || m_bScalingXDown && CurrentScale.x <= m_EndScale.x);
    bool bYDone = (!m_bScalingYDown && !m_bScalingYUp || m_bScalingYUp && CurrentScale.y >= m_EndScale.y || m_bScalingXDown && CurrentScale.y <= m_EndScale.y);

    if (bXDone && bYDone)
    {
        Finish();
        return;
    }

    if (!bXDone)
    {
        m_pActor->SetScale({ CurrentScale.x + (m_ScaleDelta.x * dt), CurrentScale.y });
    }
    if (!bYDone)
    {
        CurrentScale = m_pActor->GetScale();
        m_pActor->SetScale({ CurrentScale.x, CurrentScale.y + (m_ScaleDelta.y * dt) });
    }
}

