#include "Main.h"

void updateWagon() {
    if (!wagon.isConnected) {
        float distance = abs(train_position - wagon.position);
        if (distance < 1.0f && connectFlag) {
            wagon.isConnected = true;
        }
    }
    
    if (wagon.isConnected) {
        wagon_rotation -= rotation_speed;
        if (wagon_rotation > 360) wagon_rotation -= 360;
        wagon.position = train_position - 1.0f;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float eye_x = train_position + camera_distance * cos(camera_angle_h) * cos(camera_angle_v);
    float eye_y = camera_distance * sin(camera_angle_v);
    float eye_z = camera_distance * sin(camera_angle_h) * cos(camera_angle_v);
    gluLookAt(eye_x, eye_y, eye_z, train_position, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    drawGrass();
    railroad.draw();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    GLfloat ambientLight[] = {0.4f, 0.4f, 0.4f, 1.0f}, diffuseLight[] = {0.8f, 0.8f, 0.8f, 1.0f}, specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f}, lightPosition[] = {1.0f, 2.0f, 1.0f, 0.0f};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    trees.draw();
    wagon.draw();
    train.draw();
    smoke.draw();

    glutSwapBuffers();
}

void timer(int) {
    static bool SteamCondition = true;
    static bool StopCondition = true;
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
    
    if (rotation_speed < 15) {
        if (keyFlagW)
            rotation_speed += 0.01f;
        else if (keyFlagS)
            rotation_speed -= 0.01f;
    }
    
    if (!keyFlagW && !keyFlagS) {
        if (rotation_speed > 0)
            rotation_speed -= 0.007f;
        else if (rotation_speed < 0)
            rotation_speed += 0.007f;
    }
    
    if (!stopFlag) rotation_angle -= rotation_speed;
    if (rotation_angle > 360) rotation_angle -= 360;
    
    train_position += 0.01f * rotation_speed;
        
    if (SteamCondition && std::abs(rotation_speed) > 4) {
        playAudio(source2, buffer2, AL_TRUE);
        SteamCondition = false;
    } else if (!SteamCondition && std::abs(rotation_speed) < 4) {
        stopAudio(source2);
        SteamCondition = true;
    }
    
    if (stopFlag && StopCondition) {
        if (std::abs(rotation_speed) > 0.5f) playAudio(source3, buffer3, AL_TRUE);
        StopCondition = false;
    } else if (!stopFlag && !StopCondition) {
        stopAudio(source3);
        StopCondition = true;
    }
    
    updateWagon();
    
    smoke.update();
    railroad.update(train_position);
    trees.update(train_position);

}

bool isMachineZero(float value) {
    return std::abs(value) < std::numeric_limits<float>::epsilon();
}

void keyboard(uchar_t key, int x, int y) {
    switch (key) {
        case 'w':
            keyFlagW = true;
            break;
        case 's':
            keyFlagS = true;
            break;
        case 'q':
            stopFlag = true;
            if (!isMachineZero(rotation_speed)) rotation_speed /= 1.5f;
            break;
    }
}

void keyboardUp(uchar_t key, int x, int y) {
    static bool WireframeFlag = true;
    
    switch (key) {
        case 'w':
            keyFlagW = false;
            break;
        case 's':
            keyFlagS = false;
            break;
        case 'q':
            stopFlag = false;
            break;
        case 'c':
            connectFlag = connectFlag ? false : true;
            if (!connectFlag) wagon.isConnected = false;
            break;
        case 'e':
            WireframeFlag ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            WireframeFlag = WireframeFlag ? false : true;
            break;
        case 'r':
            playAudio(source1, buffer1, AL_FALSE);
            break;
            
    }
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouse_left_down = true;
            last_mouse_x = x;
            last_mouse_y = y;
        } else {
            mouse_left_down = false;
        }
    }
}

void mouseMotion(int x, int y) {
    if (mouse_left_down) {
        camera_angle_h += (x - last_mouse_x) * 0.005f;
        camera_angle_v += (y - last_mouse_y) * 0.005f;
        camera_angle_v = std::clamp(camera_angle_v, -1.5f, 1.5f);
        last_mouse_x = x;
        last_mouse_y = y;
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    gtextureID = loadTexture("textures/Grass.bmp");
}


int main(int argc, char** argv) {
// std::cerr << "Program started" << std::endl;
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Train Lab");
    
    init();
    initOpenAL();
    
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    
    glutMainLoop();
    cleanUpOpenAL();
// std::cerr << "Program ended" << std::endl; // for debug
    return 0;
}

