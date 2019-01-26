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
#ifndef _INCLUDE_CLSCENE_H_
#define _INCLUDE_CLSCENE_H_

#include "CLGame.h"
#include "CLActorPool.h"
#include "CLEvent.h"

/**
*   Interface for a game scene class.
*/
class CLScene
{
    protected: DLLEXPORT CLScene(CLGame* pGame);              //!< Constructor
    protected: DLLEXPORT virtual ~CLScene();                  //!< Destructor

public:

    virtual void Init() = 0;                                  //!< Initialize scene
	DLLEXPORT virtual void Cleanup();                         //!< Clean up scene

    virtual void           HandleInput(CLEvent& event) = 0;   //!< Called each frame to handle input
	DLLEXPORT virtual void Update(float dt);                  //!< Called each frame to update logic
	DLLEXPORT virtual void Render();                          //!< Called each frame to render graphics

	DLLEXPORT virtual void Pause();                           //!< Pauses scene processing
	DLLEXPORT virtual void Resume();                          //!< Resumes scene processing

	DLLEXPORT void         ChangeScene(CLScene* pScene);      //! Changes from this scene to another scene
	DLLEXPORT CLGame*      GetGame() const {return m_pGame;}  //! Returns a pointer to the game running this scene

protected:

	DLLEXPORT bool         IsPaused() const {return m_bPaused;}     //! Returns true if the scene is paused
	DLLEXPORT CLActorPool* ActorPool() const {return m_pActorPool;} //! Returns a pointer to this scene's actor pool

private:

    CLActorPool* m_pActorPool;       //!< Manages all actors for this scene
    CLGame*      m_pGame;            //!< Pointer to the game running this scene
    bool         m_bPaused;          //!< Whether scene is paused
    float        m_TransitionTime;   //!< Duration of scene transition
    bool         m_bTransitioning;   //!< True if scene is transitioning
};

#endif // _INCLUDE_CLSCENE_H_

