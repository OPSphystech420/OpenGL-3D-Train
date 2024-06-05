#include "OpenAL.h"

void loadWAVFile(const char* filename, ALuint* buffer) {
    SF_INFO sfinfo;
    SNDFILE* sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if (!sndfile) exit(EXIT_FAILURE);

    short* samples = reinterpret_cast<short*>(malloc(sfinfo.frames * sfinfo.channels * sizeof(short)));
    sf_read_short(sndfile, samples, sfinfo.frames * sfinfo.channels);
    sf_close(sndfile);

    ALenum format;
    if (sfinfo.channels == 1)
        format = AL_FORMAT_MONO16;
    else if (sfinfo.channels == 2)
        format = AL_FORMAT_STEREO16;
    else {
        free(samples);
        exit(EXIT_FAILURE);
    }

    alBufferData(*buffer, format, samples, sfinfo.frames * sfinfo.channels * sizeof(short), sfinfo.samplerate);
    free(samples);
}

void setVolume(ALuint source, float volume) {
    alSourcef(source, AL_GAIN, volume);
}

void initOpenAL() {
    device = alcOpenDevice(NULL);
    if (!device) exit(EXIT_FAILURE);
    
    context = alcCreateContext(device, NULL);
    if (!context) exit(EXIT_FAILURE);
    
    alcMakeContextCurrent(context);

    alGenBuffers(1, &buffer1);
    alGenBuffers(1, &buffer2);
    alGenBuffers(1, &buffer3);
    alGenSources(1, &source1);
    alGenSources(1, &source2);
    alGenSources(1, &source3);
    
    loadWAVFile("OpenAL/whistle.wav", &buffer1);
    loadWAVFile("OpenAL/steam.wav", &buffer2);
    loadWAVFile("OpenAL/stop.wav", &buffer3);
    
    setVolume(source1, 0.2f);
    setVolume(source2, 0.2f);
    setVolume(source3, 0.2f);
}


void playAudio(ALuint source, ALuint buffer, ALboolean loop) {
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, loop);
    alSourcePlay(source);
}


void stopAudio(ALuint source) {
    alSourceStop(source);
}

void cleanUpOpenAL() {
    alDeleteSources(1, &source1);
    alDeleteSources(1, &source2);
    alDeleteSources(1, &source3);
    alDeleteBuffers(1, &buffer1);
    alDeleteBuffers(1, &buffer2);
    alDeleteBuffers(1, &buffer3);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}
