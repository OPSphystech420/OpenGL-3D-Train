#ifndef Main_h
#define Main_h

#include "textures/textures.h"

GLuint gtextureID;

#include "OpenAL/OpenAL.h"

ALCdevice* device;
ALCcontext* context;
ALuint buffer1, buffer2, buffer3;
ALuint source1, source2, source3;

#include "OpenGL.h"

float rotation_angle = 0.0f, train_position = 0.0f, rotation_speed = 0.0f, wagon_rotation = 0.0f, camera_angle_h = 0.0f, camera_angle_v = 0.0f, camera_distance = 5.0f;
bool mouse_left_down = false;
bool keyFlagW = false, keyFlagS = false, stopFlag = false, connectFlag = false;
int last_mouse_x, last_mouse_y;

Smoke smoke;
Railroad railroad;
Train train;
Wagon wagon(-5.0f);
SpruceTree trees;

void updateWagon();
void display();
bool isMachineZero(float value);
void keyboard(uchar_t key, int x, int y);
void keyboardUp(uchar_t key, int x, int y);
void mouseButton(int button, int state, int x, int y);
void mouseMotion(int x, int y);

#endif


