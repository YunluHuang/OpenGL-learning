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
    vec3 xAxis = normalize(cross(eye - center, up));
    vec3 yAxis = normalize(up);
    vec3 zAxis = normalize(eye - center);
    switch (key) {
        case 'w': {
            cout << "w pressed, move forward" << endl;
            view = glm::translate(view, moveSpeed * zAxis);
            break;
        }
        case 's': {
            cout << "s pressed, move backward" << endl;
            view = glm::translate(view, moveSpeed * -zAxis);
            break;
        }
        case 'a': {
            cout << "a pressed, move left" << endl;
            view = glm::translate(view, moveSpeed * -xAxis);
            break;
        }
        case 'd': {
            cout << "d pressed, move right" << endl;
            view = glm::translate(view, moveSpeed * xAxis);
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
            //rotate horizontally counter-clockwise
            cout << "left arrow key" << endl;
            model = glm::rotate(model, rotateAmount, yAxis);
            break;
        }
            //up
        case 101: {
            //rotate vertically counter-clockwise
            cout << "up arrow key" << endl;
            model = glm::rotate(model, rotateAmount, xAxis);
            break;
        }
            //right
        case 102: {
            //rotate horizontally counter-clockwise
            cout << "right arrow key" << endl;
            model = glm::rotate(model, -rotateAmount, yAxis);
            break;
        }
            //down
        case 103: {
            //rotate vertically counter-clockwise
            cout << "down arrow key" << endl;
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
    if(button == 3 || button == 4) {
        if(state == GLUT_UP) {
            cout << "scroll up" << endl;
        }
    }
}

void mouseRotate(int x, int y) {
    //press left button
    cout << "oldx = " << oldX << ", x = " << x << endl;
    float rotateX = mouseSpeed * limitedFPS * float (x - oldX);
    float rotateY = mouseSpeed * limitedFPS * float (y - oldY);
    vec3 eulerAngle = vec3(rotateY, rotateX, 0.0f);
    quat quaternion = quat(eulerAngle);
    mat4 rotationM = glm::toMat4(quaternion);
    cout << "eye = (" << eye.x << ", " << eye.y << ", " << eye.z << ")" << endl;
    eye = vec3(rotationM * vec4(eye, 0.0f));
    view = glm::inverse(rotationM) * view;
    oldX = x;
    oldY = y;
    glutPostRedisplay();
}

void mouseMove(int x, int y) {
    oldX = x;
    oldY = y;
    glutPostRedisplay();
}
