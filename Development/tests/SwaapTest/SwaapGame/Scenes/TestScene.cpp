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
#include "TestScene.h"
#include "MainMenuScene.h"
#include "CrystalLayer.h"
#include "SwaapGame.h"
#include <string>

using namespace std;

/*
*   Default constructor that calls CLScene's default constructor and initializes
*   test scene members
*       /param pGame The CLGame to run this scene
*/
TestScene::TestScene(CLGame* pGame)
    : CLScene(pGame)
{
}

/*
*   Initialize the scene and load all actors from the Main Menu actor file
*/
void TestScene::Init()
{
    // Load actors from file for this scene
    ActorPool()->AddActorsFromFile("content/properties/TestSceneActors.json");
    m_pShip = ActorPool()->FindSprite("Ship");

    m_ScreenSize =
    {
        CLRenderer::GetRenderer()->GetScreenSize().w,
        CLRenderer::GetRenderer()->GetScreenSize().h
    };

    ///////////////////////// FIRE
    CLAParticles FireParticles;
    FireParticles.Create(ActorPool(),
        "Particle.png",                                 // Image
        { m_ScreenSize.x / 2, m_ScreenSize.y / 2, 5 },  // Position
        { 5.f, 0.f },                            // Position Var
        800,                                        // Max
        0.05f,                                      // Emit rate
        { 0.f, -50.f },                             // Velocity
        { 10.f, 10.f },                               // Velocity Var
        0.0,                                        // Angle
        0.0,                                        // Angular Velocity
        { 240, 150, 0 },                          // Color
        { 15, 15, 0 },                                // Color Var
        50,                                        // Alpha
        50,                                          // Alpha Var
        1.f,                                        // Scale
        0.f,                                        // Scale Var
        0.5f,                                        // Lifespan
        0.25f,                                       // Lifespan Var
        CLVECTOR_ZERO);                             // Gravity

    m_pFireParticles = ActorPool()->AddParticleActor("FireParticles", FireParticles);


    ///////////////////////// WATER
    CLAParticles WaterParticles;
    WaterParticles.Create(ActorPool(),
        "Particle.png",                                 // Image
        { (m_ScreenSize.x / 2), 0, 4 },             // Position
        { (m_ScreenSize.x / 2), 0.f },              // Position Var
        1000,                                        // Max
        0.00005f,                                      // Emit rate
        { 0.f, 200.f },                             // Velocity
        { 20.f, 100.f },                               // Velocity Var
        0.0,                                        // Angle
        0.0,                                        // Angular Velocity
        { 0, 150, 240 },                          // Color
        { 0, 15, 15 },                                // Color Var
        55,                                        // Alpha
        55,                                          // Alpha Var
        1.f,                                        // Scale
        0.f,                                        // Scale Var
        10.25f,                                        // Lifespan
        0.5f,                                       // Lifespan Var
        CLVECTOR_ZERO);                             // Gravity

    m_pWaterParticles = ActorPool()->AddParticleActor("WaterParticles", WaterParticles);
}

/*
*   Cleans up resources used by the Test Scene
*/
void TestScene::Cleanup()
{
    // Cleanup parent
    CLScene::Cleanup();
}

/*
*   Handles input events for the Test Scene
*/
void TestScene::HandleInput(CLEvent& event)
{
    const float Acceleration = 0.1f;
    const float Momentum     = 0.95f;
    const float Speed        = (1.f - Momentum) * Acceleration;

    switch (event.Type())
    {
        // Keyboard
        case CL_KEYDOWN:
            switch (event.Code())
            {
                case SDLK_ESCAPE:               GetGame()->PopScene(); break;
                case SDLK_SPACE:                m_pFireParticles->Fire(); m_pWaterParticles->Fire(); break;
                case SDLK_r:                    Cleanup(); Init(); break;
                case SDLK_UP:   case SDLK_w:    MoveShip(Up); break;
                case SDLK_DOWN: case SDLK_s:    MoveShip(Down); break;
                case SDLK_LEFT: case SDLK_a:    MoveShip(Left); break;
                case SDLK_RIGHT:case SDLK_d:    MoveShip(Right); break;
            }
            break;

        // Controller
		/*case SDL_CONTROLLERBUTTONDOWN:

			switch (event.cbutton.button)
			{
				case SDL_CONTROLLER_BUTTON_B:           GetGame()->PopScene(); break;
				case SDL_CONTROLLER_BUTTON_Y:           Cleanup(); Init(); break;
				case SDL_CONTROLLER_BUTTON_DPAD_UP:     MoveShip(Up); break;
				case SDL_CONTROLLER_BUTTON_DPAD_DOWN:   MoveShip(Down); break;
				case SDL_CONTROLLER_BUTTON_DPAD_LEFT:   MoveShip(Left); break;
				case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:  MoveShip(Right); break;
			}
			break;*/
    }
}

/*
*   Updates logic for the Test scene
*/
void TestScene::Update(float dt)
{
    CLScene::Update(dt);

    m_pFireParticles->Update(dt);
    m_pWaterParticles->Update(dt);

    CLPos ShipPos = m_pShip->GetPosition();
    if (ShipPos.x > m_ScreenSize.x)
    {
        m_pShip->SetPosition({ 0, ShipPos.y, ShipPos.z });
    }
    else if (ShipPos.x < 0)
    {
        m_pShip->SetPosition({ m_ScreenSize.x, ShipPos.y, ShipPos.z });
    }

    if (ShipPos.y > m_ScreenSize.y)
    {
        m_pShip->SetPosition({ ShipPos.x, 0, ShipPos.z });
    }
    else if (ShipPos.y < 0)
    {
        m_pShip->SetPosition({ ShipPos.x, m_ScreenSize.y, ShipPos.z });
    }


}

void TestScene::MoveShip(SwaapTestDirection direction)
{

    const float Acceleration = 30.f;
    const float Momentum = 0.9f;
    const float Speed = (1.f - Momentum) * Acceleration;

    switch (direction)
    {
        case Up:
            m_pShip->SetVelocity({ m_pShip->GetVelocity().x, (Momentum * m_pShip->GetVelocity().y) - Speed });
            m_pShip->SetRotation(0.f);
            break;

        case Down:
            m_pShip->SetVelocity({ m_pShip->GetVelocity().x, (Momentum * m_pShip->GetVelocity().y) + Speed });
            m_pShip->SetRotation(180.f);
            break;

        case Left:
            m_pShip->SetVelocity({ (Momentum * m_pShip->GetVelocity().x) - Speed, m_pShip->GetVelocity().y });
            m_pShip->SetRotation(270.f);
            break;

        case Right:
            m_pShip->SetVelocity({ (Momentum * m_pShip->GetVelocity().x) + Speed, m_pShip->GetVelocity().y });
            m_pShip->SetRotation(90.f);
            break;
    }
}