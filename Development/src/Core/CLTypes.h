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
#ifndef _INCLUDE_CLTYPES_H_
#define _INCLUDE_CLTYPES_H_

#include <cstdint>
#include <utility>

#ifdef CRYSTALLAYERENGINE_EXPORTS 
    #define DLLEXPORT __declspec(dllexport)  
#else
    #define DLLEXPORT __declspec(dllimport)  
#endif

#ifdef _DEBUG
    #define _PROFILING
#endif

// Tells SDL that we have our own main function
#define SDL_MAIN_HANDLED

// We don't need much from windows
#define WIN32_LEAN_AND_MEAN

#define CL_CLONEABLE_BASE(Type) virtual Type* NewCopy() const = 0;
#define CL_CLONEABLE(Type)      virtual Type* NewCopy() const { return new Type(*this); }

// Colors
struct CLColor3 { uint8_t r, g, b; } ;        //!< RGB color with values 0-255
#define CLCOLOR_WHITE   {255,255,255}
#define CLCOLOR_BLACK   {0,0,0}
#define CLCOLOR_RED     {255,0,0}
#define CLCOLOR_GREEN   {0,255,0}
#define CLCOLOR_BLUE    {0,0,255}
#define CLCOLOR_YELLOW  {255,255,0}
#define CLCOLOR_CYAN    {0,255,255}
#define CLCOLOR_PURPLE  {255,0,255}
#define CLCOLOR_GREY    {185,185,185}

// Geometry
struct CLPos { float x, y; uint8_t z = 0; };  //!< A position where x and y are 2D coordinates and z is rendering depth


struct CLVector2 { float x, y; };           //!< A 2D float vector
struct CLVector3 { float x, y, z; };        //!< A 3D float vector
struct CLRect { float x, y, w, h; };        //!< A basic rectangle structure
struct CLSize2D { float w, h; };            //!< A 2D size structure with width and height
#define CLPOS_ZERO      {0.f,0.f,0}
#define CLVECTOR_ZERO   {0.f,0.f}
#define CLVECTOR_ONE    {1.f,1.f}
#define CLRECT_ZERO     {0.f,0.f,0.f,0.f}
#define CLSIZE_ZERO     {0.f,0.f}

#endif // _INCLUDE_CLTYPES_H_

