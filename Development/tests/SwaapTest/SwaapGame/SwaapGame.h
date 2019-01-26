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
#ifndef _INCLUDE_SWAAPGAME_H_
#define _INCLUDE_SWAAPGAME_H_

#include "Core\CLGame.h"
#include "Scenes\MainMenuScene.h"
#include "Scenes\GameplayScene.h"
#include "Scenes\TestScene.h"
#include "Scenes\SplashScene.h"
#include <string>

/**
*   Main class for the Swaap game.
*/
class SwaapGame : public CLGame
{
public:
    //! Constructor
    SwaapGame(const char* title, CLSize2D size, uint8_t renderLayers);
    //! Destructor
    ~SwaapGame();

    //! Returns a pointer to the Main Menu scene
    MainMenuScene* MainMenu() const { return m_pMainMenuScene; }
    //! Returns a pointer to the Gameplay scene
    GameplayScene* Gameplay() const { return m_pGameplayScene; }
    //! Returns a pointer to the Test scene
    TestScene* Test() const { return m_pTestScene; }
    //! Returns a pointer to the Test scene
    SplashScene* Splash() const { return m_pSplashScene; }

private:
    MainMenuScene* m_pMainMenuScene;        //!< Pointer to the Main Menu scene
    GameplayScene* m_pGameplayScene;        //!< Pointer to the Gameplay scene
    TestScene*     m_pTestScene;            //!< Pointer to the test scene
    SplashScene*   m_pSplashScene;          //!< Pointer to the splash screen scene
};

#endif // _INCLUDE_SWAAPGAME_H_