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
#include "SwaapGame.h"
#include <string>

using namespace std;

/**
*   Constructor that calls the base CLGame constructor, then allocates
*   scenes for the game and pushes the first scene
*       /param title The window title
*       /param size The size of the window
*       /param renderLayers The number of z-layers for rendering (optional)
*/
SwaapGame::SwaapGame(const char* title, CLSize2D size, uint8_t renderLayers)
    : CLGame::CLGame(title, size, renderLayers)
{
    m_pMainMenuScene = new MainMenuScene(this);
    m_pGameplayScene = new GameplayScene(this);
    m_pTestScene = new TestScene(this);
    m_pSplashScene = new SplashScene(this);

    PushScene(m_pSplashScene);
}

/**
*   Destructor that frees all of the game scenes
*/
SwaapGame::~SwaapGame()
{
    if (m_pMainMenuScene)
    {
        delete m_pMainMenuScene;
        m_pMainMenuScene = nullptr;
    }

    if (m_pGameplayScene)
    {
        delete m_pGameplayScene;
        m_pGameplayScene = nullptr;
    }

    if (m_pTestScene)
    {
        delete m_pTestScene;
        m_pTestScene = nullptr;
    }

    if (m_pSplashScene)
    {
        delete m_pSplashScene;
        m_pSplashScene = nullptr;
    }
}