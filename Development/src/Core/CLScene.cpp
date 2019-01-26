/*
Crystal Layer
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
#include "CLScene.h"
#include "d_printf.h"
using namespace std;

/*
*   Constructor that saves a pointer to the game and allocates resource pools
*       /param pGame The CLGame to run this scene
*/
CLScene::CLScene(CLGame* pGame) : 
    m_pGame(pGame),
    m_bTransitioning(false) 
{
    // Create scene's actor pool
    m_pActorPool = new CLActorPool(CLRenderer::GetRenderer());
}

/*
*   Destructor that frees resources
*/
CLScene::~CLScene()
{
    Cleanup();

    if (m_pActorPool != nullptr)
    {
        // Free the actor pool
        delete m_pActorPool;
        m_pActorPool = nullptr;
    }
}

/*
*   Cleans up the scene by emptying resource pools
*/
void CLScene::Cleanup()
{
    if (m_pActorPool != nullptr)
    {
        m_pActorPool->DestroyAllActors();
    }
}


/*
*   Updates the scene by updating all actors in the actor pool and processing
*   a scene transition if one is happening
*/
void CLScene::Update(float dt)
{
    if (!m_bPaused)
    {
        // Update all actors in the scene's actor pool
        m_pActorPool->Update(dt);
    }
}

/*
*   Renders every actor in the scene's actor pool
*/
void CLScene::Render()
{
    if (!m_bPaused)
    {
        m_pActorPool->RenderActors();
    }
}

/*
*   Sets the scene's status as paused
*/
void CLScene::Pause()
{
    m_bPaused = true;
}

/*
*   Sets the scene's status as unpaused
*/
void CLScene::Resume()
{
    m_bPaused = false;
}

/** 
*   Simply calls the game's ChangeScene function.
*        /param scene The new scene
*/
void CLScene::ChangeScene(CLScene* pScene)
{
    m_pGame->ChangeScene(pScene);
}