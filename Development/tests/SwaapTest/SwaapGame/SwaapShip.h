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
#ifndef _INCLUDE_SWAAPSHIP_H_
#define _INCLUDE_SWAAPSHIP_H_

#include "CrystalLayer.h"
#include "SwaapColorEntity.h"

#define SWAAPSHIP_COLUMNSIZE 4
#define SWAAPSHIP_ROWSIZE    4
#define SWAAPSHIP_GRIDSIZE   SWAAPSHIP_COLUMNSIZE * SWAAPSHIP_ROWSIZE
/**
*   The player controlled ship
*/
class SwaapShip : public SwaapColorEntity
{
public:

    //! Constructor that takes a sprite for the ship along with movement properties
    SwaapShip(CLASprite* pSprite, CLRect moveBounds, float travelDistance, float travelSpeed, float shootSpeed);

    CLPos GridPosition() const { return m_GridPositions[m_GridPlace-1]; }
    
    void  AddKill() { m_NumChainedKills++; } //!< Add a kill to the ship
    int   GetKills() const { return m_NumChainedKills; }

    void  MoveDown();                   //!< Move the ship down
    void  MoveLeft();                   //!< Move the ship left
    void  MoveRight();                  //!< Move the ship right
    void  MoveUp();                     //!< Move the ship up
    void  PointDown();                  //!< Point the ship down
    void  PointLeft();                  //!< Point the ship left
    void  PointRight();                 //!< Point the ship right
    void  PointUp();                    //!< Point the ship up
    void  ResetPosition();              //!< Reset ship position
    void  Shoot(CLVector2 distance);    //!< Makes the ship shoot
    void  Return();                     //!< Return to current grid position
    void  TurnAround();                 //!< Turn 180 degrees

private:

    CLRect      m_MoveBounds;       //!< Rectangle bounds the ship is allowed to move in
    float       m_ShootTime;        //!< How long it takes the ship to shoot
    float       m_TravelDistance;   //!< The distance the ship travels when moving
    float       m_TravelSpeed;      //!< How long the ship takes to travel when moving
    int         m_NumChainedKills;  //!< Number of kills while shooting

    CLPos       m_GridPositions[SWAAPSHIP_GRIDSIZE];    //!< The grid of positions the ship can move to
    int         m_GridPlace;                            //!< The ship's place in the position grid
};

#endif // _INCLUDE_PLAYERSHIP_H_