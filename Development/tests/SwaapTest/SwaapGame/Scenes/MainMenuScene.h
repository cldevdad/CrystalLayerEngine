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
#ifndef _INCLUDE_MAINMENUSCENE_H_
#define _INCLUDE_MAINMENUSCENE_H_

#include "CrystalLayer.h"

/**
*   The main menu scene where the user can proceed to gameplay or exit.
*/
class MainMenuScene : public CLScene
{
public:
    MainMenuScene(CLGame* pGame);

    void Init();
    void Cleanup();

    void HandleInput(CLEvent& event);
    void Update(float dt);

    void Pause();
    void Resume();

private:
    
    UINT32 m_PlaySound;
    UINT32 m_Music;
};

#endif // _INCLUDE_MAINMENUSCENE_H_