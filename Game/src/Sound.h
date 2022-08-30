#pragma once

#include <bass/bass.h>

#include <string>

#include "logger.h"

class Sound 
{
public:
    Sound(std::string filename, bool ifLoop = false);

    void SetUpSound();
    void Play();
    void Pause();
    void Stop();
    void Loop(bool loop);
    void SetVolume(float vol); 
    void CleanUp();

private:
    HSTREAM sound;
    std::string filename;       // Name of the file
    std::string storagepath;    // Path to store the sound
    bool ifLoop;                // Set if loop the sound
    bool ifReverb = false;      // Set if reverb the sound
    float volume = -1;          // Volume of the sound
};