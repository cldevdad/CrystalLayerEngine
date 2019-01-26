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
#ifndef _INCLUDE_CLAUDIOENGINE_H
#define _INCLUDE_CLAUDIOENGINE_H

#include "..\Core\CLTypes.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include <vector>

typedef std::pair<uint32_t, Mix_Chunk*> SoundRecord;  //!< Sound file id/sound record
typedef std::pair<uint32_t, Mix_Music*> MusicRecord;  //!< Music file id/music record

/**
*   A basic audio engine that handles loading and playing of sound
*   and music files
*/
class CLAudioEngine
{

public:

	DLLEXPORT CLAudioEngine();
	DLLEXPORT ~CLAudioEngine();

	DLLEXPORT uint32_t    AddSoundEffect(const char* file);   //!< Add a sound to the engine
	DLLEXPORT uint32_t    AddMusic(const char* file);         //!< Add music to the engine
	DLLEXPORT void        PlaySoundEffect(uint32_t id);       //!< Play a sound
	DLLEXPORT void        PlayMusic(uint32_t id);             //!< Play music
	DLLEXPORT void        PauseMusic();                       //!< Pause music
	DLLEXPORT void        ResumeMusic();                      //!< Resume music
	DLLEXPORT void        StopMusic();                        //!< Stop music

private:

	DLLEXPORT uint32_t    HashId(const char* id);         //!< Generate a unique id for sound/music files
	DLLEXPORT Mix_Chunk*  FindSoundEffect(uint32_t id);   //!< Find a sound that was loaded
	DLLEXPORT uint32_t    FindSoundRecord(uint32_t id);   //!< Finds an added sound by id
	DLLEXPORT Mix_Music*  FindMusicTrack(uint32_t id);    //!< Find a music track that was loaded
	DLLEXPORT uint32_t    FindMusicRecord(uint32_t id);   //!< Finds an added music track by id

    std::vector<SoundRecord> m_Sounds;          //!< Records of all loaded sounds
    std::vector<MusicRecord> m_MusicTracks;     //!< Records of all loaded music

    static CLAudioEngine* m_pAudioEngine;       //!< Singleton instance of audio engine

public:

    //! Returns the single static instance
	DLLEXPORT
    static CLAudioEngine* GetEngine()
    {
        if (m_pAudioEngine == nullptr)
        {
            m_pAudioEngine = new CLAudioEngine();
        }
        return m_pAudioEngine;
    }
};

#endif // _INCLUDE_CLAUDIOENGINE_H