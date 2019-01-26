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
#include "GameplayScene.h"
#include "CrystalLayer.h"
#include "..\SwaapGame.h"
#include <string>

using namespace std;

//#define CHEAT_NO_ENEMIES

const float g_SwaapTileWidth            = 64.f;
const float g_SwaapTileHeight           = 64.f;
const float g_SwaapShipSpeed            = 0.125f;    
const float g_SwaapShootSpeed           = 0.2f;
const float g_SwaapEnemySpawnSpeed      = 1.5f;
const int   g_SwaapEnemyValueMultiplier = 100;

/*
*   Constructor that calls CLScene's constructor with the game and initializes
*   gameplay members
*       /param pGame The CLGame to run this scene
*/
GameplayScene::GameplayScene(CLGame* pGame) :
    CLScene(pGame),
    m_bGamePaused(false),
    m_Score(0),
    m_pScoreLabel(nullptr),
    m_bUpdateScoreLabel(false),
    m_pPlayerShip(nullptr),
    m_pPlayerShipSprite(nullptr),
    m_pShipZone(nullptr),
    m_TopEnemySpawnPoints(),
    m_BottomEnemySpawnPoints(),
    m_LeftEnemySpawnPoints(),
    m_RightEnemySpawnPoints(),
    m_EnemySpawnCountdown(0.f),
    m_PreviousEnemySpawnZone(NoZone)
{
}

/*
*   Deconstructor that calls cleanup
*/
GameplayScene::~GameplayScene()
{
    Cleanup();
}

/*
*   Initialize the scene and load all actors from the Gameplay actor file
*/
void GameplayScene::Init()
{
    // Create the player ship sprite
    CLASprite PlayerShipSprite;
    PlayerShipSprite.Create("Ship.png", { 512.f, 368.f, 6 });
    m_pPlayerShipSprite = ActorPool()->AddSpriteActor("PlayerShip", PlayerShipSprite);

    // Create the ship movement zone sprite
    CLASprite ShipZoneSprite;
    ShipZoneSprite.Create("ShipZone.png", { 512, 368, 1 }, { 150, 150, 150 });
    m_pShipZone = ActorPool()->AddSpriteActor("ShipZone", ShipZoneSprite);

    // Get the rect dimensions of the ship zone for setting the ship's movement boundary
    CLRect ShipBoundary = CLRECT_ZERO;
    if (m_pShipZone != nullptr)
    {
        ShipBoundary =
        {
            m_pShipZone->GetRect().x,
            m_pShipZone->GetRect().y,
            m_pShipZone->GetRect().w,
            m_pShipZone->GetRect().h
        };
    };

    // Create the player ship
    m_pPlayerShip = new SwaapShip(m_pPlayerShipSprite, ShipBoundary, g_SwaapTileWidth, g_SwaapShipSpeed, g_SwaapShootSpeed);

    // Load actors from file for this scene
    ActorPool()->AddActorsFromFile("content/properties/GameplayActors.json");

    // Hide the "Paused" text
    ActorPool()->FindLabel("Paused")->SetAlpha(0);
    ActorPool()->FindSprite("ScreenDim")->SetAlpha(0);

    // Initialize score and set score label text
    m_pScoreLabel = ActorPool()->FindLabel("Score");
    m_Score = 0;
    m_bUpdateScoreLabel = true;

    // Top zone enemy spawn points (first 4 dot sprites)
    m_TopEnemySpawnPoints[0] = ActorPool()->FindSprite("EnemyZoneTop_1")->GetPosition();
    m_TopEnemySpawnPoints[1] = ActorPool()->FindSprite("EnemyZoneTop_2")->GetPosition();
    m_TopEnemySpawnPoints[2] = ActorPool()->FindSprite("EnemyZoneTop_3")->GetPosition();
    m_TopEnemySpawnPoints[3] = ActorPool()->FindSprite("EnemyZoneTop_4")->GetPosition();

    // Bottom zone enemy spawn points (last 4 dot sprites)
    m_BottomEnemySpawnPoints[0] = ActorPool()->FindSprite("EnemyZoneBottom_17")->GetPosition();
    m_BottomEnemySpawnPoints[1] = ActorPool()->FindSprite("EnemyZoneBottom_18")->GetPosition();
    m_BottomEnemySpawnPoints[2] = ActorPool()->FindSprite("EnemyZoneBottom_19")->GetPosition();
    m_BottomEnemySpawnPoints[3] = ActorPool()->FindSprite("EnemyZoneBottom_20")->GetPosition();

    // Left zone enemy spawn points (first dot sprite of each row)
    m_LeftEnemySpawnPoints[0] = ActorPool()->FindSprite("EnemyZoneLeft_1")->GetPosition();
    m_LeftEnemySpawnPoints[1] = ActorPool()->FindSprite("EnemyZoneLeft_8")->GetPosition();
    m_LeftEnemySpawnPoints[2] = ActorPool()->FindSprite("EnemyZoneLeft_15")->GetPosition();
    m_LeftEnemySpawnPoints[3] = ActorPool()->FindSprite("EnemyZoneLeft_22")->GetPosition();

    // Right zone enemy spawn points (last dot sprite of each row)
    m_RightEnemySpawnPoints[0] = ActorPool()->FindSprite("EnemyZoneRight_7")->GetPosition();
    m_RightEnemySpawnPoints[1] = ActorPool()->FindSprite("EnemyZoneRight_14")->GetPosition();
    m_RightEnemySpawnPoints[2] = ActorPool()->FindSprite("EnemyZoneRight_21")->GetPosition();
    m_RightEnemySpawnPoints[3] = ActorPool()->FindSprite("EnemyZoneRight_28")->GetPosition();

    // Initialize the enemy spawn timer
    m_EnemySpawnCountdown = g_SwaapEnemySpawnSpeed;

    // Initialize sounds
    m_ShootSound = CLAudioEngine::GetEngine()->AddSoundEffect("content/Sounds/SlideBrush.wav");
    m_SwapSound  = CLAudioEngine::GetEngine()->AddSoundEffect("content/Sounds/Silly2.wav");
    m_ScoreSound = CLAudioEngine::GetEngine()->AddSoundEffect("content/Sounds/WAV-S-ARM WHOOSH QUICK 02.wav");
    m_LoseSound  = CLAudioEngine::GetEngine()->AddSoundEffect("content/Sounds/Lost.wav");

    m_HitSound[0] = CLAudioEngine::GetEngine()->AddSoundEffect("content/Sounds/StoneFootstep1.wav");
    m_HitSound[1] = CLAudioEngine::GetEngine()->AddSoundEffect("content/Sounds/StarPongPlain.wav");
    m_HitSound[2] = CLAudioEngine::GetEngine()->AddSoundEffect("content/Sounds/StarPongHit2.wav");
    m_HitSound[3] = CLAudioEngine::GetEngine()->AddSoundEffect("content/Sounds/StarPongHit1.wav");
    m_HitSound[4] = CLAudioEngine::GetEngine()->AddSoundEffect("content/Sounds/WeaponBonus.wav");
    m_HitSound[5] = CLAudioEngine::GetEngine()->AddSoundEffect("content/Sounds/StoneFootstep1.wav");
    m_HitSound[6] = CLAudioEngine::GetEngine()->AddSoundEffect("content/Sounds/StoneFootstep1.wav");

    // Music
    m_MusicTrack[0] = CLAudioEngine::GetEngine()->AddMusic("content/Music/Mining by Moonlight.mp3");
    m_MusicTrack[1] = CLAudioEngine::GetEngine()->AddMusic("content/Music/StyleFunk.mp3");
    m_MusicTrack[2] = CLAudioEngine::GetEngine()->AddMusic("content/Music/ZigZag.mp3");
    m_MusicTrack[3] = CLAudioEngine::GetEngine()->AddMusic("content/Music/Your Call.mp3");
    CLAudioEngine::GetEngine()->PlayMusic(m_MusicTrack[0]);

    // Make sure we're not paused
    m_bGamePaused = false;
}

/*
*   Cleans up resources used by Gameplay
*/
void GameplayScene::Cleanup()
{ 
    CLScene::Cleanup();

    // Empty enemies
    m_TopEnemies.clear();
    m_BottomEnemies.clear();
    m_LeftEnemies.clear();
    m_RightEnemies.clear();

    // Reset gameplay
    m_EnemySpawnCountdown = g_SwaapEnemySpawnSpeed;
    m_PreviousEnemySpawnZone = NoZone;
    m_Score = 0;
    m_bUpdateScoreLabel = true;

    // Free player ship
    if (m_pPlayerShip != nullptr)
    {
        delete m_pPlayerShip;
        m_pPlayerShip = nullptr;
    }

    // Stop music
    CLAudioEngine::GetEngine()->StopMusic();
}

/*
*   Handles input events for Gameplay
*/
void GameplayScene::HandleInput(CLEvent& event)
{
    switch (event.Type())
    {
        case CL_KEYDOWN:
            switch (event.Code())
            {
                case CLK_ESCAPE:
                    GetGame()->PopScene();
                    break;

                case CLK_SPACE:
                    if (!m_bGamePaused)
                    {
                        Shoot();
                    }
                    break;

                case CLK_p:
                    m_bGamePaused ? ResumeGame() : PauseGame();
                    break;

                case CLK_UP:
                case CLK_w:
                    if (!m_bGamePaused)
                    {
                        m_pPlayerShip->PointUp();
                        m_pPlayerShip->MoveUp();
                    }
                    break;

                case CLK_DOWN:
                case CLK_s:
                    if (!m_bGamePaused)
                    {
                        m_pPlayerShip->PointDown();
                        m_pPlayerShip->MoveDown();
                    }
                    break;

                case CLK_LEFT:
                case CLK_a:
                    if (!m_bGamePaused)
                    {
                        m_pPlayerShip->PointLeft();
                        m_pPlayerShip->MoveLeft();
                    }
                    break;

                case CLK_RIGHT:
                case CLK_d:
                    if (!m_bGamePaused)
                    {
                        m_pPlayerShip->PointRight();
                        m_pPlayerShip->MoveRight();
                    }
                    break;
            }
            break;

        /*case SDL_CONTROLLERBUTTONDOWN:
        {
            switch (event.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_START:
                    m_bGamePaused ? ResumeGame() : PauseGame();
                    break;

                case SDL_CONTROLLER_BUTTON_A:
                    if (!m_bGamePaused)
                    {
                        Shoot();
                    }
                    break;

                case SDL_CONTROLLER_BUTTON_B:
                    GetGame()->PopScene();
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    if (!m_bGamePaused)
                    {
                        m_pPlayerShip->PointUp();
                        m_pPlayerShip->MoveUp();
                    }
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    if (!m_bGamePaused)
                    {
                        m_pPlayerShip->PointDown();
                        m_pPlayerShip->MoveDown();
                    }
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    if (!m_bGamePaused)
                    {
                        m_pPlayerShip->PointLeft();
                        m_pPlayerShip->MoveLeft();
                    }
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    if (!m_bGamePaused)
                    {
                        m_pPlayerShip->PointRight();
                        m_pPlayerShip->MoveRight();
                    }
                    break;

            }
        }*/
    }
}

/*
*   Updates Gameplay logic
*/
void GameplayScene::Update(float dt)
{
    // Call the base scene update so it can update the actor pool
    CLScene::Update(dt);

    if (m_bGamePaused)
    {
        return;
    }

    // Update the score label
    if (m_bUpdateScoreLabel)
    {
        if (m_pScoreLabel != nullptr)
        {
            m_pScoreLabel->ChangeText(m_Score);
        }

        m_bUpdateScoreLabel = false;
    }

#if !defined(CHEAT_NO_ENEMIES)
    // Spawn enemies
    m_EnemySpawnCountdown -= dt;
    if (m_EnemySpawnCountdown <= 0)
    {
        SpawnEnemy();
        m_EnemySpawnCountdown = g_SwaapEnemySpawnSpeed;
    }

    // Enemy collisions
    UpdateEnemies(&m_TopEnemies);
    UpdateEnemies(&m_BottomEnemies);
    UpdateEnemies(&m_LeftEnemies);
    UpdateEnemies(&m_RightEnemies);
#endif
}

/*
*   Adjusts the score value by adding a delta value to it
*       /param delta Value to add to the score
*/
void GameplayScene::AdjustScore(INT32 delta)
{
    // Play sound
    CLAudioEngine::GetEngine()->PlaySoundEffect(m_ScoreSound);

    // Change score
    m_Score += delta;
    m_bUpdateScoreLabel = true;

    // Bounce score label
    static CLPos ScoreOriginPos = m_pScoreLabel->GetPosition();
    static CLPos ScoreHighPos = { ScoreOriginPos.x, ScoreOriginPos.y - 16.f, ScoreOriginPos.z };
    static CLPos ScoreLowPos = { ScoreOriginPos.x, ScoreOriginPos.y + 16.f, ScoreOriginPos.z };
    
    CLActionMoveTo   ActBounceUp(ScoreHighPos, 0.05f);
    CLActionMoveTo   ActBounceDown(ScoreLowPos, 0.05f);
    CLActionMoveTo   ActMoveBack(ScoreOriginPos, 0.1f);
    CLActionSequence ActScoreBounce({ &ActBounceUp, &ActBounceDown, &ActMoveBack });

    m_pScoreLabel->SetPosition(ScoreOriginPos);
    m_pScoreLabel->RunAction(ActScoreBounce);

    // Temporary music change
    static bool doitonce = true;
    if (doitonce && m_Score >= 5000)
    {
        CLAudioEngine::GetEngine()->StopMusic();
        CLAudioEngine::GetEngine()->PlayMusic(m_MusicTrack[2]);
        doitonce = false;
    }
}

/*
*   Callback function for a CLActionCallFuncEx to adjust score.
*       /param pData Ignored
*       /param nData Value to add to the score
*/
void GameplayScene::AdjustScoreCB(void* pData, int nData)
{
    INT32 Delta = static_cast<INT32>(nData);
    AdjustScore(Delta);
}

/*
*   Called when the game is over so that actors can be paused while
*   still giving the user input control
*/
void GameplayScene::GameOver()
{
    CLAudioEngine::GetEngine()->StopMusic();
    CLAudioEngine::GetEngine()->PlaySoundEffect(m_LoseSound);
    ActorPool()->FindSprite("ShipZone")->SetColorMod({ 255, 0, 0 });
    PauseGame();
}

/*
*   Sets the game to paused and displays the pause screen dim and label
*/
void GameplayScene::PauseGame()
{
    if (m_bGamePaused)
    {
        return;
    }

    m_bGamePaused = true;
    ActorPool()->FindActor("ScreenDim")->SetAlpha(255);
    ActorPool()->FindActor("Paused")->SetAlpha(255);
}

/*
*   Sets the game to unpaused and hides the pause screen dim and label
*/
void GameplayScene::ResumeGame()
{
    if (!m_bGamePaused)
    {
        return;
    }

    m_bGamePaused = false;
    ActorPool()->FindActor("ScreenDim")->SetAlpha(0);
    ActorPool()->FindActor("Paused")->SetAlpha(0);
}

/*
*   Calculates the distance from the ship to the edge of the screen and tells
*   the ship to shoot that distance
*/
void GameplayScene::Shoot()
{
    static  CLSize2D  ScreenSize    = CLRenderer::GetRenderer()->GetScreenSize();
    const   CLPos     ShipPosition  = m_pPlayerShipSprite->GetPosition();
    const   int       ShipRotation  = static_cast<int>(m_pPlayerShipSprite->GetRotation());
            CLVector2 Distance      = CLVECTOR_ZERO;

    // Get distance
    switch (ShipRotation)
    {
        case 0:
            // Up
            Distance = { 0, -ShipPosition.y };
            break;

        case 90:
            // Right
            Distance = { ScreenSize.w - ShipPosition.x, 0 };
            break;

        case 180:
            // Down
            Distance = { 0, ScreenSize.h - ShipPosition.y };
            break;

        case 270:
            // Left
            Distance = { -ShipPosition.x, 0 };
            break;

        default:
            break;
    }

    // Shoot ship
    m_pPlayerShip->Shoot(Distance);

    // Play sound
    CLAudioEngine::GetEngine()->PlaySoundEffect(m_ShootSound);
}

/*
*   Performs a special fade effect on the four corner dot grids. This is called
*   when the player performs a big combo.
*       /param pEnemy Pointer to the destroyed enemy to get color from
*/
void GameplayScene::RunDotColorSpecialEffect(SwaapEnemy* pEnemy)
{
    //CLColor3 color = pEnemy->GetSprite()->GetColor();

    const int NumDotSpritesPerQuadrant = 35;

    for (int i = 1; i <= NumDotSpritesPerQuadrant; ++i)
    {
        char id[64] = { 0 };
        sprintf_s(id, 64, "stl_%i", i);
        CLASprite* pDotSprite = ActorPool()->FindSprite(id);

        CLActionFadeTo ActFadeOut(0, 0.5f);
        CLActionFadeTo ActFadeIn(255, 0.5f);
        CLActionSequence ActBlink({ &ActFadeOut, &ActFadeIn });
        pDotSprite->RunAction(ActBlink);
    }

    for (int i = 1; i <= NumDotSpritesPerQuadrant; ++i)
    {
        char id[64] = { 0 };
        sprintf_s(id, 64, "str_%i", i);
        CLASprite* pDotSprite = ActorPool()->FindSprite(id);

        CLActionFadeTo ActFadeOut(0, 0.5f);
        CLActionFadeTo ActFadeIn(255, 0.5f);
        CLActionSequence ActBlink({ &ActFadeOut, &ActFadeIn });
        pDotSprite->RunAction(ActBlink);
    }

    for (int i = 1; i <= NumDotSpritesPerQuadrant; ++i)
    {
        char id[64] = { 0 };
        sprintf_s(id, 64, "sbl_%i", i);
        CLASprite* pDotSprite = ActorPool()->FindSprite(id);

        CLActionFadeTo ActFadeOut(0, 0.5f);
        CLActionFadeTo ActFadeIn(255, 0.5f);
        CLActionSequence ActBlink({ &ActFadeOut, &ActFadeIn });
        pDotSprite->RunAction(ActBlink);
    }

    for (int i = 1; i <= NumDotSpritesPerQuadrant; ++i)
    {
        char id[64] = { 0 };
        sprintf_s(id, 64, "sbr_%i", i);
        CLASprite* pDotSprite = ActorPool()->FindSprite(id);

        CLActionFadeTo ActFadeOut(0, 0.5f);
        CLActionFadeTo ActFadeIn(255, 0.5f);
        CLActionSequence ActBlink({ &ActFadeOut, &ActFadeIn });
        pDotSprite->RunAction(ActBlink);
    }
}

/*
*   Updates all the enemies in an enemy container each frame. First, it checks
*   if the enemy has hit the center for game over. Then it checks if the enemy
*   has hit the player ship. It determines whether to destroy the enemy or switch
*   colors between the player and enemy. Enemies advancing is taken care of in
*   GameplayScene::PlaceEnemy.
*       /param enemies One of the four enemy groups to update
*   
*/
void GameplayScene::UpdateEnemies(EnemyGroup* enemies)
{
    for (auto it = enemies->begin(); it != enemies->end(); )
    {
        SwaapEnemy& Enemy        = *it;
        CLASprite*  pEnemySprite = Enemy.GetSprite();

        if (pEnemySprite != nullptr)
        {
            // If enemy hits center
            if (pEnemySprite->OverlapsActor(m_pShipZone))
            {
                GameOver();
                return;
            }
            // If enemy hits player ship
            else if (pEnemySprite->OverlapsActor(m_pPlayerShipSprite))
            {
                CLPos           EnemyPos  = pEnemySprite->GetPosition();
                CLPos           PlayerPos = m_pPlayerShipSprite->GetPosition();
                EnemyZone       Zone      = Enemy.GetZone();
                bool            bParallel = false;

                if ((Zone == TopZone) || (Zone == BottomZone))
                {
                    bParallel = (static_cast<int>(EnemyPos.x) == static_cast<int>(PlayerPos.x));
                }
                else if((Zone == LeftZone) || (Zone == RightZone))
                {
                    bParallel = (static_cast<int>(EnemyPos.y) == static_cast<int>(PlayerPos.y));
                }

                // If player and enemy are parallel
                if (bParallel)
                {
                    SwaapColor EnemyColor  = Enemy.GetColor();
                    SwaapColor PlayerColor = m_pPlayerShip->GetColor();

                    // Same color (Destroy Enemy)
                    if (EnemyColor == PlayerColor)
                    {
                        // Calculate points based on number of enemies killed this update
                        m_pPlayerShip->AddKill();
                        int Kills  = m_pPlayerShip->GetKills();
                        int Points = g_SwaapEnemyValueMultiplier * Kills;

                        if (Kills >= 3)
                        {
                            // Special effects for chain kills
                            RunDotColorSpecialEffect(&Enemy);
                        }

                        // Play a sound from the array of chained hit sounds
                        CLAudioEngine::GetEngine()->PlaySoundEffect(m_HitSound[Kills - 1]);

                        // Create a points label by the enemy
                        CLALabel PointsLabel;
                        PointsLabel.Create(Points, "V5Xtende.ttf", static_cast<float>(22 + 11*Kills), CLCOLOR_WHITE, EnemyPos);

                        const int  MaxPointsLabelIDs = 1000;
                        static int IntID = 0;
                        char       StringID[16] = { 0 };
                                   sprintf_s(StringID, 16, "PointsLabel_%i", IntID);
                        CLALabel* pLabel = ActorPool()->AddLabelActor(StringID, PointsLabel);
                        IntID = (IntID >= MaxPointsLabelIDs) ? 0 : IntID + 1;

                        // Bounce the points label off in a random direction then move it to the score and increase score
                        int       RandX       = 0;
                        int       RandY       = 0;
                        CLPos     RandPos     = CLPOS_ZERO;
                        CLSize2D  ScreenSize  = CLRenderer::GetRenderer()->GetScreenSize();
                        bool      bOnScreen   = false;
                        const int MaxAttempts = 50;
                        int       Attempts    = 0;
                        do 
                        {
                            RandX = std::rand() / ((RAND_MAX + 1) / (256 * Kills)) - (128 * Kills);
                            RandY = std::rand() / ((RAND_MAX + 1) / (256 * Kills)) - (128 * Kills);
                            RandPos = { PointsLabel.GetPosition().x + RandX, PointsLabel.GetPosition().y + RandY };

                            bOnScreen =    RandPos.x > 0
                                        && RandPos.y > 0
                                        && RandPos.x < ScreenSize.w
                                        && RandPos.y < ScreenSize.h;

                            Attempts++;

                        } while (!bOnScreen && Attempts < MaxAttempts);

                        float StickTime = (Kills == 1) ? 0.f : 0.045f * Kills;
                        CLActionMoveTo      ActBounceOff(RandPos, 0.15f);
                        CLActionDelay       ActStick(StickTime);
                        CLActionMoveTo      ActMovePoints(m_pScoreLabel->GetPosition(), 0.45f);
                        CLActionCallFuncEx  ActIncreaseScore(bind(&GameplayScene::AdjustScoreCB, this, placeholders::_1, placeholders::_2), nullptr, Points);
                        
                        CLActionSequence ActScoreChange({ &ActBounceOff, &ActStick, &ActMovePoints, &ActIncreaseScore }, true, ActorPool());
                        pLabel->RunAction(ActScoreChange);

                        // Destroy Enemy sprite and remove enemy from group
                        ActorPool()->DestroyActor(pEnemySprite->GetId());
                        it = enemies->erase(it);
                    }
                    // Different colors (Switch Colors)
                    else
                    {
                        // Play sound
                        CLAudioEngine::GetEngine()->PlaySoundEffect(m_SwapSound);

                        // Switch colors
                        Enemy.SetColor(PlayerColor);
                        m_pPlayerShip->SetColor(EnemyColor);

                        // Block enemy color changes for a brief period, to let the player ship start returning
                        Enemy.BlockColorChanges(0.1f);

                        // Return ship
                        m_pPlayerShip->Return();

                        ++it;
                    }
                }
                else
                {
                    ++it; // Enemy overlaps player, but not parallel to ship
                }
            }
            else
            {
                ++it; // Enemy doesn't overlap player
            }
        }
        else
        {
            ++it; // Enemy sprite was nullptr
        }
    }
}

/*
*   Spawns an enemy at a zone that is not the previous zone
*/
void GameplayScene::SpawnEnemy()
{
    // Get a random spawn zone that isn't the previous spawn zone
    EnemyZone SpawnZone = NoZone;
    do
    {
        SpawnZone = static_cast<EnemyZone>(1 + rand() / ((RAND_MAX + 1) / ZONE_MAX));
    }
    while (SpawnZone == m_PreviousEnemySpawnZone);

    // Get a random spawn point in that zone
    const int   NumPoints = 4;
    int         SpawnPoint(rand() / ((RAND_MAX + 1) / NumPoints - 1));
    CLPos*      pSpawnPosition = nullptr;

    switch (SpawnZone)
    {
        default:
        case NoZone:
        case TopZone:    pSpawnPosition = &m_TopEnemySpawnPoints[SpawnPoint]; break;
        case RightZone:  pSpawnPosition = &m_RightEnemySpawnPoints[SpawnPoint]; break;
        case BottomZone: pSpawnPosition = &m_BottomEnemySpawnPoints[SpawnPoint]; break;
        case LeftZone:   pSpawnPosition = &m_LeftEnemySpawnPoints[SpawnPoint]; break;
    }

    // Save this spawn zone as the previous spawn zone
    m_PreviousEnemySpawnZone = SpawnZone;

    // Create an indicator sprite
    CLASprite    Indicator;
    const  UINT8 IndicatorZLayer  = 8;
    const  int   MaxIndicatorIds  = 1000;
    static int   IntId            = 0;
    char         StringId[16]     = { 0 };
    sprintf_s(StringId, 16, "EIndicator_%i", IntId);
    IntId = (IntId < MaxIndicatorIds) ? IntId + 1 : 0;

    Indicator.Create("EnemySpawnIndicator.png", { pSpawnPosition->x, pSpawnPosition->y, IndicatorZLayer });
    CLASprite* pIndicator = ActorPool()->AddSpriteActor(StringId, Indicator);
    
    // Create a sequence of actions to fade the indicator in and out, and place an enemy, with the 
    // action flagged to destroy the indicator on completion.
    CLActionFadeTo      ActFadeOutIndicator(0, 0.25f);
    CLActionFadeTo      ActFadeInIndicator(255, 0.25f);
    CLActionCallFuncEx  ActPlaceEnemy(bind(&GameplayScene::PlaceEnemyCB, this, placeholders::_1, placeholders::_2), pSpawnPosition, SpawnPoint );
    CLActionSequence    ActSpawn(
                        {
                            &ActFadeOutIndicator,
                            &ActFadeInIndicator,
                            &ActFadeOutIndicator,
                            &ActFadeInIndicator,
                            &ActFadeOutIndicator,
                            &ActPlaceEnemy
                        }, 
                        true,
                        ActorPool());

    pIndicator->RunAction(ActSpawn);
}

/*
*   Callback that places an enemy when an enemy spawn sequence action is complete. The
*   enemy's position is stored in the pointer argument and the enemy's spawn line is
*   the int data.
*       /param pData Pointer to the CLPos to place the enemy at
*       /param nData The line this enemy spawns at in its group
*/
void GameplayScene::PlaceEnemyCB(void* pData, int nData)
{
    CLPos* pPosition = reinterpret_cast<CLPos*>(pData);
    int    SpawnLine = nData;

    // Create the enemy sprite
    CLASprite EnemySprite;

        /* TEMPORARY - Each enemy design should be assigned its own color, but this just randomizes it */
            int enemyNum = (1 + rand() / ((RAND_MAX + 1) / 4));
            char buf[256] = "";
            sprintf_s(buf, 256, "Enemy_%i.png", enemyNum);
            EnemySprite.Create(buf, *pPosition);
        /* END */

    // Create a unique string ID for the enemy sprite
    const int  MaxEnemyIDs = 1000;
    static int IntID = 0;
    char       StringID[16] = { 0 };
    
	sprintf_s(StringID, 16, "Enemy_%i", IntID);
    IntID = (IntID < MaxEnemyIDs) ? IntID + 1 : 0;

    // Add the enemy sprite to the actor pool
    ActorPool()->AddActor(StringID, EnemySprite);
    
    // Determine zone and group based on enemy sprite position, so the 
    // enemy object can be created in the correct group.
    CLASprite*      pEnemySprite = ActorPool()->FindSprite(StringID);
    CLPos           EnemyPos     = pEnemySprite->GetPosition();
    CLSize2D const  ScreenSize   = GetGame()->GetRenderer()->GetScreenSize();

    EnemyZone   SpawnZone   = NoZone;
    EnemyGroup* pEnemyGroup = nullptr;
    if (EnemyPos.y < g_SwaapTileHeight * 4)
    {
        SpawnZone   = TopZone;
        pEnemyGroup = &m_TopEnemies;
    }
    else if (EnemyPos.y > ScreenSize.h - (g_SwaapTileHeight * 4))
    {
        SpawnZone   = BottomZone;
        pEnemyGroup = &m_BottomEnemies;
    }
    else if (EnemyPos.x < g_SwaapTileWidth * 4)
    {
        SpawnZone   = LeftZone;
        pEnemyGroup = &m_LeftEnemies;
    }
    else if (EnemyPos.x > ScreenSize.w - (g_SwaapTileWidth * 4))
    {
        SpawnZone   = RightZone;
        pEnemyGroup = &m_RightEnemies;
    }

    // Tell all enemies in this line to advance
    if (pEnemyGroup != nullptr)
    {
        for (SwaapEnemy& enemy : *pEnemyGroup)
        {
            if (enemy.GetLine() == SpawnLine)
            {
                enemy.Advance();
            }
        }
    }

    // Add enemy
    SwaapEnemy Enemy(ActorPool()->FindSprite(StringID), SpawnZone, SpawnLine, g_SwaapTileWidth, 0.1f);
    pEnemyGroup->push_back(Enemy);
}