#ifndef OpenAL_h
#define OpenAL_h

#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <sndfile.h>

extern ALCdevice* device;
extern ALCcontext* context;
extern ALuint buffer1, buffer2, buffer3;
extern ALuint source1, source2, source3;

void loadWAVFile(const char* filename, ALuint* buffer);
void setVolume(ALuint source, float volume);
void initOpenAL();
void playAudio(ALuint source, ALuint buffer, ALboolean loop);
void stopAudio(ALuint source);
void cleanUpOpenAL();

#endif
