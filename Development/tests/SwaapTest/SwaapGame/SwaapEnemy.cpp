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
#include "SwaapEnemy.h"

SwaapEnemy::SwaapEnemy(CLASprite* pSprite, EnemyZone zone, int line, float travelDistance, float travelSpeed) :
    m_Zone(zone),
    m_Line(line),
    m_TravelDistance(travelDistance),
    m_TravelSpeed(travelSpeed)
{
    // Set the sprite's z position so it's always on top
    CLPos SpritePos = pSprite->GetPosition();
    SpritePos.z = 5;
    pSprite->SetPosition(SpritePos);

    // Rotate the sprite to face the center
    switch (m_Zone)
    {
        case TopZone:    pSprite->SetRotation(180.f); break;
        case RightZone:  pSprite->SetRotation(270.f); break;
        case BottomZone: pSprite->SetRotation(0.f); break;
        case LeftZone:   pSprite->SetRotation(90.f); break;
    }

    // Set the color entity's sprite
    m_pSprite = pSprite;

    // Set a random color
    SetRandomColor();
}

void SwaapEnemy::Advance()
{
    if (m_pSprite == nullptr)
        return;

    CLPos pos = CLPOS_ZERO;
    switch (m_Zone)
    {
        case TopZone: 
        {
            pos = GetSprite()->GetPosition();
            pos = { pos.x, pos.y + m_TravelDistance, pos.z };
            CLActionMoveTo MoveAction(pos, m_TravelSpeed);
            m_pSprite->RunAction(MoveAction);
            break;
        }
        case RightZone:
        {
            pos = GetSprite()->GetPosition();
            pos = { pos.x - m_TravelDistance, pos.y, pos.z };
            CLActionMoveTo MoveAction(pos, m_TravelSpeed);
            m_pSprite->RunAction(MoveAction);
            break;
        }
        case BottomZone:
        {
            pos = GetSprite()->GetPosition();
            pos = { pos.x, pos.y - m_TravelDistance, pos.z };
            CLActionMoveTo MoveAction(pos, m_TravelSpeed);
            m_pSprite->RunAction(MoveAction);
            break;
        }
        case LeftZone:
        {
            pos = GetSprite()->GetPosition();
            pos = { pos.x + m_TravelDistance, pos.y, pos.z };
            CLActionMoveTo MoveAction(pos, m_TravelSpeed);
            m_pSprite->RunAction(MoveAction);
            break;
        }
    }
}

