#include "sound.h"

Sound::Sound(std::string filename, bool ifLoop) : filename(filename), ifLoop(ifLoop) {

}

void Sound::setUpSound() {
    std::string fullpath = storagepath + filename;

    if (!(sound = BASS_StreamCreateFile(false, fullpath.c_str(), 0, 0, BASS_SAMPLE_MONO))) {
        std::string errortext = "Load error: " + BASS_ErrorGetCode();
        LOG_ERROR(errortext);
    }

    if (ifLoop == true) {
        loop(ifLoop);
    }
    
    BASS_ChannelGetAttribute(sound, BASS_ATTRIB_VOL, &volume);
}


void Sound::play() {
    if (!BASS_ChannelPlay(sound, FALSE)) {
        std::string errortext = "Load error: " + BASS_ErrorGetCode();
        LOG_ERROR(errortext);
    }
}

void Sound::pause() {
    if (!BASS_ChannelPause(sound)) {
        std::string errortext = "Load error: " + BASS_ErrorGetCode();
        LOG_ERROR(errortext);
    }
}

void Sound::stop() {
    if (!BASS_ChannelStop(sound)) {
        std::string errortext = "Load error: " + BASS_ErrorGetCode();
        LOG_ERROR(errortext);
    }
}

void Sound::loop(bool loop) {
    ifLoop = loop;

    if (BASS_ChannelFlags(sound, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP) == -1) {
        std::string errortext = "Load error: " + BASS_ErrorGetCode();
        LOG_ERROR(errortext);
    }
}

void Sound::setVolume(float vol) {
    volume = vol;
    BASS_ChannelSetAttribute(sound, BASS_ATTRIB_VOL, volume);
}

void Sound::cleanUp() {
    BASS_StreamFree(sound);
}