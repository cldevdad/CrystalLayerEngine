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

#include "CLActionSequence.h"
#include "..\Actors\CLAActor.h"
#include "..\Core\d_printf.h"

/**
*   Copy constructor
*       @param action The source CLAction action
*/
CLActionSequence::CLActionSequence(const CLActionSequence& action)
    : CLAction(action)
{
    m_bDestroyActor = action.m_bDestroyActor;
    m_pActorPool    = action.m_pActorPool;

    for(auto it = action.m_ActionQueue.begin(); it != action.m_ActionQueue.end(); ++it)
    {
        CLAction* pAction = *it;
        m_ActionQueue.push_back(pAction->NewCopy());
    }
}

/**
*   Constructor that takes an actor and constructs an empty sequence with a duration of 0
*       @param pActor The actor this action is controlling
*/
CLActionSequence::CLActionSequence(CLAActor* pActor) :
    CLAction(0.f),
    m_bDestroyActor(false),
    m_pActorPool(nullptr)
{
}

/**
*   Constructor that takes a list of actions to perform in sequence.
*       @param actions A list of actions to perform
*       @param destroyActor Whether or not to destroy the actor at the end
*       @param pActorPool Actor pool to destroy the actor from, if destroyActor is true
*/
CLActionSequence::CLActionSequence(std::initializer_list<CLAction*> actions, bool destroyActor, CLActorPool* pActorPool) :
    CLAction(0.f),
    m_bDestroyActor(destroyActor),
    m_pActorPool(pActorPool)
{
    for (auto pAction : actions)
    {
        AddAction(pAction);
    }
}

/**
*   Destructor that finishes the action in case the action is destroyed
*   before finishing
*/
CLActionSequence::~CLActionSequence()
{
    m_bRunning = false;
    m_bDone = true;

    for (auto it = m_ActionQueue.begin(); it != m_ActionQueue.end(); ++it)
    {
        CLAction* pAction = *it;

        if (pAction != nullptr)
        {
            delete pAction;
            pAction = nullptr;
        }
    }
}

/**
*   Allocates a new action and pushes it onto the sequence
*/
void CLActionSequence::AddAction(CLAction* pAction)
{
    CLAction* NewAction = pAction->NewCopy();
    m_ActionQueue.push_back(NewAction);
    m_Duration += NewAction->GetDuration();
}

/**
*   Starts the sequence by calling start on the queue's front action
*/
void CLActionSequence::Start()
{
    if (!m_ActionQueue.empty())
    {
        CLAction* pFirstAction = m_ActionQueue.front();
        pFirstAction->SetActor(m_pActor);
        pFirstAction->Start();
    }
    else
    {
        d_printf("[%s][WARNING] Started empty sequence.\n", _FUNC);
        Finish();
    }
}

/**
*   Finishes the sequence action by setting running to false and done executing to true
*/
void CLActionSequence::Finish()
{
    m_bRunning = false;
    m_bDone = true;

    if (m_bDestroyActor && (m_pActorPool != nullptr))
    {
        UINT32 id = m_pActor->GetId();
        m_pActorPool->DestroyActorDelayed(m_pActor->GetId());
    }
}

/**
*   Updates the sequence by first checking if all actions are done (we're finished), then
*   checking if the front action is done (free and pop it, and start new front action), then
*   passing the delta time to the front action's update function.
*       @param dt Time in seconds since previous update
*/
void CLActionSequence::Update(float dt)
{
    if (m_bDone || m_ActionQueue.empty())
    {
        return;
    }

    // Get a pointer to the front action
    CLAction* pCurrentAction = m_ActionQueue.front();
    if (pCurrentAction != nullptr)
    {
        pCurrentAction->Update(dt);
    }

    // If the front action is done
    if (pCurrentAction->IsDone())
    {
        // Pop action off the queue
        pCurrentAction = nullptr;
        m_ActionQueue.pop_front();

        if (!m_ActionQueue.empty())
        {
            pCurrentAction = m_ActionQueue.front();

            if (pCurrentAction != nullptr)
            {
                pCurrentAction->SetActor(m_pActor);
                pCurrentAction->Start();
            }
        }
        else
        {
            Finish();
        }
    }
}