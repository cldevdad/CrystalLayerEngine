
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
#include "CLActorPool.h"
#include "CLTypes.h"
#include "d_printf.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "document.h" // rapidjson
#include "..\Actors\CLALabel.h"
#include "..\Actors\CLASprite.h"

using namespace std;
using namespace rapidjson;

// For profiling add/destroy times
#ifdef _PROFILING
    #include <chrono>
    using namespace std::chrono;
#endif


/**
*   Constructor that assigns the renderer and initializes actor pool members
*/
CLActorPool::CLActorPool(CLRenderer* pRenderer) :
    m_Actors(0),
    m_pRenderer(pRenderer), 
    m_bSortOnUpdate(false)
{
    m_DelayedDestroyHashId = HashId("DestroyActorDelayed");
    m_Actors.reserve(300);
}     

/**
*   Destructor that destroys all actors in the pool.
*/
CLActorPool::~CLActorPool()
{
    DestroyAllActors();
}

/**
*   Adds an actor with a string identifier to the pool. Assigns the actor's
*   renderer and allocates a new actor copy for the pool.
*       /param id A string identifier for looking up the actor
*       /param actor The actor to add to the pool
*       /return A pointer to the actor
*/
CLAActor* CLActorPool::AddActor(const char* id, CLAActor& actor)
{
    // Allocate a new copy of the actor.
    CLAActor* pActor = actor.NewCopy();
    pActor->SetRenderer(m_pRenderer);

    // Create the hashed int identifier and set the actor's id
    uint32_t IntId = HashId(id);
    pActor->SetId(IntId);

    // Create a record from the id and copied actor then insert it
    APRecord record = APRecord(IntId, pActor);
    m_Actors.push_back(record);

    // Whenever an actor is added the pool needs to be sorted
    m_bSortOnUpdate = true;

    d_printf("[%s] Added Actor \"%s\" (hash: %lu)\n", _FUNC, id, IntId);

    return pActor;
}

/**
*   Adds a sprite actor to the pool by casting and calling AddActor
*       /param id A string identifier for looking up the actor
*       /param actor The actor to add to the pool
*       /return A pointer to the sprite
*/
CLASprite* CLActorPool::AddSpriteActor(const char* id, CLASprite& sprite)
{
    return static_cast<CLASprite*>(AddActor(id, sprite));
}

/**
*   Adds a label actor to the pool by casting and calling AddActor
*       /param id A string identifier for looking up the actor
*       /param label The label actor
*       /return A pointer to the sprite
*/
CLALabel* CLActorPool::AddLabelActor(const char* id, CLALabel& label)
{
    return static_cast<CLALabel*>(AddActor(id, label));
}

/**
*   Adds a particle system actor to the pool by adding all its sprites
*       /param id A string identifier for looking up the actor
*       /param actor The particle system actor
*       /return A pointer to the particle system
*/
CLAParticles* CLActorPool::AddParticleActor(const char* id, CLAParticles& particles)
{
    m_Actors.reserve(particles.GetMax());
    return static_cast<CLAParticles*>(AddActor(id, particles));
}

/**
*   Removes an actor from the pool by its string identifier
*       /param id The actor's string identifier
*/
void CLActorPool::DestroyActor(const char* id)
{
    DestroyActor(HashId(id));
}

/**
*   Deallocates memory for an actor and removes it from the pool
*       /param id The actor's hash identifier
*/
void CLActorPool::DestroyActor(uint32_t id)
{
    APIterator it = FindRecord(id);
    CLAActor* pActor = it->second;

    if (pActor != nullptr)
    {
#       ifdef _DEBUG
        // Actor id can be changed in the case of delayed destroy,
        // so we want to printf the original hash ID, not the 
        // "DelayDestroy" hash id.
        uint32_t OriginalActorID = pActor->GetId();
#       endif

        // Free actor
        delete pActor;
        pActor = nullptr;

        // Remove actor
        m_Actors.erase(it);

#       ifdef _DEBUG
        if (id == OriginalActorID)
        {
            printf("[%s] Destroyed actor %lu\n", _FUNC, OriginalActorID);
        }
        else
        {
            printf("[%s] Delay Destroyed actor %lu\n", _FUNC, OriginalActorID);
        }
#       endif
    }
}

/**
*   Flags an actor for destroy on the next update. This is to allow a CLAction
*   that destroys an actor to complete before the actor destroys the action.
*       /param id The actor's hash identifier
*/
void CLActorPool::DestroyActorDelayed(uint32_t id)
{
    APIterator it = FindRecord(id);
    while (it != m_Actors.end())
    {
        it->first = m_DelayedDestroyHashId;
        it = FindRecord(id);
    }
}

/**
*   Destroys all actors in the actor pool, deallocating memory for
*   each actor then emptying the pool entirely
*/
void CLActorPool::DestroyAllActors()
{
#   ifdef _PROFILING 
    auto StartTime = high_resolution_clock::now();
    size_t ActorCount = m_Actors.size();
#   endif

    // Iterate through actor pool and destroy actors
    for(auto const& record : m_Actors)
    {
        CLAActor* pActor = record.second;
        if (pActor != nullptr)
        {
            // Free actor
            delete pActor;
            pActor = nullptr;

            d_printf("[%s] Destroyed Actor %lu.\n", _FUNC, record.first);
        }
    }
    
    // Empty the pool container
    m_Actors.clear();

#   ifdef _PROFILING 
    auto FinishTime = high_resolution_clock::now();
    auto ElapsedTime = duration_cast<milliseconds>(FinishTime - StartTime);
    float ElapsedSeconds = ElapsedTime.count() * 0.001f;
    d_printgood("[%s] Destroyed %d Actors in %.3f seconds\n", __FUNCTION__, (int)ActorCount, ElapsedSeconds);
#   endif
}

/**
*   Looks up an actor record by its identifier and returns the actor
*       /param id The actor's string identifier
*       /return A pointer to the actor or nullptr
*/
CLAActor* CLActorPool::FindActor(const char* id)
{
    UINT32 hashid = HashId(id);

    for (APRecord& record : m_Actors)
    {
        if (record.first == hashid)
        {
            return record.second;
        }
    }

    d_printerror("[%s] Actor %s not found!\n", _FUNC, id);
    return nullptr;
}

/**
*   Attempts to lookup a label actor by its id and return a pointer to it
*       /param id The label's string identifier
*/
CLALabel* CLActorPool::FindLabel(const char* id)
{
    return dynamic_cast<CLALabel*>(FindActor(id));
}

/**
*   Attempts to lookup a sprite actor by its id and return a pointer to it
*       /param id The sprite's string identifier
*/
CLASprite* CLActorPool::FindSprite(const char* id)
{
    return dynamic_cast<CLASprite*>(FindActor(id));
}

/**
*   Loads actors specified in a file, allocates memory
*   for them, and inserts them into the container
*       /param fileName The file name
*/
void CLActorPool::AddActorsFromFile(const char* fileName)
{
#   ifdef _PROFILING
    auto   StartTime = chrono::high_resolution_clock::now();
    size_t StartSize = m_Actors.size();
#   endif

    ifstream inFile(fileName, ios::in);
    if (!inFile.is_open())
    {
        d_printerror("Unable to open file: %s\n", fileName);
        return;
    }

    // Create JSON string
    string jsonString = "";
    string currentLine = "";
    while (getline(inFile, currentLine))
    {
        jsonString.append(currentLine);
    }
    inFile.close();

    // Create JSON document object from string
    Document JsonDocument;
    JsonDocument.Parse(jsonString.c_str());

    // Labels
    if (JsonDocument.HasMember("labels"))
    {
        const Value& labels = JsonDocument["labels"];
        assert(labels.IsArray());
        for (Value::ConstValueIterator itr = labels.Begin(); itr != labels.End(); ++itr)
        {
            assert(itr->IsObject());
            const Value& JsonValue = *itr;

            AddNewLabel(
                JsonValue["id"].GetString(),
                JsonValue["text"].GetString(),
                JsonValue["font"].GetString(),
                JsonValue["fontSize"].GetInt(),
                { static_cast<uint8_t>(JsonValue["r"].GetInt()), static_cast<uint8_t>(JsonValue["g"].GetInt()), static_cast<uint8_t>(JsonValue["b"].GetInt()) },
                { JsonValue["x"].GetFloat(), JsonValue["y"].GetFloat(), static_cast<uint8_t>(JsonValue["z"].GetInt()) });
        }
    }

    // Sprites
    if (JsonDocument.HasMember("sprites"))
    {
        const Value& sprites = JsonDocument["sprites"];
        assert(sprites.IsArray());
        for (Value::ConstValueIterator itr = sprites.Begin(); itr != sprites.End(); ++itr)
        {
            assert(itr->IsObject());
            const Value& JsonValue = *itr;

            AddNewSprite(
                JsonValue["id"].GetString(),
                JsonValue["file"].GetString(),
                { static_cast<uint8_t>(JsonValue["r"].GetInt()), static_cast<uint8_t>(JsonValue["g"].GetInt()), static_cast<uint8_t>(JsonValue["b"].GetInt()) },
                { JsonValue["x"].GetFloat(), JsonValue["y"].GetFloat(), static_cast<uint8_t>(JsonValue["z"].GetInt()) });
        }
    }

    // Groups
    if (JsonDocument.HasMember("groups"))
    {
        const Value& groups = JsonDocument["groups"];
        assert(groups.IsArray());

        for (Value::ConstValueIterator itr = groups.Begin(); itr != groups.End(); ++itr)
        {
            assert(itr->IsObject());
            const Value& JsonValue = *itr;

            APGroupProperties gp;
            gp.id = JsonValue["id"].GetString();
            gp.layout = JsonValue["layout"].GetString();
            gp.groupSize = JsonValue["size"].GetInt();
            gp.rowSize = JsonValue["rowsize"].GetInt();
            gp.spacing = { JsonValue["spacing_x"].GetFloat(), JsonValue["spacing_y"].GetFloat(), 0 };
            gp.color = { static_cast<uint8_t>(JsonValue["r"].GetInt()), static_cast<uint8_t>(JsonValue["g"].GetInt()), static_cast<uint8_t>(JsonValue["b"].GetInt()) };
            gp.position = { JsonValue["x"].GetFloat(), JsonValue["y"].GetFloat(), static_cast<uint8_t>(JsonValue["z"].GetInt()) };
            gp.type = JsonValue["type"].GetString();

            if (strcmp(gp.type, "sprite") == 0)
            {
                gp.fileName = JsonValue["file"].GetString();
            }
            else if (strcmp(gp.type, "label") == 0)
            {
                gp.font = JsonValue["font"].GetString();
                gp.text = JsonValue["text"].GetString();
                gp.fontSize = JsonValue["fontSize"].GetFloat();
            }

            AddNewGroup(gp);
        }
    }

#   ifdef _PROFILING
    auto   FinishTime = chrono::high_resolution_clock::now();
    auto   ElapsedTime = chrono::duration_cast<chrono::milliseconds>(FinishTime - StartTime);
    float  ElapsedSeconds = ElapsedTime.count() * 0.001f;
    d_printgood("[%s] Added %d Actors in %.3f seconds\n", __FUNCTION__, static_cast<int>(m_Actors.size() - StartSize), ElapsedSeconds);
#   endif
}

/**
*   Goes through the actor pool and has each actor render itself
*/
void CLActorPool::RenderActors()
{
    for (auto const& record : m_Actors)
    {
        CLAActor* pActor = record.second;
        pActor->Render();
    }
}

/**
*   Updates the actor pool.
*/
void CLActorPool::Update(float dt)
{
    // Sort actors for rendering
    if (m_bSortOnUpdate)
    {
        SortActorsForRendering();
        m_bSortOnUpdate = false;
    }

    // Update actors
    int ticks = 0;
    for (auto const& record : m_Actors)
    {
        uint32_t id = record.first;
        ticks++;

        if (id == m_DelayedDestroyHashId)
        {
            // Actor was flagged to be destroyed
            DestroyActor(id);
        }
        else
        {
            CLAActor* pActor = record.second;

            if (pActor != nullptr)
            {
                if (!pActor->IsAlive())
                {
                    // Destroy dead actors
                    DestroyActor(id);
                }
                else
                {
                    // Update actor
                    pActor->Update(dt);
                }
            }
        }
    }
}

/**
*   Adds a new label to the actor pool.
*       /param id The actor's identifier
*       /param text Text that the label will display
*       /param font Font name for the label
*       /param size Font size
*       /param color Text color
*       /param pos Label position
*/
void CLActorPool::AddNewLabel(const char* id, const char* text, const char* font, float size, CLColor3 color, CLPos pos)
{
    CLALabel Label;
    Label.Create(text, font, size, color, pos);
    Label.SetColorMod(color);
    AddActor(id, Label);
}

/**
*   Adds a new sprite to the actor pool.
*       /param id The actor's identifier
*       /param filename The sprite's image file name
*       /param color Text color
*       /param pos Label position
*/
void CLActorPool::AddNewSprite(const char* id, const char* filename, CLColor3 color, CLPos pos)
{
    CLASprite Sprite;
    Sprite.Create(filename, pos, color);
    AddActor(id, Sprite);
}

/**
*   Adds a new group of actors to the actor pool based on parameters in the
*   passed in group properties. Groups are copies of the same actor, positioned in
*   various layouts.
*       /param props APGroupProperties for creating and positioning actors in a group.
*/
void CLActorPool::AddNewGroup(APGroupProperties& props)
{
    CLPos GroupPosition  = props.position;
    CLPos OffsetPosition = { props.spacing.x, props.spacing.y, 0 };
    int   ActorsPerRow   = props.rowSize;
    bool  LayoutGrid     = !strcmp(props.layout, "grid");
    bool  LayoutLine     = !strcmp(props.layout, "line");

    // Hash actor types and the group type for faster comparisons
    int32_t SpriteType_Hash = HashId("sprite");
    int32_t LabelType_Hash = HashId("label");
    int32_t GroupType_Hash = HashId(props.type);

    int Row = 0;
    char ActorId[256];
    for (int i = 0; i < props.groupSize; ++i)
    {
        int ActorNumber = i + 1;

        // Set position
        if (LayoutLine)
        {
            props.position.x = GroupPosition.x + (OffsetPosition.x * i);
            props.position.y = GroupPosition.y + (OffsetPosition.y * ActorNumber);
        }
        else if (LayoutGrid)
        {
            int Column = ActorNumber % ActorsPerRow;
            if (Column == 0)
            {
                Column = ActorsPerRow;
            }

            if ((ActorNumber % ActorsPerRow) == 1)
            {
                // Start of a row
                Row++;
                props.position.x = GroupPosition.x;
                props.position.y = GroupPosition.y + (OffsetPosition.y * (Row - 1));
            }
            else
            {
                // Rest of a row
                props.position.x = GroupPosition.x + (OffsetPosition.x * (Column - 1));
                props.position.y = GroupPosition.y + (OffsetPosition.y * (Row - 1));
            }
        }

        // Add actor
        sprintf_s(ActorId, sizeof(ActorId), "%s_%d", props.id, ActorNumber);
        if (GroupType_Hash == SpriteType_Hash)
        {
            AddNewSprite(ActorId, props.fileName, props.color, props.position);
        }
        else if (GroupType_Hash == LabelType_Hash)
        {
            AddNewLabel(ActorId, props.text, props.font, props.fontSize, props.color, props.position);
        }
    }
}

/**
*   Looks up an actor record in the actor pool and return an iterator to it
*       /param id The actor's hash identifier
*       /return An iterator to this APRecord for the actor, or end()
*/
APIterator CLActorPool::FindRecord(uint32_t id)
{
    for (APIterator it = m_Actors.begin(); it != m_Actors.end(); ++it)
    {
        uint32_t  ActorId = it->first;
        CLAActor* pActor  = it->second;

        if (id == ActorId)
        {
            return it;
        }
    }

    d_printwarn("[%s][WARNING!] Record for actor %lu not found.\n", _FUNC, id);
    return m_Actors.end();
}

/**
*   Sorts actors according to their rendering order by performing
*   a counting sort on the CLAActor's render layer.
*/
void CLActorPool::SortActorsForRendering()
{
    const uint16_t NumActors = static_cast<uint16_t>(m_Actors.size());
    const uint8_t  NumLayers = m_pRenderer->GetLayerCount();

    // Create a histogram counting number of times each layer is used
    vector<uint16_t> LayerCount(NumLayers, 0);
    for (uint16_t i = 0; i < NumActors; i++)
    {
        uint8_t RenderLayer = m_Actors[i].second->GetRenderLayer();
        LayerCount[RenderLayer] += 1;
    }

    // Perform a cumulative sum on the layer counts
    for (uint16_t i = 1; i < NumLayers; i++)
    {
        LayerCount[i] += LayerCount[i - 1];
    }

    // Create the sorted pool
    vector<APRecord> sorted(NumActors);
    for (uint16_t i = 0; i < NumActors; i++)
    {
        uint8_t  RenderLayer = m_Actors[i].second->GetRenderLayer();
        uint16_t ActorsRemainingOnLayer = LayerCount[RenderLayer];

        sorted[ActorsRemainingOnLayer - 1] = m_Actors[i];
        LayerCount[RenderLayer]--;
    }

    // Copy sorted pool to main pool
    m_Actors = sorted;
}

/**
*   Uses the FNV 1-a hash algorithm to convert the actor's string id
*   to an integer
*       /param id A string to lookup the actor with
*       /return A 32 bit unique identifier
*/
uint32_t CLActorPool::HashId(const char* id)
{
    uint32_t const Prime  = 16777619U;
    uint32_t const Offset = 2166136261U;
    uint32_t       Hash   = Offset;

    for (uint32_t i = 0; i < strlen(id); ++i)
    {
        Hash = Hash ^ id[i];
        Hash = Hash * Prime;
    }

    return Hash;
}