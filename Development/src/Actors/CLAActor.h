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
#ifndef _INCLUDE_CLACTOR_H_
#define _INCLUDE_CLACTOR_H_

#include "..\Core\CLTypes.h"
#include "..\Renderer\CLSurface.h"
#include "..\Renderer\CLTexture.h"
#include "..\Actions\CLAction.h"
#include <vector>

/**
*   An actor is anything that can be placed and rendered in the scene.
*/
class CLAActor
{
public:
    //! Allows cloning through NewCopy()
    DLLEXPORT CL_CLONEABLE_BASE(CLAActor)

    //! Destructor
    DLLEXPORT virtual ~CLAActor();             

    //! Updates actor
	DLLEXPORT virtual void Update(float dt);
    //! Renders actor
	DLLEXPORT virtual void Render();

    //! Returns true if this actor collides with a given actor
	DLLEXPORT bool OverlapsActor(CLAActor* pActor);
    //! Makes the actor no longer alive
	DLLEXPORT void Kill() { m_bAlive = false; }
    //! Runs a CLAction on this actor
	DLLEXPORT void RunAction(CLAction& pAction);
    //! Sets the actor's alpha value from 0-255
	DLLEXPORT void SetAlpha(uint8_t alpha);
    //! Sets the actor's color
	DLLEXPORT void SetColorMod(CLColor3 color);
    //! Sets the actor's unique identifier
	DLLEXPORT void SetId(uint32_t id) { m_Id = id; }
    //! Sets actor's lifespan
	DLLEXPORT void SetLifespan(float duration) { m_Lifespan = duration; }
    //! Sets actor's position
	DLLEXPORT void SetPosition(CLPos position);
    //! Sets the actor's renderer
	DLLEXPORT void SetRenderer(CLRenderer* pRenderer) { m_pRenderer = pRenderer; }
    //! Sets the actor's rendering z-layer
	DLLEXPORT void SetRenderLayer(uint8_t layer) { m_Position.z = layer; }
    //! Sets the actor's rotation angle
	DLLEXPORT void SetRotation(double angle) { m_Rotation = angle; }
    //! Sets the actor's scale
	DLLEXPORT void SetScale(CLVector2 scale);
    //! Sets the actor's velocity vector
	DLLEXPORT void SetVelocity(CLVector2 velocity) { m_Velocity = velocity; }
    //! Stops all actions that are running
	DLLEXPORT void StopAllActions();
    //! Stops all actions that are running that change the actor's position
	DLLEXPORT void StopAllMoveActions();
    
    //! Returns this actor's alpha value
	DLLEXPORT uint8_t       GetAlpha()        const { return m_pTexture->GetAlphaValue(); }
    //! Returns this actor's color
	DLLEXPORT CLColor3      GetColor()        const { return m_Color; }
    //! Returns this actor's unique identifier
	DLLEXPORT uint32_t      GetId()           const { return m_Id; }
    //! Returns this actor's lifespan
	DLLEXPORT float         GetLifespan()     const { return m_Lifespan; }
    //! Gets the actor's position
	DLLEXPORT CLPos         GetPosition()     const { return m_Position; }
    //! Returns the actor's position/dimension rect
	DLLEXPORT CLRect        GetRect()         const { return m_RenderRect; }
    //! Returns the actor's rendering layer (z-order)
	DLLEXPORT uint8_t       GetRenderLayer()  const { return m_Position.z; }
    //! Returns a pointer to the actor's renderer
	DLLEXPORT CLRenderer*   GetRenderer()     const { return m_pRenderer; }
    //! Returns the actor's rotation angle
	DLLEXPORT double        GetRotation()     const { return m_Rotation; }
    //! Returns the actor's scale
	DLLEXPORT CLVector2     GetScale()        const { return m_Scale; }
    //! Returns a pointer to the actor's software surface
	DLLEXPORT CLSurface*    GetSurface()      const { return m_pSurface; }
    //! Returns a pointer to the actor's hardware texture
	DLLEXPORT CLTexture*    GetTexture()      const { return m_pTexture; }
    //! Returns this actor's velocity vector
	DLLEXPORT CLVector2     GetVelocity()     const { return m_Velocity; }
    //! Returns true if this actor is alive
	DLLEXPORT bool          IsAlive()         const { return m_bAlive; }

protected:
    
    //! Default constructor
	DLLEXPORT CLAActor();
    //! Copy constructor
	DLLEXPORT CLAActor(const CLAActor& actor);
    
    //! Frees surface and texture
	DLLEXPORT void FreeActor();
    //! Assigns the actor's rect (position and dimensions)
	DLLEXPORT void SetActorRenderRect(CLRect rect);
    //! Assigns the actor's software surface
	DLLEXPORT void SetActorSurface(CLSurface* pSurface) { m_pSurface = pSurface; }
    //! Assigns the actor's hardware texture
	DLLEXPORT void SetActorTexture(CLTexture* pTexture) { m_pTexture = pTexture; }

private:

    //! Returns true if this actor is running a sequence action
	DLLEXPORT bool IsRunningSequence();

    // Basic attributes
    uint32_t      m_Id;              //!< Identifier for this actor when in a CLActorPool
    bool          m_bAlive;          //!< Whether this actor is alive or not
    CLColor3      m_Color;           //!< Color to apply to actor
    CLPos         m_Position;        //!< Position in the scene
    double        m_Rotation;        //!< Rotation angle
    CLVector2     m_Scale;           //!< Actor's scale
    float         m_Lifespan;        //!< Actor is destroyed if this is reaches 0, never destroyed if -1
    CLVector2     m_Velocity;        //!< Movement velocity in pixels per second

    std::vector<CLAction*> m_CurrentActions;
    

    // Rendering 
    CLRenderer*   m_pRenderer;       //!< Renderer that renders this actor
    CLSurface*    m_pSurface;        //!< Software surface
    CLTexture*    m_pTexture;        //!< Hardware texture
    CLRect        m_RenderRect;      //!< Rendering position and size
};

#endif // _INCLUDE_CLACTOR_H_