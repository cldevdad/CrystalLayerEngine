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

#include "CLActionFadeTo.h"
#include "..\Actors\CLAActor.h"
#include "..\Core\d_printf.h"

/**
*   Copy constructor
*       @param action The source CLAction action
*/
CLActionFadeTo::CLActionFadeTo(const CLActionFadeTo& action)
    : CLAction(action)
{
    m_u8StartAlpha    = action.m_u8StartAlpha;
    m_u8CurrentAlpha  = action.m_u8CurrentAlpha;
    m_u8EndAlpha      = action.m_u8EndAlpha;
    m_fCurrentAlpha   = action.m_fCurrentAlpha;
    m_FadeRate        = action.m_FadeRate;
    m_bFadeOut        = action.m_bFadeOut;
}

/**
*   Constructor that takes an actor, a target alpha value, and a fade duration
*       @param alpha The alpha value to fade to (0-255)
*       @param duration How long the fade should take
*/
CLActionFadeTo::CLActionFadeTo(uint8_t alpha, float duration) :
    CLAction(duration),
    m_fCurrentAlpha(0.f),
    m_u8EndAlpha(alpha)
{
    
}

/**
*   Destructor that sets the action to not running and done executing
*/
CLActionFadeTo::~CLActionFadeTo()
{
    m_bRunning = false;
    m_bDone = true;
}

/**
*   Starts the fade action by setting running to true
*/
void CLActionFadeTo::Start()
{
    if (m_pActor != nullptr)
    {
        m_u8CurrentAlpha = m_u8StartAlpha = m_pActor->GetAlpha();
        m_fCurrentAlpha  = static_cast<float>(m_u8CurrentAlpha);

        if (m_u8StartAlpha > m_u8EndAlpha)
        {
            // Fade OUT
            m_FadeRate = -static_cast<float>((m_u8StartAlpha - m_u8EndAlpha)) / m_Duration;
            m_bFadeOut = true;
        }
        else
        {
            // Fade IN
            m_FadeRate = static_cast<float>((m_u8EndAlpha - m_u8StartAlpha)) / m_Duration;
            m_bFadeOut = false;
        }

        m_bRunning = true;
        if (m_Duration <= 0)
        {
            Finish();
        }
    }
    else
    {
        d_printf("[%s][ERROR!] Couldn't start action. Actor was null.\n", _FUNC);
    }
}

/**
*   Finishes the fade action by setting running to false and done executing to true,
*   then setting the actor's alpha to the target alpha (in case there were any last
*   second rounding errors)
*/
void CLActionFadeTo::Finish()
{
    m_bRunning = false;
    m_bDone = true;
    m_pActor->SetAlpha(m_u8EndAlpha);
}

/**
*   Updates the actor's alpha value to the proper alpha value based on the
*   action's fade rate and delta time since previous update
*       @param dt Time in seconds since previous update
*/
void CLActionFadeTo::Update(float dt)
{
    if (m_bDone || m_pActor == nullptr)
    {
        return;
    }

    // Save the actor's current UINT8 alpha
    m_u8CurrentAlpha = m_pActor->GetAlpha();

    if (    m_bFadeOut && m_u8CurrentAlpha <= m_u8EndAlpha
        || !m_bFadeOut && m_u8CurrentAlpha >= m_u8EndAlpha)
    {
        Finish();
    }
    else
    {
        // Adjust the action's saved float alpha value (0.f-255.f) by the fade rate * delta
        const float FadeDelta    = m_FadeRate * dt;
        m_fCurrentAlpha         += FadeDelta;

        // If the ACTIONS float alpha is different than the actor's UINT8 alpha, then we have
        // changed another 1.0 and can now update the actor
        if(m_fCurrentAlpha != m_u8CurrentAlpha)
        {
            m_u8CurrentAlpha = static_cast<uint8_t>(m_fCurrentAlpha);
            m_pActor->SetAlpha(m_u8CurrentAlpha);
        }

    }
}

