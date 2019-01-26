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
#ifndef _INCLUDE_GAMEPLAYSCENE_H_
#define _INCLUDE_GAMEPLAYSCENE_H_

#include "CrystalLayer.h"
#include "..\SwaapShip.h"
#include "..\SwaapEnemy.h"
#include <vector>

typedef std::vector<SwaapEnemy> EnemyGroup;

/**
*   The gameplay scene where the user is playing the game and can go back
*   to the main menu.
*/
class GameplayScene : public CLScene
{
public:

    GameplayScene(CLGame* pGame);
    ~GameplayScene();

    void Init();
    void Cleanup();
    void HandleInput(CLEvent& event);
    void Update(float dt);
   
private:

    /* Core Gameplay ***************************************************************************************/
    void                    AdjustScore(INT32 delta);               //!< Changes score value
    void                    AdjustScoreCB(void* pData, int nData);  //!< Callback to change score
    void                    UpdateEnemies(EnemyGroup* enemies);     //!< Updates enemies in a container
    void                    GameOver();                             //!< Performs game over logic
    void                    PauseGame();                            //!< Pauses the game
    void                    ResumeGame();                           //!< Resumes (unpauses) the game
    void                    Shoot();                                //!< Tells the ship to shoot
    void                    RunDotColorSpecialEffect(SwaapEnemy* pEnemy);
    
    bool                    m_bGamePaused;              //!< True if the game is paused
    UINT32                  m_Score;                    //!< Player's score value
    SwaapShip*              m_pPlayerShip;              //!< Player controlled ship object
    CLASprite*              m_pPlayerShipSprite;        //!< Player ship sprite
    CLASprite*              m_pShipZone;                //!< Zone sprite that the ship moves around on
    
    /* Enemies *********************************************************************************************/
    void                    PlaceEnemyCB(void* pData, int nData);   //!< Places an enemy
    void                    SpawnEnemy();                           //!< Starts an enemy spawn          
    float                   m_EnemySpawnCountdown;                  //!< Enemy spawned when this reaches zero

    EnemyZone               m_PreviousEnemySpawnZone;   //!< Last zone that spawned an enemy
    EnemyGroup              m_TopEnemies;               //!< Enemies in the top zone
    CLPos                   m_TopEnemySpawnPoints[4];   //!< Enemy spawn points for the top zone
    EnemyGroup              m_BottomEnemies;            //!< Enemies in the bottom zone
    CLPos                   m_BottomEnemySpawnPoints[4];//!< Enemy spawn points for the bottom zone
    EnemyGroup              m_LeftEnemies;              //!< Enemies in the left zone
    CLPos                   m_LeftEnemySpawnPoints[4];  //!< Enemy spawn points for the left zone
    EnemyGroup              m_RightEnemies;             //!< Enemies in the right zone
    CLPos                   m_RightEnemySpawnPoints[4]; //!< Enemy spawn points for the right zone
                              
    /* User Interface **************************************************************************************/
    CLALabel*               m_pScoreLabel;              //!< Label that displays the score
    bool                    m_bUpdateScoreLabel;        //!< Whether the score label needs an update

    /* Audio ***********************************************************************************************/
    UINT32 m_ScoreSound;    //!< Score changed sound
    UINT32 m_ShootSound;    //!< Player shooting sound
    UINT32 m_SwapSound;     //!< Swap colors sound
    UINT32 m_HitSound[7];   //!< Hit a same-color enemy sound
    UINT32 m_LoseSound;     //!< Game over sound

    UINT32 m_MusicTrack[4];    //!< Gameplay music
    UINT32 m_VictoryMusic;     //!< Music played at level end
};

#endif // _INCLUDE_GAMEPLAYSCENE_H_