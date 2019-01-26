/*
Debug Only Printf
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
#ifndef _INCLUDE_DPRINTF_H_
#define _INCLUDE_DPRINTF_H_

#include <cstdio>

#if defined(__WINDOWS__)
#include <windows.h>
#endif

//! Shorter function name macro
#define _FUNC __FUNCTION__

// Only printf if in debug mode
#ifdef _DEBUG
#if defined(__WINDOWS__)
    #define DCONSOLE GetStdHandle(STD_OUTPUT_HANDLE)
    #define WHITE_TEXT  FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
    #define YELLOW_TEXT FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN
    #define GREEN_TEXT  FOREGROUND_INTENSITY | FOREGROUND_GREEN
    #define RED_TEXT    FOREGROUND_INTENSITY | FOREGROUND_RED

    #define d_printf(...)     SetConsoleTextAttribute(DCONSOLE, WHITE_TEXT); printf(__VA_ARGS__)
    #define d_printwarn(...)  SetConsoleTextAttribute(DCONSOLE, YELLOW_TEXT); printf(__VA_ARGS__); SetConsoleTextAttribute(DCONSOLE, WHITE_TEXT);
    #define d_printerror(...) SetConsoleTextAttribute(DCONSOLE, RED_TEXT); printf(__VA_ARGS__); SetConsoleTextAttribute(DCONSOLE, WHITE_TEXT);
    #define d_printgood(...)  SetConsoleTextAttribute(DCONSOLE, GREEN_TEXT); printf(__VA_ARGS__); SetConsoleTextAttribute(DCONSOLE, WHITE_TEXT);
    #define d_printfunc printf("[%s]\n", __FUNCTION__)
#else
    #define d_printf(...)     printf(__VA_ARGS__)
    #define d_printwarn(...)  printf(__VA_ARGS__);
    #define d_printerror(...) printf(__VA_ARGS__);
    #define d_printgood(...)  printf(__VA_ARGS__);
    #define d_printfunc printf("[%s]\n", __FUNCTION__)
#endif
#else
    #define d_printf(...) 0
    #define d_printwarn(...) 0
    #define d_printerror(...) 0
    #define d_printfunc 0
#endif

#endif // _INCLUDE_DPRINTF_H_