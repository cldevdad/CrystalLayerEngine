/*
Crystal Layer - main include file
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
#ifndef _INCLUDE_CRYSTALLAYER_H_
#define _INCLUDE_CRYSTALLAYER_H_

#include "SDL.h"

#include "Core\d_printf.h"
#include "Core\CLGame.h"
#include "Core\CLScene.h"
#include "Core\CLEvent.h"

// Actors
#include "Actors\CLAActor.h"
#include "Actors\CLAButton.h"
#include "Actors\CLALabel.h"
#include "Actors\CLAParticles.h"
#include "Actors\CLASprite.h"

// Actions
#include "Actions\CLActionCallFunc.h"
#include "Actions\CLActionCallFuncEx.h"
#include "Actions\CLActionDelay.h"
#include "Actions\CLActionFadeTo.h"
#include "Actions\CLActionMoveBy.h"
#include "Actions\CLActionMoveTo.h"
#include "Actions\CLActionScaleTo.h"
#include "Actions\CLActionSequence.h"

#include "Audio/CLAudioEngine.h"

#endif // _INCLUDE_CRYSTALLAYER_H_