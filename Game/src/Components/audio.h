#pragma once

#include <bass/bass.h>

#include <string>

#include "../sound.h"
#include "../logger.h"

struct Audio {
    Audio(std::string filename, bool playOnStart = true, bool ifLoop = false, std::string tag = "") 
        : filename(filename), tag(tag), playOnStart(playOnStart) {
		
        sound = new Sound(filename, ifLoop);
        sound->setUpSound();
        sound->loop(ifLoop);
	}

    ~Audio() {
        sound->cleanUp();
        delete sound;
    }

    Sound* sound;
    std::string filename;
    std::string tag;
    float volume = 1;
    bool playOnStart;
};
