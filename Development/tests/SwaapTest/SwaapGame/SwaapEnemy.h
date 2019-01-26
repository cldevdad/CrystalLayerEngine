/*
Swaap
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
#ifndef _INCLUDE_SWAAPENEMY_H_
#define _INCLUDE_SWAAPENEMY_H_

#include "CrystalLayer.h"
#include "SwaapColorEntity.h"

/**
*   Possible zones for the enemy to spawn in. NoZone should be used
*   when the first enemy hasn't spawned yet.
*/
enum EnemyZone 
{ 
    NoZone, 
    TopZone, 
    RightZone, 
    BottomZone, 
    LeftZone, 
    ZONE_MAX=LeftZone 
};

/**
*   The enemy class
*/
class SwaapEnemy : public SwaapColorEntity
{
public:

    SwaapEnemy(CLASprite* pSprite, EnemyZone zone, int line, float travelDistance, float travelSpeed);
    
    void            Advance();
    int             GetLine()   const { return m_Line; }
    EnemyZone       GetZone()   const { return m_Zone; }

private:

    float           m_TravelDistance;
    float           m_TravelSpeed;
    EnemyZone       m_Zone;
    int             m_Line;
    int             m_Value;
};

#endif // _INCLUDE_SWAAPENEMY_H_