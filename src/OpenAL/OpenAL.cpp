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

float CalcVol(float distance) {
    if (distance >= 30.0f) {
        return 0.1f;
    } else if (distance <= 2.0f) {
        return 0.5f;
    } else if (distance <= 5.0f) {
        float slope = (0.3f - 0.5f) / (5.0f - 2.0f);
        return 0.5f + slope * (distance - 2.0f);
    } else {
        float slope = (0.1f - 0.3f) / (30.0f - 5.0f);
        return 0.3f + slope * (distance - 5.0f);
    }
}

void VolumeDistance(float dist, float speed) {
    float volume = CalcVol(dist);
    if (speed < 7) volume -= 0.2f - speed*0.02f;
    volume = std::clamp(volume, 0.1f, 1.0f);
    setVolume(source1, volume);
    setVolume(source2, volume);
    setVolume(source3, volume);
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
    
    loadWAVFile("src/OpenAL/whistle.wav", &buffer1);
    loadWAVFile("src/OpenAL/steam.wav", &buffer2);
    loadWAVFile("src/OpenAL/stop.wav", &buffer3);
    
    setVolume(source1, 0.5f);
    setVolume(source2, 0.5f);
    setVolume(source3, 0.5f);
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
