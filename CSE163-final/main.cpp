#define MAINPROGRAM

#include <iostream>

#include "math.hpp"

using namespace std;

int width = 960, height = 540;

void readfile(const char * filename);
void init();
void display();
void idle();
void initControl();

int main(int argc, char * argv[]) {
    
    if (argc < 2) {
        cerr << "Please specify the scene file" << endl;
        return -1;
    }
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("final-project");
    
    readfile(argv[1]);
    init();
    initControl();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    
    glutMainLoop();
}
