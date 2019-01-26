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
#include "SwaapShip.h"
#include <functional>

enum SwaapShipDirections { UP = 0, RIGHT = 90, DOWN = 180, LEFT = 270};

/*
    NOTE ABOUT SHIP MOVEMENT:

    The ship needs to be constricted to a movement grid in this game. If the user moves
    the ship's sprite actor while it is already moving, it will stop the previous movement 
    where it is and start the next movement. This causes the ship to be misaligned with the
    grid. (This happens if the user is moving the ship around really fast)

    So, the ship constructs with an array of positions (m_GridPositions). Instead of moving 
    the ship by its current position + travel distance, it moves from its current position to 
    the target position in the array (tracked by m_GridPlace).

    For example, on 4x4 grid, the possible values of m_GridPlace are:

    1		2		3		4
    5		6		7		8
    9		10		11		12
    13		14		15		16

    If the ship is in the CLPos at position 1, and the user presses down, it'll move to the
    CLPos at position 5.

    Column is determined by position % rowsize (if 0, column == rowsize)

*/

/** 
*   Constructor that takes a pointer to the ship's sprite along with movement properties
*       @param pSprite Sprite to visually represent the ship
*       @param moveBounds Rectangle bounds the ship is allowed to move within
*       @param travelDistance How far the ship travels when moving
*       @param travelSpeed How long it takes the ship to travel when moving
*       @param shootSpeed How long it takes the ship to shoot
*/
SwaapShip::SwaapShip(CLASprite* pSprite, CLRect moveBounds, float travelDistance, float travelSpeed, float shootSpeed) :
    m_MoveBounds(moveBounds),
    m_TravelDistance(travelDistance),
    m_TravelSpeed(travelSpeed),
    m_ShootTime(shootSpeed),
    m_NumChainedKills(0)
{
    // Assign sprite and set a random color to the ship
    m_pSprite = pSprite;
    SetRandomColor();

    // Set up movement grid positions
    int rowIndex = 0;
    for (int gridIndex = 0; gridIndex < SWAAPSHIP_GRIDSIZE; ++gridIndex)
    {
        int        column = ((gridIndex + 1) % SWAAPSHIP_ROWSIZE);
        if (column == 0)
        {
            column = SWAAPSHIP_COLUMNSIZE;
        }
        
        // Create position
        m_GridPositions[gridIndex] =
        {
            m_MoveBounds.x + m_TravelDistance * (column-1),
            m_MoveBounds.y + m_TravelDistance * rowIndex,
            pSprite->GetRenderLayer()
        };

        // New row next time around
        if (column == SWAAPSHIP_COLUMNSIZE) rowIndex++;
    }

    // Move to first position
    ResetPosition();
}

/**
*   Tells the ship to move down
*/
void SwaapShip::MoveDown()
{
    int TargetGridPlace = m_GridPlace + SWAAPSHIP_ROWSIZE;
    if (TargetGridPlace <= SWAAPSHIP_GRIDSIZE)
    {
        CLActionMoveTo MoveAction(m_GridPositions[TargetGridPlace-1], m_TravelSpeed);
        m_pSprite->RunAction(MoveAction);
        m_GridPlace = TargetGridPlace;
    }
}

/**
*   Tells the ship to move left
*/
void SwaapShip::MoveLeft()
{
    int Column = m_GridPlace % SWAAPSHIP_ROWSIZE;
    if (Column == 0) Column = SWAAPSHIP_ROWSIZE;

    if (Column != 1)
    {
        int TargetIndex = m_GridPlace - 2;
        CLActionMoveTo MoveAction(m_GridPositions[TargetIndex], m_TravelSpeed);
        m_pSprite->RunAction(MoveAction);
        m_GridPlace--;
    }
}


/**
*   Tells the ship to move right
*/
void SwaapShip::MoveRight()
{
    int Column = m_GridPlace % SWAAPSHIP_ROWSIZE;
    if (Column == 0) Column = SWAAPSHIP_ROWSIZE;

    if (Column != SWAAPSHIP_ROWSIZE)
    {
        int TargetIndex = m_GridPlace;
        CLActionMoveTo MoveAction(m_GridPositions[TargetIndex], m_TravelSpeed);
        m_pSprite->RunAction(MoveAction);

        m_GridPlace++;
    }
}

/**
*   Tells the ship to move up
*/
void SwaapShip::MoveUp()
{
    int TargetGridPlace = m_GridPlace - SWAAPSHIP_ROWSIZE;
    if (TargetGridPlace >= 1)
    {
        CLActionMoveTo MoveAction(m_GridPositions[TargetGridPlace - 1], m_TravelSpeed);
        m_pSprite->RunAction(MoveAction);
        m_GridPlace = TargetGridPlace;
    }
}

/**
*   Rotates the ship's sprite to point down
*/
void SwaapShip::PointDown()
{
    m_pSprite->SetRotation(DOWN);
}

/**
*   Rotates the ship's sprite to point left
*/
void SwaapShip::PointLeft()
{
    m_pSprite->SetRotation(LEFT);
}

/**
*   Rotates the ship's sprite to point right
*/
void SwaapShip::PointRight()
{
    m_pSprite->SetRotation(RIGHT);
}

/**
*   Rotates the ship's sprite to point up
*/
void SwaapShip::PointUp()
{
    m_pSprite->SetRotation(UP);
}

/** 
*   Resets the ship's position to the first grid position and makes it
*   point up
*/
void SwaapShip::ResetPosition()
{
    m_GridPlace = 1;
    m_pSprite->SetPosition(m_GridPositions[m_GridPlace - 1]);
    PointUp();
}

/**
*   Turns the ship to face in the opposite direction
*/
void SwaapShip::TurnAround()
{
    switch (static_cast<int>(m_pSprite->GetRotation()))
    {
        case UP:
            PointDown();
            break;
        case RIGHT:
            PointLeft();
            break;
        case DOWN:
            PointUp();
            break;
        case LEFT:
            PointRight();
            break;
        default:
            break;
    }
}

/**
*   Tells the ship to shoot off in the direction it is facing, then back
*   again. This action will be terminated early if the ship collides with 
*   an enemy (see GameplayScene::UpdateEnemies). In that case, Return will
*   be called.
*       @param distance Distance vector for shoot distance
*/
void SwaapShip::Shoot(CLVector2 distance)
{
    AllowColorChanges();
    m_NumChainedKills = 0;

    m_pSprite->StopAllActions();
    CLActionMoveBy      ActShoot(distance.x, distance.y, m_ShootTime);
    CLActionCallFunc    ActTurn(std::bind(&SwaapShip::TurnAround, this));
    CLActionMoveTo      ActReturn(GridPosition(), m_ShootTime);
    CLActionSequence    ActShootSequence({ &ActShoot, &ActTurn, &ActReturn });
    m_pSprite->RunAction(ActShootSequence);
}

/**
*   Runs an action to return the ship to its grid position.
*/
void SwaapShip::Return()
{
    BlockColorChanges();

    m_pSprite->StopAllActions();
    CLActionCallFunc    ActTurn(std::bind(&SwaapShip::TurnAround, this));
    CLActionMoveTo      ActReturn(GridPosition(), m_ShootTime);
    CLActionCallFunc    ActAllowColorChanges(std::bind(&SwaapColorEntity::AllowColorChanges, this));
    CLActionSequence    ActReturnSequence({ &ActTurn, &ActReturn, &ActAllowColorChanges });
    m_pSprite->RunAction(ActReturnSequence);
}