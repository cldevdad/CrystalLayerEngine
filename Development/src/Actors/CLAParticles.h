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
#ifndef _INCLUDE_CLAPARTICLES_H_
#define _INCLUDE_CLAPARTICLES_H_
#include "CLAActor.h"
#include <vector>

class CLASprite;
class CLActorPool;

/**
*   A particle system actor
*/
class CLAParticles : public CLAActor
{

public:

    //! Allows cloning through NewCopy()
	DLLEXPORT CL_CLONEABLE(CLAParticles)
    DLLEXPORT CLAParticles();

	DLLEXPORT
	void Create(CLActorPool* pActorPool,
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
                CLVector2    gravity);

	DLLEXPORT void Fire();
	DLLEXPORT void Update(float dt);

	DLLEXPORT void End();

	DLLEXPORT void Render();
	DLLEXPORT int  GetMax() const { return m_Max; }

private:

	DLLEXPORT void Emit();
	DLLEXPORT void ParticleDiedCB();

    int          m_ActiveParticles;
    bool         m_bRunning;
    char         m_ImageFile[512];
    CLPos        m_Position;
    CLPos        m_PositionVar;
    int          m_Max;
    float        m_Rate;
    CLVector2    m_Velocity;
    CLVector2    m_VelocityVar;
    double       m_Angle;
    float        m_AngularVelocity;
    CLColor3     m_Color;
    CLColor3     m_ColorVar;
    uint8_t      m_Alpha;
    uint8_t      m_AlphaVar;
    float        m_Scale;
    float        m_ScaleVar;
    float        m_Life;
    float        m_LifeVar;
    CLVector2    m_Gravity;

    CLActorPool*            m_pActorPool;
};

#endif // _INCLUDE_CLAPARTICLES_H_