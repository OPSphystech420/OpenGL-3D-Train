#include "OpenGL.h"

#pragma mark - Smoke

void Smoke::deallocate() {
    if (!particles.empty()) particles.clear();
}
    
void Smoke::update() {
    particles.erase(std::remove_if(particles.begin(), particles.end(),
                                   [](const SmokeParticle& p) { return p.alpha <= 0.0f || p.y > 2.0f; }), particles.end());
    for (auto& particle : particles) {
        particle.x += particle.xSpeed;
        particle.y += particle.ySpeed;
        particle.z += particle.zSpeed;
        particle.size += 0.001f;
        particle.alpha -= 0.01f;
    }
    
    if (std::abs(rotation_speed) > 0.5f) {
        particles.emplace_back(train_position + 0.3f, 0.8f, 0.0f);
    }
    
}

void Smoke::draw() {
    float cameraX = 0.0f, cameraY = 2.0f, cameraZ = 5.0f;
    std::sort(particles.begin(), particles.end(), [=](const SmokeParticle& a, const SmokeParticle& b) {
        return a.distanceToCamera(cameraX, cameraY, cameraZ) > b.distanceToCamera(cameraX, cameraY, cameraZ);
    });
    for (const auto& particle : particles) {
        glPushMatrix();
        glColor4f(0.8f, 0.8f, 0.8f, particle.alpha);
        glTranslatef(particle.x, particle.y, particle.z);
        glutSolidSphere(particle.size, 16, 16);
        glPopMatrix();
    }
}

#pragma mark - SpruceTree

void SpruceTree::update(float train_position) {
    updatePositions(trees_positions, train_position, 5.0f);
}

void SpruceTree::draw() {
    for (const auto& pos : trees_positions) {
        drawTree(pos);
    }
}

TreePosition SpruceTree::generateRandomPosition(float x) {
    float z;
    
    do {
        z = rand() % 12 - 6.0f;
    } while (z > -2.0f && z < 2.0f);
    
    int cones = rand() % 2 + 2;
    return TreePosition(x, z, cones);
}

void SpruceTree::drawTree(const TreePosition& pos) {
    glPushMatrix();
    glTranslatef(pos.x, -0.5f, pos.z);
    
    glColor3f(0.4f, 0.25f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 0.0f);
    glScalef(0.2f, 0.7f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glColor3f(0.0, 0.3, 0.0);
    for (int i = 0; i < pos.cones; ++i) {
        glPushMatrix();
        glTranslatef(0.0f, 0.6f * i, 0.0f);
        glRotatef(-90, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.5f - 0.1f * i, 1.0f, 16, 16);
        glPopMatrix();
    }
    
    glPopMatrix();
}

void SpruceTree::updatePositions(std::vector<TreePosition>& positions, float train_position, float step) {
    float track_front = train_position + 30.0f;
    float track_back = train_position - 30.0f;
    
    while (!positions.empty() && positions.front().x < track_back - step) {
        positions.erase(positions.begin());
        positions.push_back(generateRandomPosition(positions.back().x + step));
    }
    
    while (!positions.empty() && positions.back().x > track_front + step) {
        positions.pop_back();
        positions.insert(positions.begin(), generateRandomPosition(positions.front().x - step));
    }
}

#pragma mark - Railroad

void Railroad::update(float train_position) {
    updatePositions(ties_positions, train_position, 0.5f);
    updatePositions(rails_positions, train_position, 5.0f);
}

void Railroad::draw() {
    glColor3f(0.1f, 0.1f, 0.1f);
    for (float pos : rails_positions) {
        drawRail(pos, 0.3f);
        drawRail(pos, -0.3f);
    }
    glColor3f(0.3f, 0.2f, 0.1f);
    for (float pos : ties_positions) {
        drawTie(pos);
    }
}

void Railroad::drawRail(float x, float z) {
    glPushMatrix();
    glTranslatef(x, -0.5f, z);
    glScalef(5.0f, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void Railroad::drawTie(float x) {
    glPushMatrix();
    glTranslatef(x, -0.6f, 0.0f);
    glScalef(0.1f, 0.1f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void Railroad::updatePositions(std::vector<float>& positions, float train_position, float step) {
    float track_front = train_position + 20.0f;
    float track_back = train_position - 20.0f;
    
    while (!positions.empty() && positions.front() < track_back - step) {
        positions.erase(positions.begin());
        positions.push_back(positions.back() + step);
    }
    
    while (!positions.empty() && positions.back() > track_front + step) {
        positions.pop_back();
        positions.insert(positions.begin(), positions.front() - step);
    }
}

#pragma mark - Wheel

void Wheel::drawSpokes(float radius) {
    for (int i = 0; i < 6; ++i) {
        glPushMatrix();
        glRotatef(i * 60.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(0.0f, -0.02f);
        glVertex2f(radius, -0.02f);
        glVertex2f(radius, 0.02f);
        glVertex2f(0.0f, 0.02f);
        glEnd();
        glPopMatrix();
    }
}

void Wheel::draw(float x, float y, float z, float rot_angle) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rot_angle, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glutSolidTorus(0.03f, wheel_radius, 12, 36);
    glColor3f(0.5f, 0.3f, 0.7f);
    drawSpokes(wheel_radius);
    glPopMatrix();
}
    
void Wheel::drawCylinderBetweenPoints(float x1, float y1, float z1, float x2, float y2, float z2, float radius) {
    GLUquadric* quad = gluNewQuadric();
    float dx = x2 - x1, dy = y2 - y1, dz = z2 - z1;
    float length = sqrt(dx * dx + dy * dy + dz * dz);
    float ax = dz == 0 ? -dy : -dy * dz, ay = dz == 0 ? dx : dx * dz, az = dz == 0 ? 0 : length * length - dz * dz;
    float angle = atan2(sqrt(dx * dx + dy * dy), dz) * 180.0f / M_PI;
    
    glPushMatrix();
    glTranslatef(x1, y1, z1);
    glRotatef(angle, ax, ay, az);
    gluCylinder(quad, radius, radius, length, 32, 32);
    glPopMatrix();
    gluDeleteQuadric(quad);
}
    
void Wheel::drawUnderbody() {
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, -0.3f, 0.0f);
    glScalef(0.8f, 0.1f, 0.4f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

#pragma mark - Wagon

void Wagon::draw() {
    if (!stopFlag && std::abs(rotation_speed) > 1.5f && isConnected) shakeOffset = 0.005f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.001f * 15.0f);
    
    float thickness = 0.05f;
    float halfThickness = thickness / 2.0f;
    
    glColor3f(0.4f, 0.4f, 0.0f);
    glPushMatrix();
    glTranslatef(position, shakeOffset, shakeOffset);
    
    glPushMatrix();
    glTranslatef(0.0f, -0.25f + halfThickness, 0.0f);
    glScalef(1.0f, thickness, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.25f - halfThickness);
    glScalef(1.0f, 0.5f - thickness, thickness + 0.001f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -0.25f + halfThickness);
    glScalef(1.0f, 0.5f - thickness, thickness + 0.001f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.5f + halfThickness, 0.0f, 0.0f);
    glScalef(thickness + 0.001f, 0.5f - thickness, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.5f - halfThickness, 0.0f, 0.0f);
    glScalef(thickness + 0.001f, 0.5f - thickness, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    
    glColor3f(0.1f, 0.1f, 0.1f);
    if (spheres.empty()) {
        initializeSpheres();
    }
    
    for (auto &sphere : spheres) {
        applyGravityAndShaking(sphere);
        drawSphere(sphere);
    }
    
    Wheel::draw(0.3f, -0.3f, 0.35f, wagon_rotation);
    Wheel::draw(-0.3f, -0.3f, 0.35f, wagon_rotation);
    Wheel::draw(0.3f, -0.3f, -0.35f, wagon_rotation + 180.0f);
    Wheel::draw(-0.3f, -0.3f, -0.35f, wagon_rotation + 180.0f);
    
    Wheel::drawCylinderBetweenPoints(0.3f, -0.3f, 0.35f, 0.3f, -0.3f, -0.35f, 0.02f);
    Wheel::drawCylinderBetweenPoints(-0.3f, -0.3f, 0.35f, -0.3f, -0.3f, -0.35f, 0.02f);
    
    Wheel::drawUnderbody();
    
    glPopMatrix();
}

void Wagon::initializeSpheres() {
    for (int i = 0; i < numSpheres; ++i) {
        Sphere sphere;
        sphere.x = (rand() / (float)RAND_MAX - 0.5f) * (boxWidth - 2 * sphereRadius);
        sphere.y = -0.25f + sphereRadius + i * 2 * sphereRadius;
        sphere.z = (rand() / (float)RAND_MAX - 0.5f) * (boxDepth - 2 * sphereRadius);
        sphere.radius = sphereRadius;
        sphere.vy = 0;
        spheres.push_back(sphere);
    }
}

void Wagon::drawSphere(const Sphere &sphere) {
    glPushMatrix();
    glTranslatef(sphere.x, sphere.y, sphere.z);
    glutSolidSphere(sphere.radius, 20, 20);
    glPopMatrix();
}

void Wagon::applyGravityAndShaking(Sphere &sphere) {
    sphere.vy += gravity;
    sphere.y += sphere.vy;
    
    if (std::abs(rotation_speed) > 1.5f && isConnected) {
        sphere.x += (rand() / (float)RAND_MAX - 0.5f) * maxShakeOffset;
        sphere.z += (rand() / (float)RAND_MAX - 0.5f) * maxShakeOffset;
    }
    
    if (sphere.x > 0.45f - sphere.radius) sphere.x = 0.45f - sphere.radius;
    if (sphere.x < -0.45f + sphere.radius) sphere.x = -0.45f + sphere.radius;
    if (sphere.z > 0.2f - sphere.radius) sphere.z = 0.2f - sphere.radius;
    if (sphere.z < -0.2f + sphere.radius) sphere.z = -0.2f + sphere.radius;
    
    if (sphere.y < -0.25f + sphere.radius) {
        sphere.y = -0.25f + sphere.radius;
        sphere.vy = 0;
    }
    
    for (auto &other : spheres) {
        if (&sphere != &other) {
            float dx = sphere.x - other.x;
            float dy = sphere.y - other.y;
            float dz = sphere.z - other.z;
            float distance = sqrt(dx * dx + dy * dy + dz * dz);
            float minDistance = sphere.radius + other.radius;
            if (distance < minDistance) {
                float overlap = minDistance - distance;
                float adjustX = (dx / distance) * overlap / 2;
                float adjustY = (dy / distance) * overlap / 2;
                float adjustZ = (dz / distance) * overlap / 2;
                
                sphere.x += adjustX;
                sphere.y += adjustY;
                sphere.z += adjustZ;
                other.x -= adjustX;
                other.y -= adjustY;
                other.z -= adjustZ;
                
                if (adjustY > 0) {
                    sphere.vy = 0;
                }
            }
        }
    }
}

#pragma mark - Train

void Train::draw() {
    glColor3f(0.5f, 0.0f, 0.0f);
    glPushMatrix();
    
    if (!stopFlag && std::abs(rotation_speed) > 1) shakeOffset = 0.005f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.001f * 20.0f);
    
    glTranslatef(train_position, shakeOffset, shakeOffset);
    
    drawBody();
    drawChimney();
    drawWheels();
    drawConnectingRods();
    Wheel::drawUnderbody();
    drawCab();
    glPopMatrix();
}
    
void Train::drawBody() {
    glPushMatrix();
    glScalef(1.0f, 0.5f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void Train::drawChimney() {
    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0.3f, 0.7f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.1f, 0.1f, 0.6f, 32, 32);
    glColor3f(0.0f, 0.0f, 0.0f);
    gluDisk(quad, 0.0f, 0.1f, 32, 1);
    gluDeleteQuadric(quad);
    glPopMatrix();
}

void Train::drawWheels() {
    Wheel::draw(0.3f, -0.3f, 0.35f, rotation_angle);
    Wheel::draw(-0.3f, -0.3f, 0.35f, rotation_angle);
    Wheel::draw(0.3f, -0.3f, -0.35f, rotation_angle + 180.0f);
    Wheel::draw(-0.3f, -0.3f, -0.35f, rotation_angle + 180.0f);
}

void Train::drawConnectingRods() {
    glColor3f(0.8f, 0.2f, 0.2f);
    Wheel::drawCylinderBetweenPoints(0.3f + wheel_radius * cos(rotation_angle * M_PI / 180.0f), -0.3f + wheel_radius * sin(rotation_angle * M_PI / 180.0f), 0.35f,
                                     -0.3f + wheel_radius * cos((rotation_angle + 180.0f) * M_PI / 180.0f), -0.3f + wheel_radius * sin((rotation_angle + 180.0f) * M_PI / 180.0), 0.35f, 0.02f);
    Wheel::drawCylinderBetweenPoints(0.3 + wheel_radius * cos(rotation_angle * M_PI / 180.0f), -0.3f + wheel_radius * sin(rotation_angle * M_PI / 180.0f), -0.35f,
                                     -0.3f + wheel_radius * cos((rotation_angle + 180.0f) * M_PI / 180.0f), -0.3f + wheel_radius * sin((rotation_angle + 180.0f) * M_PI / 180.0), -0.35f, 0.02f);
    glColor3f(0.5f, 0.5f, 0.5f);
    Wheel::drawCylinderBetweenPoints(0.3f, -0.3f, 0.35f, 0.3f, -0.3f, -0.35f, 0.02f);
    Wheel::drawCylinderBetweenPoints(-0.3f, -0.3f, 0.35f, -0.3f, -0.3f, -0.35f, 0.02f);
}

void Train::drawCab() {
    glColor3f(0.0f, 0.0f, 0.5f);
    glPushMatrix();
    glTranslatef(-0.25f, 0.4f, 0.0f);
    glScalef(0.4f, 0.4f, 0.4f);
    glutSolidCube(1.0f);
    glPopMatrix();
}
