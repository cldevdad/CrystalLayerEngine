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
#ifndef _INCLUDE_CLACTORPOOL_H_
#define _INCLUDE_CLACTORPOOL_H_

#include "..\Actors\CLAActor.h"
#include "..\Actors\CLALabel.h"
#include "..\Actors\CLASprite.h"
#include "..\Actors\CLAParticles.h"
#include "CLTypes.h"
#include <vector>
#include "rapidjson.h"

//! Actor pool record type (string identifier/actor pointer pair)
typedef std::pair<uint32_t, CLAActor*>    APRecord;
//! Actor pool record iterator type
typedef std::vector<APRecord>::iterator   APIterator;

/**
*   Group properties for adding a group of actors. This will become
*   obsolete along with the whole group system, once a scene/level
*   editor is made. Groups are really just for hand-editing JSON.
*/
class APGroupProperties
{
public:

    // Shared
    const char* id;         //<! String identifier, which will be appended with a number
    const char* type;       //<! Type of group: sprite, label, etc.
    const char* layout;     //<! Layout of group, line or grid
    int         groupSize;  //<! Number of actors in group
    int         rowSize;    //<! Number of actors in a row (for grid layout)
    CLPos       spacing;    //<! Spacing offset between actors.
    CLPos       position;   //<! Position of group, and first actor
    CLColor3    color;      //<! Color mod for actors

    // Labels
    const char* text;       //<! Text for label actors
    const char* font;       //<! Font name for label actors
    int         fontSize;   //<! Font size for label actors
    
    // Sprites
    const char* fileName;   //<! Image file name for sprite actors

    /** Default constructor that just zeros everything out. */
	DLLEXPORT
    APGroupProperties() : 
        id(""),
        type(""),
        layout(""),
        groupSize(0),
        spacing(CLPOS_ZERO),
        position(CLPOS_ZERO),
        color(CLCOLOR_WHITE),
        text(""),
        font(""),
        fontSize(0),
        fileName("") 
    { 
    }
};

/**
*   Manages the loading, storage, and access of Actor objects
*   on a Scene
*/
class CLActorPool
{
public:
    //! Constructor
	DLLEXPORT CLActorPool(CLRenderer* pRenderer);
    //! Destructor
	DLLEXPORT ~CLActorPool();

	DLLEXPORT CLAActor*       AddActor(const char* id, CLAActor& actor);              //!< Adds an actor to the pool
	DLLEXPORT CLASprite*      AddSpriteActor(const char* id, CLASprite& sprite);      //!< Adds a sprite actor to the pool
	DLLEXPORT CLALabel*       AddLabelActor(const char* id, CLALabel& label);         //!< Adds a label actor to the pool
	DLLEXPORT CLAParticles*   AddParticleActor(const char* id, CLAParticles& label);  //!< Adds a particle actor to the pool
	DLLEXPORT void            AddActorsFromFile(const char* fileName);                //!< Adds actors from an xml file

	DLLEXPORT void            DestroyActor(const char* id);                           //!< Destroys an actor by its string id
	DLLEXPORT void            DestroyActor(uint32_t id);                              //!< Destroys an actor by its unique id
	DLLEXPORT void            DestroyActorDelayed(uint32_t id);                       //!< Flags an actor to be destroyed next tick
	DLLEXPORT void            DestroyAllActors();                                     //!< Removes all actors from the pool

	DLLEXPORT CLAActor*       FindActor(const char* id);                              //!< Finds an actor by its string id
	DLLEXPORT CLALabel*       FindLabel(const char* id);                              //!< Finds a label by its string id
	DLLEXPORT CLASprite*      FindSprite(const char* id);                             //!< Finds a sprite by its string id

	DLLEXPORT void            RenderActors();                                         //!< Renders all actors in the pool
	DLLEXPORT void            Update(float dt);                                       //!< Updates all actors in the pool
	DLLEXPORT int             Size() { return m_Actors.size(); }                      //!< Returns number of actors in pool

private:
    std::vector<APRecord>  m_Actors;         //!< Container of actor records
    CLRenderer*            m_pRenderer;      //!< Pointer to the renderer for rendering actors
    bool                   m_bSortOnUpdate;  //!< True when actors need to be re-sorted

    //! Adds a new label actor to the actor pool
	DLLEXPORT void AddNewLabel(const char* id, const char* text, const char* font, int size, CLColor3 color, CLPos pos);
    //! Adds a new sprite actor to the actor pool
	DLLEXPORT void AddNewSprite(const char* id, const char* filename, CLColor3 color, CLPos pos);
    //! Adds a new group of actors to the actor pool
	DLLEXPORT void AddNewGroup(APGroupProperties& props);

    //! Returns an iterator to an actor record
	DLLEXPORT APIterator FindRecord(uint32_t id);
    //! Hashes the actor's string id to a unique int id
	DLLEXPORT uint32_t   HashId(const char* id);
    //! Sorts actors by rendering order
	DLLEXPORT void       SortActorsForRendering();

    //! This is an actor's id when set to be destroyed on next update
    uint32_t   m_DelayedDestroyHashId;
};

#endif // _INCLUDE_CLACTORPOOL_H_