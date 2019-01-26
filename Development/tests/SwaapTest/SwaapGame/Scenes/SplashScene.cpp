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
#include "SplashScene.h"
#include "MainMenuScene.h"
#include "CrystalLayer.h"
#include "SwaapGame.h"
#include <string>

using namespace std;

/*
*   Default constructor that calls CLScene's default constructor and initializes
*   slash scene members
*       /param pGame The CLGame to run this scene
*/
SplashScene::SplashScene(CLGame* pGame) :
    CLScene(pGame)
{
}

/*
*   Initialize the Splash scene 
*/
void SplashScene::Init()
{
    // Play music
    m_MusicTrack = CLAudioEngine::GetEngine()->AddMusic("content/Music/Babylon.mp3");
    CLAudioEngine::GetEngine()->PlayMusic(m_MusicTrack);

    // Create Crystal Layer logo
    CLPos const posCLLogo =
    {
        (CLRenderer::GetRenderer()->GetScreenSize().w / 2) - 256,
        (CLRenderer::GetRenderer()->GetScreenSize().h / 2) - 256
    };
    CLASprite LogoSprite;
    LogoSprite.Create("CLLogo.png", posCLLogo);
    CLASprite* pLogo = ActorPool()->AddSpriteActor("CLLogo", LogoSprite);
    
    // Fade logo in and out then call GoToMainMenu
    CLActionFadeTo      ActHide(0, 0.f);
    CLActionFadeTo      ActFadeIn(255, 0.5f);
    CLActionDelay       ActWait(2.f);
    CLActionFadeTo      ActFadeOut(0, 0.5f);
    CLActionCallFunc    ActGoToMainMenu(std::bind(&SplashScene::GoToMainMenu, this));
    CLActionSequence    ActIntro(
                                    { 
                                        &ActHide,
                                        &ActFadeIn,
                                        &ActWait,
                                        &ActFadeOut,
                                        &ActGoToMainMenu
                                    });

    pLogo->RunAction(ActIntro);
}

/*
*   Cleans up resources used by the Splash Scene
*/
void SplashScene::Cleanup()
{
    // Cleanup parent
    CLScene::Cleanup();
}

/*
*   Handles input events for the Splash Scene
*/
void SplashScene::HandleInput(CLEvent& event)
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
                    GoToMainMenu();
                    break;
            }
            break;

        case SDL_CONTROLLERBUTTONDOWN:
            GoToMainMenu();
            break;
    }
}

/*
*   Updates the Splash scene
*/
void SplashScene::Update(float dt)
{
    CLScene::Update(dt);
}

void SplashScene::GoToMainMenu()
{
    GetGame()->ChangeScene(dynamic_cast<SwaapGame*>(GetGame())->MainMenu());
}