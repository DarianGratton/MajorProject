#include "Sound.h"

Sound::Sound(string filename, bool ifLoop) : filename(filename), ifLoop(ifLoop) { }

void Sound::SetUpSound() 
{
    string fullpath = storagepath + filename;

    if (!(sound = BASS_StreamCreateFile(false, fullpath.c_str(), 0, 0, BASS_SAMPLE_MONO))) 
    {
        string errortext = "Load error: " + to_string(BASS_ErrorGetCode());
        LOG_ERROR(errortext);
    }

    if (ifLoop == true) 
        Loop(ifLoop);
    
    BASS_ChannelGetAttribute(sound, BASS_ATTRIB_VOL, &volume);
}


void Sound::Play() 
{
    if (!BASS_ChannelPlay(sound, FALSE)) 
    {
        string errortext = "Load error: " + to_string(BASS_ErrorGetCode());
        LOG_ERROR(errortext);
    }
}

void Sound::Pause() 
{
    if (!BASS_ChannelPause(sound)) 
    {
        string errortext = "Load error: " + to_string(BASS_ErrorGetCode());
        LOG_ERROR(errortext);
    }
}

void Sound::Stop() 
{
    if (!BASS_ChannelStop(sound)) 
    {
        string errortext = "Load error: " + to_string(BASS_ErrorGetCode());
        LOG_ERROR(errortext);
    }
}

void Sound::Loop(bool loop) 
{
    ifLoop = loop;

    if (BASS_ChannelFlags(sound, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP) == -1) 
    {
        string errortext = "Load error: " + to_string(BASS_ErrorGetCode());
        LOG_ERROR(errortext);
    }
}

void Sound::SetVolume(float vol) 
{
    volume = vol;
    BASS_ChannelSetAttribute(sound, BASS_ATTRIB_VOL, volume);
}

void Sound::CleanUp() 
{
    BASS_StreamFree(sound);
}
