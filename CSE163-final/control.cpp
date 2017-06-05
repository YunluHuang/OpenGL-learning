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

float rotateAmount = 1 / PI;
float moveSpeed = 0.05f;
float mouseSpeed = 0.1f;
float limitedFPS = 1.0f / 60.0f;

int oldX = width / 2;
int oldY = height / 2;

void keyboard(unsigned char key, int x, int y) {
    vec3 xAxis = -normalize(cross(eye - center, up)); xAxis.y = 0;
    vec3 yAxis = normalize(up);
    vec3 zAxis = -normalize(eye - center); zAxis.y = 0;
    switch (key) {
        case 'w': {
            center += moveSpeed * zAxis;
            eye += moveSpeed * zAxis;
            break;
        }
        case 's': {
            center -= moveSpeed * zAxis;
            eye -= moveSpeed * zAxis;
            break;
        }
        case 'a': {
            center -= moveSpeed * xAxis;
            eye -= moveSpeed * xAxis;
            break;
        }
        case 'd': {
            center += moveSpeed * xAxis;
            eye += moveSpeed * xAxis;
            break;
        }
        case ' ': {
            center += moveSpeed * yAxis;
            eye += moveSpeed * yAxis;
            break;
        }
        case 'x': {
            center -= moveSpeed * yAxis;
            eye -= moveSpeed * yAxis;
            break;
        }
        case 27: {
            exit(1);
            break;
        }
        default: {
            cout << "invalid key" << endl;
            break;
        }
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == 3 || button == 4) {
        
        // Disregard up event
        if (state == GLUT_UP) {
            return;
        }
        
        // Calculate
        vec3 delta = 0.5f * normalize(center - eye);
        if (button == 3) {
            eye += delta;
        }
        else {
            eye -= delta;
        }
        
        glutPostRedisplay();
    }
}

void mouseRotate(int x, int y) {
    // press left button
    // cout << "oldx = " << oldX << ", x = " << x << endl;
    
    float rotateX = -mouseSpeed * limitedFPS * float (x - width / 2);
    float rotateY = -mouseSpeed * limitedFPS * float (y - height / 2);
    
    vec3 diff = center - eye;
    vec3 eulerAngle = vec3(rotateY, rotateX, 0.0f);
    quat quaternion = quat(eulerAngle);
    mat4 rotationM = glm::toMat4(quaternion);
    diff = vec3(rotationM * vec4(diff, 0.0f));
    center = eye + diff;
    
    CGPoint warpPoint = CGPointMake(windowX + width / 2, windowY + height / 2);
    CGWarpMouseCursorPosition(warpPoint);
    CGAssociateMouseAndMouseCursorPosition(true);
    
    oldX = x;
    oldY = y;
    glutPostRedisplay();
}

void mouseMove(int x, int y) {
    oldX = x;
    oldY = y;
    glutPostRedisplay();
}
