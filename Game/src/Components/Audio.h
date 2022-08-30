#pragma once

#include <bass/bass.h>

#include <string>

#include "../Sound.h"
#include "../Logger.h"

struct Audio 
{
    Audio(std::string filename, bool playOnStart = true, bool ifLoop = false, std::string tag = "") 
        : filename(filename), tag(tag), playOnStart(playOnStart) 
    {
        sound = new Sound(filename, ifLoop);
        sound->SetUpSound();
        sound->Loop(ifLoop);
        isPlaying = false;
	}

    ~Audio() 
    {
        sound->CleanUp();
        delete sound;
    }

    Sound* sound;
    std::string filename;
    std::string tag;
    float volume = 1;
    bool playOnStart;
    bool isPlaying;
};
