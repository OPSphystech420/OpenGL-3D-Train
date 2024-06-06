#ifndef OpenGL_h
#define OpenGL_h

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#define GL_SILENCE_DEPRECATION
#include <GL/glut.h>

extern float rotation_angle, train_position, rotation_speed, wagon_rotation;
extern bool stopFlag;
const float wheel_radius = 0.15f;

struct SmokeParticle {
    float x, y, z, size, alpha, xSpeed, ySpeed, zSpeed;
    
    SmokeParticle(float x, float y, float z) : x(x), y(y), z(z), size(0.1f), alpha(1.0f) {
        xSpeed = static_cast<float>(rand()) / RAND_MAX * 0.02f - 0.01f;
        ySpeed = static_cast<float>(rand()) / RAND_MAX * 0.02f;
        zSpeed = static_cast<float>(rand()) / RAND_MAX * 0.02f - 0.01f;
    }
    
    float distanceToCamera(float cx, float cy, float cz) const {
        return std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy) + (z - cz) * (z - cz));
    }
};

class Smoke {
public:
    void deallocate();
    void update();
    void draw();

private:
    std::vector<SmokeParticle> particles;
};

struct TreePosition {
    float x;
    float z;
    int cones;

    TreePosition(float x, float z, int cones) : x(x), z(z), cones(cones) {}
};

class SpruceTree {
public:
    SpruceTree() {
        std::srand(static_cast<uint32_t>(time(0)));
        for (float i = -30.0f; i <= 30.0f; i += 5.0f) {
            trees_positions.push_back(generateRandomPosition(i));
        }
    }

    void update(float train_position);

    void draw();

private:
    std::vector<TreePosition> trees_positions;

    TreePosition generateRandomPosition(float x);
    void drawTree(const TreePosition& pos);
    void updatePositions(std::vector<TreePosition>& positions, float train_position, float step);
};

class Railroad {
public:
    Railroad() {
        for (float i = -20.0f; i <= 20.0f; i += 0.5f) {
            ties_positions.push_back(i);
        }

        for (float i = -20.0f; i <= 20.0f; i += 5.0f) {
            rails_positions.push_back(i);
        }
    }

    void update(float train_position);
    void draw();

private:
    std::vector<float> ties_positions;
    std::vector<float> rails_positions;

    void drawRail(float x, float z);
    void drawTie(float x);
    void updatePositions(std::vector<float>& positions, float train_position, float step) ;
};

class Wheel {
public:
    void drawSpokes(float radius);
    void draw(float x, float y, float z, float rot_angle);
    void drawCylinderBetweenPoints(float x1, float y1, float z1, float x2, float y2, float z2, float radius);
    void drawUnderbody();
};

struct Sphere {
    float x, y, z;
    float radius;
    float vy;
};

class Wagon : public Wheel {
public:
    float position;
    bool isConnected = false;
    float velocity = 0.0f;
    
    Wagon(float pos) : position(pos) {}
    
    void applyImpulse(float impulse) {
        velocity += impulse;
    }
    
    void draw();
    
private:
    std::vector<Sphere> spheres;
    
    int numSpheres = 30;
    float gravity = -0.005f;
    float maxShakeOffset = 0.02f;
    float boxWidth = 1.0f;
    float boxHeight = 0.5f;
    float boxDepth = 0.5f;
    float sphereRadius = 0.07f;
    float shakeOffset = 0.0f;

    void initializeSpheres();
    void drawSphere(const Sphere &sphere);
    void applyGravityAndShaking(Sphere &sphere);

};

class Train : public Wheel {
public:
    void draw();

private:
    float shakeOffset = 0.0f;
    
    void drawBody();
    void drawChimney();
    void drawWheels();
    void drawConnectingRods();
    void drawCab();
};


#endif
