#define MAINPROGRAM

#include <iostream>

#include "math.hpp"

using namespace std;

int width = 720, height = 480;

void readfile(const char * filename);
void init();
void display();
void idle();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseRotate(int x, int y);
void mouseMove(int x, int y);

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
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseRotate);
    glutMotionFunc(mouseRotate);
    
    glutMainLoop();
}
