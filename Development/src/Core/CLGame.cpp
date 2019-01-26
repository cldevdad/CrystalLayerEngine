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
#include "CLGame.h"
#include "CLScene.h"
#include "CLEvent.h"
#include "..\Actors\CLALabel.h"
#include "..\Actions\CLActionMoveTo.h"
#include "d_printf.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "SDL.h"

using namespace std;
using namespace std::chrono;

#define CLGAME_EXIT_OK 0
#define CLGAME_EXIT_ERROR -1

/**
*   Constructor that calls Init with the game title, dimensions, and number
*   of rendering layers
*       @param title Title of the game to display on the window
*       @param size Size of the game window
*       @param renderLayers Number of z-layers for rendering
*/
CLGame::CLGame(const char* title, CLSize2D size, UINT8 renderLayers) :
    m_pRenderer(nullptr),
    m_bFPSCount(false)
{
    // Initialize window and renderer
    m_pWindow = new CLWindow(title, size);
    if (m_pWindow != nullptr)
    {
        m_pWindow->SetTitle(title);
        m_pRenderer = CLRenderer::GetRenderer();

        if (m_pRenderer != nullptr)
        {
            m_pRenderer->SetLayerCount(renderLayers);
            m_pRenderer->AttachToWindow(m_pWindow);
        }
    }

    // Initialize game controller
    m_pGamepad = new CLGamepad(0);
}

/**
*   Destructor that frees the framerate counter, window, and renderer
*/
CLGame::~CLGame()
{
    if (m_bFPSCount)
    {
        DestroyFPSLabel();
    }

    if (m_pWindow != nullptr)
    {
        delete m_pWindow;
        m_pWindow = nullptr;
    }

    if (m_pRenderer != nullptr)
    {
        delete m_pRenderer;
        m_pRenderer = nullptr;
    }

    if (m_pGamepad != nullptr)
    {
        delete m_pGamepad;
        m_pGamepad = nullptr;
    }
}

/**
*    Runs the main game loop
*        \return Returns CLGAME_EXIT_OK
*/
int8_t CLGame::Run()
{
    // We are running until Update tells us otherwise
    m_bRunning = true;

    // Game loop
    float  DeltaSeconds = 0;
    while (m_bRunning)
    {
        auto TicksStart = steady_clock::now();

        HandleEvents();
        Update(DeltaSeconds);
        Render();

        auto TicksEnd = steady_clock::now();
        auto TicksDelta = duration_cast<nanoseconds>(TicksEnd - TicksStart);
        DeltaSeconds = static_cast<float>(TicksDelta.count() * 1e-9);
    }

    return CLGAME_EXIT_OK;
}

/**
*    Toggles the framerate counter on and off
*        @param enable True to enable, false to disable
*/
void CLGame::ToggleFPSCount(bool enable)
{
    if (enable && !m_bFPSCount)
    {
        CreateFPSLabel();
        m_bFPSCount = true;
    }
    else if (!enable && m_bFPSCount)
    {
        m_bFPSCount = false;
        DestroyFPSLabel();
    }
}

/** 
*    Handles input and other application events. This works by calling the HandleEvents
*    function of the scene currently on top of the stack.
*/
void CLGame::HandleEvents()
{
	CLEvent event;
	event.Poll();

    // Handle Quit events
    if (event.Type() == CL_QUIT)
    {
        m_bRunning = false;
        Quit();
    }
    
    else if (event.Type() == CL_KEYDOWN)
    {
        switch (event.Code())
        {
            // Tilde ~ to toggle framerate counter
            case CLK_BACKQUOTE:
                ToggleFPSCount(!m_bFPSCount);
                break;

            case SDLK_KP_1:
                MoveFPSLabel(BottomLeft);
                break;
            case SDLK_KP_3:
                MoveFPSLabel(BottomRight);
                break;
            case SDLK_KP_7:
                MoveFPSLabel(TopLeft);
                break;
            case SDLK_KP_9:
                MoveFPSLabel(TopRight);
                break;

            default:
                if (!m_Scenes.empty())
                {
                    m_Scenes.top()->HandleInput(event);
                }
                break;
        }  
    }
    // Pass all other events to the top game scene's input handler
    /*else if (!m_Scenes.empty())
    {
        m_Scenes.top()->HandleInput(event);
    }*/
}

/** 
*    Called each frame to update logic. This works by calling the Update
*    function of the scene currently on top of the stack.
*/
void CLGame::Update(float dt)
{
    if (!m_Scenes.empty())
    {
        m_Scenes.top()->Update(dt);
    }
    else
    {
        m_bRunning = false;
    }

    if (m_bFPSCount)
    {
        UpdateFPSLabel(dt);
    }
}

/** 
*    Renders graphics each frame. This works by calling the Render
*    function of the scene currently on top of the stack.
*/
void CLGame::Render()
{
    if (!m_Scenes.empty())
    {
        // Clear renderer
        m_pRenderer->Clear();

        // Render top scene
        m_Scenes.top()->Render();

        // Render FPS label
        if (m_bFPSCount) m_pFPSLabel->Render();

        // Present renderer
        m_pRenderer->Present();
    }
}

/** 
*    Changes the current scene to another one by cleaning up the current scene,
*    popping it off the stack, then pushing and initializing the new scene.
*        @param scene The new scene to change to
*/
void CLGame::ChangeScene(CLScene* scene)
{
    // Clean up the current scene
    if (!m_Scenes.empty())
    {
        m_Scenes.top()->Cleanup();
        m_Scenes.pop();
    }

    // Push the new scene and initialize
    m_Scenes.push(scene);
    m_Scenes.top()->Init();
}

/** 
*    Changes to another scene, but keeps the current scene running below it by
*    pausing the current scene and pushing the new scene on top.
*        @param scene The new scene to make active
*/
void CLGame::PushScene(CLScene* scene)
{
    // Pause the current scene
    if (!m_Scenes.empty())
    {
        m_Scenes.top()->Pause();
    }

    // Push the new scene and initialize
    m_Scenes.push(scene);
    m_Scenes.top()->Init();
}

/** 
*    Pops the current scene off the stack and resumes the scene that was
*    underneath it if there is one.
*/
void CLGame::PopScene()
{
    // Clean up the current scene
    if (!m_Scenes.empty())
    {
        m_Scenes.top()->Cleanup();
        m_Scenes.pop();
    }

    // Resume the previous scene or clean up if no scenes remain
    if (!m_Scenes.empty())
    {
        m_Scenes.top()->Resume();
    }
}

/**
*   Calculates the frames per second and updates the FPS label
*       /param dt Delta time in seconds
*/
void CLGame::UpdateFPSLabel(float dt)
{
    static int   FrameCount = 0;
    static float Seconds = 0.0f;
    static float UpdateRate = 0.5f;

    FrameCount++;
    Seconds += dt;
    if (Seconds > (1.0 / UpdateRate))
    {
        float fps = (FrameCount / Seconds);
        FrameCount = 0;

        char fpstext[16];
        snprintf(fpstext, 16, "%.2f", fps);
        m_pFPSLabel->ChangeText(fpstext);

        Seconds = 0;
    }

    m_pFPSLabel->Update(dt);
}

/**
*   Creates a CLALabel to display the framerate count
*       /param position Where to render the label
*/
void CLGame::CreateFPSLabel()
{
    // Set the label's position to the bottom-right of the window
    float FontSize  = 30;
    float LabelX    = static_cast<float>(m_pWindow->GetSize().w - FontSize * 6);
    float LabelY    = static_cast<float>(m_pWindow->GetSize().h - FontSize * 1.5);
    CLPos Position  = { LabelX, LabelY };

    // Create label at position
    m_pFPSLabel = new CLALabel();
    m_pFPSLabel->Create("--", "goodtimes.ttf", FontSize, CLCOLOR_WHITE, Position);
}

/**
*   Destroys the CLALabel that displays the framerate count
*/
void CLGame::DestroyFPSLabel()
{
    if (m_pFPSLabel != nullptr)
    {
        delete m_pFPSLabel;
        m_pFPSLabel = nullptr;
    }
}

/**
*   Moves the CLALabel that displays the framerate count to
*   another corner
*/
void CLGame::MoveFPSLabel(WindowCorner corner)
{
    // Don't move the fps label if it doesn't exist
    if (!m_bFPSCount)
    {
        return;
    }

    // Window size
    float WindowWidth = m_pWindow->GetSize().w;
    float WindowHeight = m_pWindow->GetSize().h;

    // Label size
    CLSize2D LabelSize = { m_pFPSLabel->GetRect().w, m_pFPSLabel->GetRect().h };

    // Destination
    float DestX = 0.f;
    float DestY = 0.f;
    const float Padding = 16.f;

    switch (corner)
    {
        case TopLeft:
            DestX = Padding;
            DestY = Padding;
            break;
        case TopRight:
            DestX = static_cast<float>((WindowWidth - LabelSize.w) - Padding);
            DestY = Padding;
            break;
        case BottomLeft:
            DestX = Padding;
            DestY = static_cast<float>((WindowHeight - LabelSize.h) - Padding);
            break;
        case BottomRight:
            DestX = static_cast<float>((WindowWidth - LabelSize.w) - Padding);
            DestY = static_cast<float>((WindowHeight - LabelSize.h) - Padding);
            break;
    }

    // Move
    const float duration = 0.3f;
    CLActionMoveTo MoveLabel(DestX, DestY, duration);
    m_pFPSLabel->RunAction(MoveLabel);
}