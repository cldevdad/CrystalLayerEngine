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
#include "CLAParticles.h"
#include "CLASprite.h"
#include "..\Core\CLActorPool.h"
#include "..\Actions\CLActionDelay.h"
#include "..\Actions\CLActionCallFunc.h"
#include "..\Actions\CLActionSequence.h"
#include "..\Core\d_printf.h"

using namespace std;

/**
*
*/
CLAParticles::CLAParticles() :
    CLAActor(),
    m_Max(0),
    m_Rate(0.f),
    m_Velocity(CLVECTOR_ZERO),
    m_VelocityVar(CLVECTOR_ZERO),
    m_Angle(0.f),
    m_AngularVelocity(0.f),
    m_Color(CLCOLOR_WHITE),
    m_ColorVar(CLCOLOR_BLACK),
    m_Scale(1.f),
    m_ScaleVar(0.f),
    m_Life(1.f),
    m_LifeVar(0.f),
    m_Gravity(CLVECTOR_ZERO),
    m_bRunning(false),
    m_ActiveParticles(0)
{
}

/**
*
*/
void CLAParticles::Create(CLActorPool* pActorPool,
                          const char*  imageFile, 
                          CLPos        position,
                          CLPos        positionVar,
                          int          max,
                          float        rate,
                          CLVector2    velocity,
                          CLVector2    velocityVar,
                          double       angle, 
                          float        angularVelocity,
                          CLColor3     color, 
                          CLColor3     colorVar,
                          uint8_t      alpha,
                          uint8_t      alphaVar,
                          float        scale,
                          float        scaleVar,
                          float        life, 
                          float        lifeVar,
                          CLVector2    gravity)
{
    sprintf_s(m_ImageFile, imageFile, 512);

    m_pActorPool        = pActorPool;
    m_Position          = position;
    m_PositionVar       = positionVar;
    m_Max               = max;
    m_Rate              = rate;
    m_Velocity          = velocity;
    m_VelocityVar       = velocityVar;
    m_Angle             = angle;
    m_AngularVelocity   = angularVelocity;
    m_Color             = color;
    m_ColorVar          = colorVar;
    m_Alpha             = alpha;
    m_AlphaVar          = alphaVar;
    m_Scale             = scale;
    m_ScaleVar          = scaleVar;
    m_Life              = life;
    m_LifeVar           = lifeVar;
    m_Gravity           = gravity;
    m_bRunning          = false;
}

void CLAParticles::Fire()
{
    if (!m_bRunning)
    {
        m_bRunning = true;
        Emit();
    }
}

void CLAParticles::Update(float dt)
{
    CLAActor::Update(dt);

    if (m_bRunning)
    {
        static float emitTimer = m_Rate;
        emitTimer -= dt;

        if (emitTimer <= 0.f && m_ActiveParticles < m_Max)
        {
            Emit();
            emitTimer = m_Rate;
        }

        if (m_ActiveParticles <= 0)
        {
            End();
            return;
        }
    }
}

void CLAParticles::End()
{
    m_bRunning = false;
    m_ActiveParticles = 0;
    return;
}

void CLAParticles::Emit()
{
    // Create sprite
    CLASprite Sprite;
    Sprite.Create(m_ImageFile, m_Position);

    // Give it a unique id
    const  int   MaxId = 50000;
    static int   IntId = 0;
    char         StringId[32] = { 0 };
    sprintf_s(StringId, 16, "Particle_%i", IntId);
    IntId = (IntId < MaxId) ? IntId + 1 : 0;

    CLASprite* pSprite = m_pActorPool->AddSpriteActor(StringId, Sprite);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  Position
    ////////////////////////////////////////////////////////////////////////////////////////////////
    CLPos pos = m_Position;
    if (m_PositionVar.x != 0.f)
    {
        int rx = std::rand() / ((RAND_MAX + 1) / static_cast<int>(m_PositionVar.x * 2));
        pos.x -= m_PositionVar.x;
        pos.x += static_cast<float>(rx);
    }
    if (m_PositionVar.y != 0.f)
    {
        int ry = std::rand() / ((RAND_MAX + 1) / static_cast<int>(m_PositionVar.x * 2));
        pos.y -= m_PositionVar.y;
        pos.y += static_cast<float>(ry);
    }

    pSprite->SetPosition(pos);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  Velocity
    ////////////////////////////////////////////////////////////////////////////////////////////////
    CLVector2 v = m_Velocity;
    if (m_VelocityVar.x != 0.f)
    {
        int rx = 1 + std::rand() / ((RAND_MAX + 1) / static_cast<int>(m_VelocityVar.x * 2));
        v.x -= m_VelocityVar.x;
        v.x += static_cast<float>(rx);
    }
    if (m_VelocityVar.y != 0.f)
    {
        int ry = 1 + std::rand() / ((RAND_MAX + 1) / static_cast<int>(m_VelocityVar.y * 2));
        v.y -= m_VelocityVar.y;
        v.y += static_cast<float>(ry);
    }

    pSprite->SetVelocity(v);


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  Angle
    ////////////////////////////////////////////////////////////////////////////////////////////////
    pSprite->SetRotation(m_Angle);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  Color
    ////////////////////////////////////////////////////////////////////////////////////////////////
    CLColor3 color = m_Color;
    if (m_ColorVar.r != 0)
    {
        color.r -= m_ColorVar.r;
        color.r += static_cast<uint8_t>(1 + std::rand() / ((RAND_MAX + 1) / (m_ColorVar.r * 2)));
    }
    if (m_ColorVar.g != 0)
    {
        color.g -= m_ColorVar.g;
        color.g += static_cast<uint8_t>(1 + std::rand() / ((RAND_MAX + 1) / (m_ColorVar.g * 2)));

    }
    if (m_ColorVar.b != 0)
    {
        color.b -= m_ColorVar.b;
        color.b += static_cast<uint8_t>(1 + std::rand() / ((RAND_MAX + 1) / (m_ColorVar.b * 2)));
    }

    pSprite->SetColorMod(color);
    

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  Alpha
    ////////////////////////////////////////////////////////////////////////////////////////////////
    uint8_t alpha = m_Alpha;
    if (m_AlphaVar != 0)
    {
        uint8_t ra = static_cast<uint8_t>(1 + std::rand() / ((RAND_MAX + 1) / (m_AlphaVar * 2)));
        alpha -= m_AlphaVar;
        alpha += ra;
    }
    pSprite->SetAlpha(alpha);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  Life
    ////////////////////////////////////////////////////////////////////////////////////////////////
    float life = m_Life;
    if (m_LifeVar > 0)
    {
        uint8_t rl = static_cast<uint8_t>(1 + std::rand() / ((RAND_MAX + 1) / (m_LifeVar * 2.f)));
        life -= m_LifeVar;
        life += rl;
    }

    pSprite->SetLifespan(life);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //  Gravity
    ////////////////////////////////////////////////////////////////////////////////////////////////
    pSprite->SetVelocity({ pSprite->GetVelocity().x + m_Gravity.x, pSprite->GetVelocity().y + m_Gravity.y });

    
    m_ActiveParticles++;

    // Wait the amount of time this particle will live, then call ParticleDiedCB. This decrements
    // the amount of particles in this system. This is the easiest way to do it because the CLActorPool is
    // managing the actual death and deallocation of the sprite actor, and we're just counting how many
    // we've added to it.
    CLActionDelay    ActWaitForDeath(pSprite->GetLifespan());
    CLActionCallFunc ActParticleDied(std::bind(&CLAParticles::ParticleDiedCB, this));
    CLActionSequence ActCallback({ &ActWaitForDeath, &ActParticleDied });
    this->RunAction(ActCallback);
}

/**
*   Empty rendering function since the base CLAActor's rendering function
*   tries to render the actor's texture, and particle actors don't have
*   a texture (only their sprites do, which will be rendered in the actor pool).
*/
void CLAParticles::Render()
{
}

void CLAParticles::ParticleDiedCB()
{
    //if (m_ActiveParticles > 0)
    //{
        m_ActiveParticles--;
    //}
}