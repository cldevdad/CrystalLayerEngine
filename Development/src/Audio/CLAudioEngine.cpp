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

#include "CLAudioEngine.h"
#include "..\Core\d_printf.h"

using namespace std;

#define CLAUDIO_NOTFOUND HashId("CLAudioNotFound")

// Initialize the static audio engine pointer
CLAudioEngine* CLAudioEngine::m_pAudioEngine = nullptr;

/**
*   Constructor that opens the SDL Mix audio engine and sets sound
*   and music volume
*/
CLAudioEngine::CLAudioEngine()
{
    const int Frequency = 22050;
    const int Channels  = 2;
    const int ChunkSize = 1024;

    if (Mix_OpenAudio(Frequency, MIX_DEFAULT_FORMAT, Channels, ChunkSize) < 0)
    {
        d_printerror("[%s][ERROR!] Unable to open SDL_mixer. SDL Error: %s", _FUNC, SDL_GetError());
        SDL_ClearError();
    }
    else
    {
        Mix_Volume(-1, 128);
        Mix_VolumeMusic(128);
    }
}

/**
*   Deconstructor that frees the sound chunks and music from SDL Mix audio
*/
CLAudioEngine::~CLAudioEngine()
{
    // Free sounds
    for (SoundRecord record : m_Sounds)
    {
        Mix_Chunk* pSound = record.second;
        if (pSound != nullptr)
        {
            Mix_FreeChunk(pSound);
            pSound = nullptr;
        }
    }

    // Free music
    for (MusicRecord record : m_MusicTracks)
    {
        Mix_Music* pTrack = record.second;
        if (pTrack != nullptr)
        {
            Mix_FreeMusic(pTrack);
            pTrack = nullptr;
        }
    }

    Mix_Quit();
}

/**
*   Adds a sound to the audio engine by loading it with SDL Mix audio and then
*   storing it in the sound records vector.
*       /param file The sound file (.wav) to load
*/
UINT32 CLAudioEngine::AddSoundEffect(const char* file)
{
    // If sound already exists just return that id
    uint32_t id = HashId(file);
    if (FindSoundRecord(id) != CLAUDIO_NOTFOUND)
    {
        d_printf("[%s] Using cached sound \"%s\" (hash: %lu)\n", _FUNC, file, id);
        return id;
    }

    // Create Sound
    Mix_Chunk* pSound = Mix_LoadWAV(file);
    if (pSound == nullptr)
    {
        d_printerror("[%s][ERROR!] Couldn't load sound \"%s\". Mix Error: %s\n", _FUNC, file, Mix_GetError());
        Mix_ClearError();
        return 0;
    }

    // Create a record with hashed filename id and sound pointer
    
    SoundRecord record(id, pSound);
    m_Sounds.push_back(record);

    d_printf("[%s] Added sound \"%s\" (hash: %lu)\n", _FUNC, file, id);
    return id;
}

/**
*   Adds music to the audio engine by loading it with SDL Mix audio and then
*   storing it in the music records vector.
*       /param file The music file (.mp3) to load
*/
uint32_t CLAudioEngine::AddMusic(const char* file)
{
    // If music already exists just return that id
    uint32_t id = HashId(file);
    if (FindMusicRecord(id) != CLAUDIO_NOTFOUND)
    {
        d_printf("[%s] Using cached music \"%s\" (hash: %lu)\n", _FUNC, file, id);
        return id;
    }

    // Create music
    Mix_Music* pMusic = Mix_LoadMUS(file);
    if (pMusic == nullptr)
    {
        d_printerror("[%s][ERROR!] Couldn't load music \"%s\". Mix Error: %s\n", _FUNC, file, Mix_GetError());
        Mix_ClearError();
    }

    // Create a record with hashed filename id and music pointer
    MusicRecord record(id, pMusic);
    m_MusicTracks.push_back(record);

    d_printf("[%s] Added music \"%s\" (hash: %lu)\n", _FUNC, file, id);
    return id;
}

/**
*   Plays a sound that was loaded into the audio engine.
*       /param id The sound record's identifier
*/
void CLAudioEngine::PlaySoundEffect(uint32_t id)
{
    Mix_PlayChannel(-1, FindSoundEffect(id), 0);
}

/**
*   Plays a music file that was loaded into the audio engine.
*       /param id The music record's identifier
*/
void CLAudioEngine::PlayMusic(uint32_t id)
{
    Mix_PlayMusic(FindMusicTrack(id), -1);
}

/**
*   Pauses music playing
*/
void CLAudioEngine::PauseMusic()
{
    Mix_PauseMusic();
}

/**
*   Resumes a music file that was loaded into the audio engine.
*/
void CLAudioEngine::ResumeMusic()
{
    Mix_ResumeMusic();
}

/**
*   Stops a music playing
*/
void CLAudioEngine::StopMusic()
{
    Mix_HaltMusic();
}

/**
*   Uses the FNV 1-a hash algorithm to convert the a sound's string id
*   to an integer
*       /param id A string to lookup the actor with
*       /return A 32 bit unique identifier
*/
uint32_t CLAudioEngine::HashId(const char* id)
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

/**
*   Locates a sound by its record id and returns its pointer
*       /param id The SDL Mix audio chunk
*/
Mix_Chunk* CLAudioEngine::FindSoundEffect(uint32_t id)
{
    for (SoundRecord record : m_Sounds)
    {
        uint32_t RecordId = record.first;
        if (id == RecordId)
        {
            Mix_Chunk* pSound = record.second;
            return pSound;
        }
    }

    return nullptr;
}

/**
*   Locates a music track by its record id and returns its pointer
*       /param id The SDL Mix music chunk
*/
Mix_Music* CLAudioEngine::FindMusicTrack(uint32_t id)
{
    for (MusicRecord record : m_MusicTracks)
    {
        uint32_t RecordId = record.first;
        if (id == RecordId)
        {
            Mix_Music* pTrack = record.second;
            return pTrack;
        }
    }

    return nullptr;
}

uint32_t CLAudioEngine::FindSoundRecord(uint32_t id)
{
    for (SoundRecord record : m_Sounds)
    {
        uint32_t RecordId = record.first;
        if (id == RecordId)
        {
            return id;
        }
    }

    return CLAUDIO_NOTFOUND;
}

uint32_t CLAudioEngine::FindMusicRecord(uint32_t id)
{
    for (MusicRecord record : m_MusicTracks)
    {
        uint32_t RecordId = record.first;
        if (id == RecordId)
        {
            return id;
        }
    }

    return CLAUDIO_NOTFOUND;
}

