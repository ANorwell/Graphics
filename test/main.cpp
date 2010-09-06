#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <cstdio>

void displayCallback() {
    //    glClearColor (0.0,0.0,0.0,1.0); 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 5.0,
               0.0, 0.0, 0.0,
               0.0, 1.0, 0.0);     

    glBegin(GL_QUADS);
    glColor3f(0.5,0.0,0.0);
    glVertex3f(1.0,1.0,0.0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(1.0,0.0,0.0);
    glEnd();

    //    glFlush();
    glutSwapBuffers();
}

void reshapeCallback (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0)
;
    glMatrixMode (GL_MODELVIEW);
}

void idleCallback() {
	glutPostRedisplay();
}

void setupGLUT(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600, 400);
	glutCreateWindow("Test");

  //glutFullScreen();
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
	glutIdleFunc(idleCallback);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2,0.2,0.1,1.0);
}

void init(void) 
{
   glClearColor (0.1f, 0.1f, 0.2f, 1.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHT1);
}

int testInitMain(int argc, char** argv) {
    setupGLUT(argc, argv);
    
    GLenum err = glewInit();
    if (glewIsSupported("GL_VERSION_2_0")) {
        std::cout << "Shaders available" << std::endl;
    } else {
        std::cout << "Shaders NOT available" << std::endl;
        std::cin.get();
    }

    init();
    shader();
    glutMainLoop();

    return 0;
}
