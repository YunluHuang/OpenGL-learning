//
//  Control.cpp
//  CSE163-final
//
//  Created by Ah Huang on 6/2/17.
//  Copyright © 2017 Ah Huang. All rights reserved.
//

#include "Control.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;

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

/**
 handle arrow key press
 */
void arrowKey(int key, int x, int y) {
    vec3 xAxis = normalize(cross(eye - center, up));
    vec3 yAxis = normalize(up);
    vec3 zAxis = normalize(eye - center);
    switch (key) {
        //left
        case 100: {
            // rotate horizontally counter-clockwise
            // cout << "left arrow key" << endl;
            model = glm::rotate(model, rotateAmount, yAxis);
            break;
        }
        // up
        case 101: {
            // rotate vertically counter-clockwise
            // cout << "up arrow key" << endl;
            model = glm::rotate(model, rotateAmount, xAxis);
            break;
        }
        //right
        case 102: {
            // rotate horizontally counter-clockwise
            // cout << "right arrow key" << endl;
            model = glm::rotate(model, -rotateAmount, yAxis);
            break;
        }
        // down
        case 103: {
            // rotate vertically counter-clockwise
            // cout << "down arrow key" << endl;
            model = glm::rotate(model, -rotateAmount, xAxis);
            break;
        }
        default: {
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
    
    float rotateX = -mouseSpeed * limitedFPS * float (x - oldX);
    float rotateY = -mouseSpeed * limitedFPS * float (y - oldY);
    
    vec3 diff = center - eye;
    vec3 eulerAngle = vec3(rotateY, rotateX, 0.0f);
    quat quaternion = quat(eulerAngle);
    mat4 rotationM = glm::toMat4(quaternion);
    // cout << "eye = (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << endl;
    diff = vec3(rotationM * vec4(diff, 0.0f));
    center = eye + diff;
    
    oldX = x;
    oldY = y;
    glutPostRedisplay();
}

void mouseMove(int x, int y) {
    oldX = x;
    oldY = y;
    glutPostRedisplay();
}