#pragma once

#include <bass/bass.h>

#include <string>

#include "logger.h"

class Sound {
public:
    Sound(std::string filename, bool ifLoop = false);

    void setUpSound();
    void play();
    void pause();
    void stop();
    void loop(bool loop);
    void setVolume(float vol); 
    void cleanUp();

private:
    HSTREAM sound;
    std::string filename;       // Name of the file
    std::string storagepath;    // Path to store the sound
    bool ifLoop;                // Set if loop the sound
    bool ifReverb = false;      // Set if reverb the sound
    float volume = -1;          // Volume of the sound
};