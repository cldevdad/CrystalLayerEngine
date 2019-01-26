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
#include "MainMenuScene.h"
#include "GameplayScene.h"
#include "CrystalLayer.h"
#include "SwaapGame.h"

using namespace std;

/*
*   Default constructor that calls CLScene's default constructor and initializes
*   main menu members
*       /param pGame The CLGame to run this scene
*/
MainMenuScene::MainMenuScene(CLGame* pGame)
    : CLScene(pGame)
{
}

/*
*   Initialize the scene and load all actors from the Main Menu actor file
*/
void MainMenuScene::Init()
{ 
    // Load actors from file for this scene
    ActorPool()->AddActorsFromFile("content/properties/MainMenuActors.json");

    m_PlaySound = CLAudioEngine::GetEngine()->AddSoundEffect("content/sounds/Target.wav");
    m_Music = CLAudioEngine::GetEngine()->AddMusic("content/Music/Babylon.mp3");
}

/*
*   Cleans up resources used by the Main Menu
*/
void MainMenuScene::Cleanup()
{ 
    // Cleanup parent
    CLScene::Cleanup();
}

/*
*   Handles input events for the Main Menu
*/
void MainMenuScene::HandleInput(CLEvent& event)
{
    switch (event.Type())
    {
        case CL_KEYDOWN:
            switch (event.Code())
            {
                case CLK_SPACE:
                    CLAudioEngine::GetEngine()->PlaySoundEffect(m_PlaySound);
                    GetGame()->PushScene(dynamic_cast<SwaapGame*>(GetGame())->Gameplay());
                    break;
                case CLK_ESCAPE:
                    GetGame()->PopScene();
                    break;
                case CLK_t:
                    GetGame()->PushScene(dynamic_cast<SwaapGame*>(GetGame())->Test());
                    break;
            }
            break;


        /*case SDL_CONTROLLERBUTTONDOWN:
        {
            switch (event.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_A:
                case SDL_CONTROLLER_BUTTON_START:
                    CLAudioEngine::GetEngine()->PlaySoundEffect(m_PlaySound);
                    GetGame()->PushScene(dynamic_cast<SwaapGame*>(GetGame())->Gameplay());
                    break;

                case SDL_CONTROLLER_BUTTON_B:
                    GetGame()->PopScene();
                    break;

                case SDL_CONTROLLER_BUTTON_X:
#               ifdef _DEBUG
                    GetGame()->PushScene(dynamic_cast<SwaapGame*>(GetGame())->Test());
#               endif
                    break;
            }
        }*/
    }
}

/*
*   Updates logic for the Main Menu
*/
void MainMenuScene::Update(float dt)
{
    CLScene::Update(dt);

    // Swap the position of the letter A's at an interval
    static float LetterSwapTimer  = 0.f;
    const  float TimeBetweenSwaps = 5.f;
    const  float SwapDuration     = .25f;

    LetterSwapTimer += dt;
    if (LetterSwapTimer >= TimeBetweenSwaps)
    {
        CLALabel* FirstA = ActorPool()->FindLabel("A1");
        CLALabel* SecondA = ActorPool()->FindLabel("A2");

        CLPos FirstTarget = SecondA->GetPosition();
        CLActionMoveTo MoveFirstLetter(FirstTarget, SwapDuration);
        FirstA->RunAction(MoveFirstLetter);

        CLPos SecondTarget = FirstA->GetPosition();
        CLActionMoveTo MoveSecondLetter(SecondTarget, SwapDuration);
        SecondA->RunAction(MoveSecondLetter);

        LetterSwapTimer = 0.f;
    }
}


void MainMenuScene::Pause()
{
    CLAudioEngine::GetEngine()->StopMusic();
    CLScene::Pause();
}

void MainMenuScene::Resume()
{
    CLAudioEngine::GetEngine()->PlayMusic(m_Music);
    CLScene::Resume();
}