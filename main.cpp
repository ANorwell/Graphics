#if 0
#include <windows.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <string>

#include "camera.h"
#include "SubdivScene.h"
#include "Background.h"

#include "Util.h"


int pause;

int Width = 600;
int Height = 400;
float Fovy = 35.0;

SubdivScene scene;

ShaderLoader* shader; 


Camera* camera; //the camera that controls the scene.

//Whether each movement key is down.  Allows for a smoothly moving camera->
bool LEFT_ARROW = false;
bool RIGHT_ARROW = false;
bool UP_ARROW = false;
bool DOWN_ARROW = false;
bool W_KEY = false;
bool A_KEY = false;
bool S_KEY = false;
bool D_KEY = false;

//for selection by mouse
bool SELECT_KEY = false;
bool SELECT_MOVE = false; //the selection mode
int SELECT_X;
int SELECT_Y;


GLuint v,f,f2,p;
float lpos[4] = {1,0.5,1,0};


void init();


void reshapeCallback(int width, int height) {
	Width = width;
	Height = height;
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(Fovy, (GLfloat) width/height, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glutPostRedisplay();
}


void keyCallback(unsigned char key, int x, int y)
{
  switch(key) {
  case 'q': 
    exit(0);
    break;
  case 'p':
    glutReshapeWindow(800,600);
    break;
  case 'w':
    W_KEY = true;
    break;
  case 'a':
    A_KEY = true;
    break;
  case 's':
    S_KEY = true;
    break;
  case 'd':
    D_KEY = true;
    break;
  case '+':
  case '=':
    scene.nextScenario();
    init();
    break;
  case 'n':
    scene.split();
    break;
  case 'm':
    scene.nextMode();
    break;
  case 'v':
    shader->nextShader(GL_VERTEX_SHADER);
    break;
  case 'f':
    shader->nextShader(GL_FRAGMENT_SHADER);
    break;
  case 'i':
     SELECT_MOVE = !SELECT_MOVE;
     break;

  default:
    break;
  }
  glutPostRedisplay();
}

void upKeyCallback(unsigned char key, int x, int y) {
  switch (key) {
  case 'w':
    W_KEY = false;
    break;
  case 'a':
    A_KEY = false;
    break;
  case 's':
    S_KEY = false;
    break;
  case 'd':
    D_KEY = false;
    break;
  default:
    break;
  }
}

void specialKeyCallback(int key, int x, int y) {

  switch (key) {
  case GLUT_KEY_UP:
    UP_ARROW = true;
    break;
  case GLUT_KEY_DOWN:
    DOWN_ARROW = true;
    break;
  case GLUT_KEY_LEFT:
    LEFT_ARROW = true;
    break;
  case GLUT_KEY_RIGHT:
    RIGHT_ARROW = true;
    break;
  }
}

void upSpecialKeyCallback(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_UP:
    UP_ARROW = false;
    break;
  case GLUT_KEY_DOWN:
    DOWN_ARROW = false;
    break;
  case GLUT_KEY_LEFT:
    LEFT_ARROW = false;
    break;
  case GLUT_KEY_RIGHT:
    RIGHT_ARROW = false;
    break;
  }
}


void mouseCallback(int key, int state, int x, int y) {
  if ((key == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) ) {
    //glut sees (0,0) as the upper left corner, while opengl has it as the 
    //lower left.
    SELECT_X = x;
    SELECT_Y = Height - y;
    SELECT_KEY = true;
  }
}

void handleMovementKeys() {

  if (W_KEY)
    camera->moveForward(camera->getSpeed());
  if (A_KEY)
    camera->moveLeft(camera->getSpeed());
  if (S_KEY)
    camera->moveForward(-camera->getSpeed());
  if(D_KEY)
    camera->moveLeft(-camera->getSpeed());

  if (UP_ARROW)
    camera->rotateUp(camera->getAngleSpeed());
  if (DOWN_ARROW)
    camera->rotateUp(-camera->getAngleSpeed());
  if (LEFT_ARROW)
    camera->rotateLeft(camera->getAngleSpeed());
  if (RIGHT_ARROW)
    camera->rotateLeft(-camera->getAngleSpeed());
}


void handleSelection() {
  if (SELECT_KEY) {
    if (SELECT_MOVE) {
      if(scene.selectVertex(SELECT_X, SELECT_Y, Width, Height, Fovy))
        scene.moveVertexToCamera(scene.getSelectedVertex());
    }
    else
      if(scene.selectVertex(SELECT_X, SELECT_Y, Width, Height, Fovy))
        scene.getSelectedVertex()->getData().colour(1.0,1.0,1.0);
  } 
  SELECT_KEY = false;
}

void idleCallback() {
	glutPostRedisplay();
}

//////////////////////////////////////
//  The display callback
//////////////////////////////////////
void displayCallback(void) {

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //move
  handleMovementKeys();

  //place camera
  camera->update();

  scene.lighting();

  if (SELECT_KEY) {
    handleSelection();
  }

  scene.draw();
  
  glutSwapBuffers();
}

////////////////////////////////////////////////
//  Initialize the scene (OpenGl parameters)
////////////////////////////////////////////////
void init(void) 
{
    //set the background to be off-black
    glClearColor (0.1f, 0.1f, 0.2f, 1.0);

    //some basic settings enabled
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    //light1 used by some scenes
    glEnable(GL_LIGHT1);

    camera = &scene.getCamera();
    scene.init();

}



void setupGLUT(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Shader Test");

  //glutFullScreen();
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
	glutIdleFunc(idleCallback);

    glutKeyboardFunc(keyCallback);
    glutSpecialFunc(specialKeyCallback);
    glutKeyboardUpFunc(upKeyCallback);
    glutSpecialUpFunc(upSpecialKeyCallback);
    glutMouseFunc(mouseCallback);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2,0.2,0.1,1.0);
}




void loadShaders() {

    shader = new ShaderLoader();

    shader->addShader(GL_VERTEX_SHADER, "v_shader_basic.glsl");
    shader->checkErrors();
    shader->addShader(GL_VERTEX_SHADER, "v_shader_bumpy.glsl");
    shader->checkErrors();

    shader->addShader(GL_VERTEX_SHADER, "v_shader_no_shader.glsl");

    shader->addShader(GL_FRAGMENT_SHADER, "f_shader_basic.glsl");
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
    Log::configure("shader.log", Log::DEBUG);

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
#endif
