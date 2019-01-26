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
#ifndef _INCLUDE_SPLASHSCENE_H_
#define _INCLUDE_SPLASHSCENE_H_

#include "CrystalLayer.h"

/**
*   Splash screen scene that shows logos on startup then changes 
*   to the main menu
*/
class SplashScene : public CLScene
{
public:
    SplashScene(CLGame* pGame);

    void Init();
    void Cleanup();

    void HandleInput(CLEvent& event);
    void Update(float dt);

private:

    UINT32  m_MusicTrack;

    void GoToMainMenu();
};

#endif // _INCLUDE_SPLASHSCENE_H_