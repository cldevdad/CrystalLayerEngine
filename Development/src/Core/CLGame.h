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
#ifndef _INCLUDE_CLGAME_H
#define _INCLUDE_CLGAME_H

#include "CLTypes.h"
#include "..\Renderer\CLRenderer.h"
#include "..\Renderer\CLWindow.h"
#include "..\Input\CLGamepad.h"
#include "d_printf.h"

#include <chrono>
#include <stack>

//! Window corners for moving the framerate label
enum WindowCorner { TopLeft, TopRight, BottomLeft, BottomRight };

class CLScene;
class CLALabel;

/**
*   Base class for a game. Has an interface for implementing game loop (event handling, 
*   updating, rendering). Manages a stack of game scenes.
*/
class CLGame
{
public:
    //! Destructor
	DLLEXPORT ~CLGame();

	DLLEXPORT int8_t        Run();                         //!< Runs the game loop
	DLLEXPORT void          Quit() { m_bRunning = false; } //!< Quits the game   
    
	DLLEXPORT virtual void  HandleEvents();                //!< Handles events each frame
	DLLEXPORT virtual void  Update(float dt);              //!< Updates game each frame
	DLLEXPORT virtual void  Render();                      //!< Renders graphics each frame
	DLLEXPORT void          ToggleFPSCount(bool enable);   //!< Enables or disables the framerate counter

	DLLEXPORT virtual void  ChangeScene(CLScene* scene);   //!< Changes from one scene to another
	DLLEXPORT virtual void  PushScene(CLScene* scene);     //!< Pushes a new scene on the stack
	DLLEXPORT virtual void  PopScene();                    //!< Pops the top scene off the stack

	DLLEXPORT CLRenderer*   GetRenderer()  const { return m_pRenderer; }  //!< Returns a pointer to the renderer
	DLLEXPORT CLWindow*     GetWindow()    const { return m_pWindow; }    //!< Returns a pointer to the window

protected:
    //! Constructor
	DLLEXPORT CLGame(const char* title, CLSize2D size, UINT8 renderLayers = CLRENDERER_LAYERS_DEFAULT);

private:
	DLLEXPORT void CreateFPSLabel();                  //!< Creates the framerate counter
	DLLEXPORT void DestroyFPSLabel();                 //!< Destroys the framerate counter
	DLLEXPORT void MoveFPSLabel(WindowCorner corner); //!< Moves the FPS label to a different corner
	DLLEXPORT void UpdateFPSLabel(float dt);          //!< Updates the framerate label

    CLRenderer*             m_pRenderer;    //!< The renderer
    CLWindow*               m_pWindow;      //!< The window
    bool                    m_bRunning;     //!< Whether or not the game is running
    std::stack<CLScene*>    m_Scenes;       //!< Active game scenes
    CLALabel*               m_pFPSLabel;    //!< Framerate label
    bool                    m_bFPSCount;    //!< Whether or not to display the framerate counter
    CLGamepad*              m_pGamepad;     //!< Pointer to a gamepad controller
};

#endif // _INCLUDE_CLGAME_H