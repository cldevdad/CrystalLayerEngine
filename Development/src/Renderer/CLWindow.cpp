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

#include "CLWindow.h"
#include "SDL.h"
#include "..\Core\d_printf.h"

CLWindow::CLWindow() :
    m_pSDLWindow(nullptr),
    m_Size({0.f,0.f})
{
}

CLWindow::CLWindow(const char* title, CLSize2D size)
    : m_Size(size)
{
    // Initialize SDL 
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        d_printerror("[%s][ERROR!] Couldn't initialize SDL: SDL_Error: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
        m_Size = { 0,0 };
    }

    // Create SDL window
    m_pSDLWindow = SDL_CreateWindow(
        "CL Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        static_cast<int>(m_Size.w),
        static_cast<int>(m_Size.h),
        SDL_WINDOW_SHOWN
    );

    //SDL_SetWindowFullscreen(m_pSDLWindow, SDL_WINDOW_FULLSCREEN);

    if (m_pSDLWindow == nullptr)
    {
        d_printerror("[%s][ERROR!] Couldn't create SDL Window: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
        m_Size = { 0,0 };
    }

    // Set display mode
    SDL_DisplayMode TargetDisplayMode, ClosestDisplayMode;
    TargetDisplayMode.format       = SDL_PIXELFORMAT_RGBA32;
    TargetDisplayMode.w            = static_cast<int>(m_Size.w);
    TargetDisplayMode.h            = static_cast<int>(m_Size.h);
    TargetDisplayMode.refresh_rate = 0;
    TargetDisplayMode.driverdata   = 0;
    d_printf("[%s] Target display mode: %d x %d @ %dhz\n", _FUNC, TargetDisplayMode.w, TargetDisplayMode.h, TargetDisplayMode.refresh_rate);

    if (SDL_GetClosestDisplayMode(0, &TargetDisplayMode, &ClosestDisplayMode) != nullptr)
    {
        d_printgood("[%s] Set display mode: %d x %d @ %dhz\n", _FUNC, ClosestDisplayMode.w, ClosestDisplayMode.h, ClosestDisplayMode.refresh_rate);

        if (SDL_SetWindowDisplayMode(m_pSDLWindow, &ClosestDisplayMode) >= 0)
        {
            d_printgood("[%s] SDL Window initialized\n", _FUNC);
        }
        else
        {
            d_printerror("[%s][ERROR!] Couldn't set SDL Window display mode: %s\n", _FUNC, SDL_GetError());
            SDL_ClearError();
        }
    }
    else
    {
        d_printerror("[%s][ERROR!] No suitible display mode found. SDL Error: %s\n", _FUNC, SDL_GetError());
        SDL_ClearError();
    }
    
}

CLWindow::~CLWindow()
{
    d_printfunc;
    SDL_Quit();
}

void CLWindow::SetSize(CLSize2D size)
{
    m_Size = size;
    SDL_SetWindowSize(m_pSDLWindow, static_cast<int>(size.w), static_cast<int>(size.h));
}

void CLWindow::SetTitle(const char* title)
{
    SDL_SetWindowTitle(m_pSDLWindow, title);
}

