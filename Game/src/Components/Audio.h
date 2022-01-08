#pragma once

#include <bass/bass.h>

#include <string>

#include "../Sound.h"
#include "../Logger.h"

using namespace std;

struct Audio 
{
    Audio(string filename, bool playOnStart = true, bool ifLoop = false, string tag = "") 
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
    string filename;
    string tag;
    float volume = 1;
    bool playOnStart;
    bool isPlaying;
};
