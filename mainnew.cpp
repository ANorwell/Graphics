#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <string>

#include "Camera.h"
#include "SubdivScene.h"
#include "SubdivController.h"
#include "Background.h"
#include "Globals.h"
#include "Util.h"




SubdivScene scene;
ShaderLoader* shader; 
SubdivController* controller;
Camera* camera;

//////////////////////////////////////
//  The display callback
//////////////////////////////////////
void displayCallback(void) {

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  controller->control();
  //place camera
  camera->update();
  scene.lighting();

  //move
  controller->controlMouse();

  scene.draw();
  
  glutSwapBuffers();
}

void setupController() {
    controller = new SubdivController(&scene, shader);
    controller->setGLUTCallbacks();
}

////////////////////////////////////////////////
//  Initialize the scene (OpenGl parameters)
////////////////////////////////////////////////
void init(void) 
{
   glClearColor (0.1f, 0.1f, 0.2f, 1.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHT1);

   setupController();
   camera = &scene.getCamera();
   scene.init();

}

void reshapeCallback(int width, int height) {
	Globals::setWidth(width);
	Globals::setHeight(height);
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(Globals::getFovy(), (GLfloat) width/height, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glutPostRedisplay();
}

void idleCallback() {
	glutPostRedisplay();
}


void setupGLUT(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(Globals::getWidth(), Globals::getHeight());
	glutCreateWindow("Shader Test");

  //glutFullScreen();
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
	glutIdleFunc(idleCallback);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2,0.2,0.1,1.0);
}



void loadShaders() {

    shader = new ShaderLoader();

    //shader->addShader(GL_VERTEX_SHADER, "v_shader_test.glsl");
    //shader->checkErrors();

    //vshaders
    shader->addShader(GL_VERTEX_SHADER, "v_shader_random.glsl");
    shader->checkErrors();
    shader->addShader(GL_VERTEX_SHADER, "v_shader_bumpy.glsl");
    shader->checkErrors();
    shader->addShader(GL_VERTEX_SHADER, "v_shader_no_shader.glsl");

    //fshaders
    shader->addShader(GL_FRAGMENT_SHADER, "f_shader_test.glsl");
    shader->checkErrors();
    shader->addShader(GL_FRAGMENT_SHADER, "f_shader_random.glsl");
    shader->checkErrors();

    shader->addShader(GL_FRAGMENT_SHADER, "f_shader_phong.glsl");
    shader->checkErrors();

    shader->setShader(GL_VERTEX_SHADER, 0);
    shader->setShader(GL_FRAGMENT_SHADER, 0);

    shader->useProgram();
    shader->checkErrors();
}

int main(int argc, char** argv){
	setupGLUT(argc, argv);
    if (Log::configure("shader.log", Log::DEBUG)) {
        printf("Configured logging\n");
    } else {
        printf("Could not configure logging\n");
    }

    int pause;
	GLenum err = glewInit();
	if (glewIsSupported("GL_VERSION_2_0")) {
		std::cout << "Shaders available" << std::endl;
	} else {
		std::cout << "Shaders NOT available" << std::endl;
		std::cin >> pause;
		exit(1);
	}

    loadShaders();
    init();
    glutMainLoop();

    std::cin.get();
	
	return 0;
}

