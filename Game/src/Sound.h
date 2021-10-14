#pragma once

#include <bass/bass.h>

#include <string>

#include "logger.h"

using namespace std;

class Sound 
{
public:
    Sound(string filename, bool ifLoop = false);

    void SetUpSound();
    void Play();
    void Pause();
    void Stop();
    void Loop(bool loop);
    void SetVolume(float vol); 
    void CleanUp();

private:
    HSTREAM sound;
    string filename;            // Name of the file
    string storagepath;         // Path to store the sound
    bool ifLoop;                // Set if loop the sound
    bool ifReverb = false;      // Set if reverb the sound
    float volume = -1;          // Volume of the sound
};