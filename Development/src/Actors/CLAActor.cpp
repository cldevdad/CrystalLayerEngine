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
#include "..\Actors\CLAActor.h"
#include "..\Core\d_printf.h"
#include "..\Actions\CLActionMoveTo.h"
#include "..\Actions\CLActionMoveBy.h"
#include "..\Actions\CLActionSequence.h"

/*
*   Constructor that initializes the actor's members and creates its
*   surface and texture
*/
CLAActor::CLAActor() :
    m_Rotation(0.0),
    m_bAlive(true),
    m_Color({ 255,255,255 }),
    m_Id(0),
    m_Velocity(CLVECTOR_ZERO),
    m_Position(CLPOS_ZERO),
    m_RenderRect(CLRECT_ZERO),
    m_Scale(CLVECTOR_ONE),
    m_Lifespan(-1.f)
{
    m_pSurface = new CLSurface();
    m_pTexture = new CLTexture();

    m_pRenderer = CLRenderer::GetRenderer();
}

/*
*   Copy constructor that shallow copies most members and creates a new
*   surface and texture
*/
CLAActor::CLAActor(const CLAActor& actor)
{
    // Shallow copies
    m_bAlive         = actor.m_bAlive;
    m_Color          = actor.m_Color;
    m_Id             = actor.m_Id;
    m_Velocity       = actor.m_Velocity;
    m_Position       = actor.m_Position;
    m_pRenderer      = actor.m_pRenderer;
    m_RenderRect     = actor.m_RenderRect;
    m_Rotation       = actor.m_Rotation;
    m_Scale          = actor.m_Scale;
    m_Lifespan       = actor.m_Lifespan;

    // Copy surface
    m_pSurface = new CLSurface();
    m_pSurface->CreateFromSurface(actor.m_pSurface);

    // Copy texture
    m_pTexture = new CLTexture();
    m_pTexture->CreateFromSurface(m_pSurface, m_pRenderer);
}

/*
*   Destructor that frees the the actor
*/
CLAActor::~CLAActor()
{
    //StopAllActions();
    FreeActor();
}

/*
*   Returns true if this actor overlaps the actor passed in.
*   TODO: NEW DESCRIPTION
*       @param pActor The other actor
*/
bool CLAActor::OverlapsActor(CLAActor* pActor)
{
    CLRect MyRect = GetRect();
    SDL_Rect MySDLRect;
    MySDLRect.x = static_cast<int>(MyRect.x);
    MySDLRect.y = static_cast<int>(MyRect.y);
    MySDLRect.w = static_cast<int>(MyRect.w);
    MySDLRect.h = static_cast<int>(MyRect.h);

    CLRect TheirRect = pActor->GetRect();
    SDL_Rect TheirSDLRect;
    TheirSDLRect.x = static_cast<int>(TheirRect.x);
    TheirSDLRect.y = static_cast<int>(TheirRect.y);
    TheirSDLRect.w = static_cast<int>(TheirRect.w);
    TheirSDLRect.h = static_cast<int>(TheirRect.h);

    return (SDL_HasIntersection(&MySDLRect, &TheirSDLRect) == SDL_TRUE);
}

/**
*   Allocates and starts a CLAction on this actor
*       @param action The action to start
*/
void CLAActor::RunAction(CLAction& action)
{
    // If running a sequence we can't run another action
    if (dynamic_cast<CLActionSequence*>(&action) && IsRunningSequence())
    {
        return; 
    }

    // Allocate a new action.
    CLAction* pAction = action.NewCopy();
                                                        
    // If this is a Move action, stop any existing move actions
    if (dynamic_cast<CLActionMoveTo*>(&action) || dynamic_cast<CLActionMoveBy*>(&action))
    {
        StopAllMoveActions();
    }

    // Push this action onto the actor's current actions container
    pAction->SetActor(this);
    m_CurrentActions.push_back(pAction);
    pAction->Start();
}

/*
*   Updates the actor each frame
*       @param dt Delta time in seconds since the last update
*/
void CLAActor::Update(float dt)
{
    // Don't let the delta time get too large if the system got hung up
    if (dt > 0.25f) dt = 0.25f;

    if (!IsAlive())
    {
        return;
    }

    // Update lifespan if not infinite (-1)
    if (static_cast<int>(m_Lifespan) != -1)
    {
        if (m_Lifespan > 0)
        {
            m_Lifespan -= dt;
        }

        if (m_Lifespan < 0)
        {
            m_bAlive = false;
            return;
        }
    }

    // Update actions
    for(auto it = m_CurrentActions.begin(); it != m_CurrentActions.end(); )
    {
        CLAction* pAction = *it;

        if (pAction != nullptr)
        {
            if (pAction->IsDone())
            {
                // Free and remove from container
                delete pAction;
                pAction = nullptr;

                it = m_CurrentActions.erase(it);
            }
            else
            {
                // Update action
                pAction->Update(dt);

                if (!m_CurrentActions.empty())
                {
                    ++it;
                }
                else
                {
                    // Break if empty. When an actor has only one action, and it's a CLActionSequence ending with
                    // a CLAction callfunc, the actor's m_CurrentActions will be empty upon completion. Then execution
                    // will return back to this loop, where it will crash because m_CurrentActions doesn't even have
                    // a begin().
                    break;
                }
            }
        }
    }

    // Update actor's position
    SetPosition({ m_Position.x + (m_Velocity.x*dt), m_Position.y + (m_Velocity.y*dt), m_Position.z });
}

/*
*   Copies the actor's texture to the rendering target at its current location
*/
void CLAActor::Render()
{
    if (m_pTexture != nullptr)
    {
        m_pTexture->RenderCopy(m_RenderRect, m_Rotation, m_Scale);
    }
}

/*
*   Changes the actors scale
*/
void CLAActor::SetScale(CLVector2 scale)
{
    m_Scale = { scale.x, scale.y };
}

/*
*   Stops all running actions on this actor
*/
void CLAActor::StopAllActions()
{
    for (CLAction* pAction: m_CurrentActions)
    {
        if (pAction != nullptr)
        {
            delete pAction;
            pAction = nullptr;
        }
    }

    m_CurrentActions.clear();
}

/*
*   Stops all running MOVE actions on this actor
*/
void CLAActor::StopAllMoveActions()
{
    auto it = m_CurrentActions.begin();
    while (it != m_CurrentActions.end())
    {
        CLAction* pAction = *it;

        if (dynamic_cast<CLActionMoveTo*>(pAction) || dynamic_cast<CLActionMoveBy*>(pAction))
        {
            delete pAction;
            pAction = nullptr;
            it = m_CurrentActions.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

/*
*   Sets the actor's position and rendering rect
*/
void CLAActor::SetPosition(CLPos position)
{
    m_Position.x = m_RenderRect.x = position.x;
    m_Position.y = m_RenderRect.y = position.y;
    m_Position.z = position.z;
}

/*
*   Sets the color modulation that will be applied to the actor's surface
*   and texture
*       @param color A CLColor3 color with r,g,b from 0-255
*/
void CLAActor::SetColorMod(CLColor3 color)
{
    m_Color.r = color.r;
    m_Color.g = color.g;
    m_Color.b = color.b;
    m_pSurface->SetColorMod(m_Color);
    m_pTexture->SetColorMod(m_Color);
}

/*
*   Set the actor's alpha value for blending
*       @param alpha An alpha value from 0-255
*/
void CLAActor::SetAlpha(UINT8 alpha)
{
    m_pTexture->SetAlphaValue(alpha);
}

/*
*   Frees the actor's surface and texture memory
*/
void CLAActor::FreeActor()
{
    if (m_pTexture != nullptr)
    {
        delete m_pTexture;
        m_pTexture = nullptr;
    }

    if (m_pSurface != nullptr)
    {
        delete m_pSurface;
        m_pSurface = nullptr;
    }
}

/*
*   Sets the actors render rect dimensions
*/
void CLAActor::SetActorRenderRect(CLRect rect)
{
    m_RenderRect = rect;
}

/*
*   Returns true if the actor is running a sequence action
*/
bool CLAActor::IsRunningSequence()
{
    for (CLAction* pAction : m_CurrentActions)
    {
        if (dynamic_cast<CLActionSequence*>(pAction))
        {
            return true;
        }
    }

    return false;
}
