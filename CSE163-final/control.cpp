//
//  Control.cpp
//  CSE163-final
//
//  Created by Ah Huang on 6/2/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <ApplicationServices/ApplicationServices.h>

#include "variables.h"
#include "math.hpp"

using namespace std;

float moveSpeed = 0.03f;
float mouseSpeed = 0.1f;
float limitedFPS = 1.0f / 60.0f;

float yaw = 0;
float pitch = 0;

bool activeKey[128];

int mode = 1;
const int NORMAL_MODE = 1;
const int SSAO_MODE = 2;
const int DEBUG_MODE = 3;

bool animate = false;

void processKeyboard() {
    
    vec3 xAxis = -cross(eye - center, up);
    xAxis.y = 0;
    xAxis = normalize(xAxis);
    vec3 yAxis = up;
    vec3 zAxis = center - eye;
    zAxis.y = 0;
    zAxis = normalize(zAxis);
    
    if (activeKey['w']) {
        center += moveSpeed * zAxis;
        eye += moveSpeed * zAxis;
    }
    
    if (activeKey['s']) {
        center -= moveSpeed * zAxis;
        eye -= moveSpeed * zAxis;
    }
    
    if (activeKey['a']) {
        center -= moveSpeed * xAxis;
        eye -= moveSpeed * xAxis;
    }
    
    if (activeKey['d']) {
        center += moveSpeed * xAxis;
        eye += moveSpeed * xAxis;
    }
    
    if (activeKey[' ']) {
        center += moveSpeed * yAxis;
        eye += moveSpeed * yAxis;
    }
    
    if (activeKey['x']) {
        center -= moveSpeed * yAxis;
        eye -= moveSpeed * yAxis;
    }
}

void keyboardDown(unsigned char key, int x, int y) {
    activeKey[key] = true;
    switch (key) {
        case 27: {
            exit(1);
            break;
        }
        case '1': {
            mode = NORMAL_MODE;
            break;
        }
        case '2': {
            mode = SSAO_MODE;
            break;
        }
        case '3': {
            mode = DEBUG_MODE;
            break;
        }
        case 'p': {
            animate = !animate;
            break;
        }
        default: {
            break;
        }
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    activeKey[key] = false;
}

void mouse(int button, int state, int x, int y) {
    
}

void setMousePosition(int x, int y) {
    CGPoint warpPoint = CGPointMake(windowX + x, windowY + y);
    CGWarpMouseCursorPosition(warpPoint);
    CGAssociateMouseAndMouseCursorPosition(true);
}

void centerMousePosition() {
    setMousePosition(width / 2, height / 2);
}

void mouseRotate(int x, int y) {
    
    yaw += -mouseSpeed * limitedFPS * float (x - width / 2);
    pitch += -mouseSpeed * limitedFPS * float (y - height / 2);
    
    yaw = yaw < -pi ? 2 * pi + yaw : yaw > pi ? -2 * pi + yaw : yaw;
    pitch = pitch < -pi / 2 ? -pi / 2 + 0.01 : pitch > pi / 2 ? pi / 2 - 0.01 : pitch;
    
    mat4 rotation = mat4(1.0f), identity = mat4(1.0f);
    rotation *= glm::rotate(identity, yaw, vec3(0, 1, 0));
    rotation *= glm::rotate(identity, pitch, vec3(1, 0, 0));
    
    vec3 diff = vec3(rotation * vec4(0, 0, -1.0f, 0.0f));
    center = eye + diff;
    
    centerMousePosition();
    glutPostRedisplay();
}

void mouseMove(int x, int y) {
    
}

void initKeys() {
    for (int i = 0; i < 128; i++) {
        activeKey[i] = false;
    }
}

void initControl() {
    initKeys();
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseRotate);
    glutMotionFunc(mouseRotate);
}
