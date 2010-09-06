#include "SubdivController.h"
#include "Camera.h"
#include "Globals.h"


//TODO: write a Globals class with static width/height/fovy.
//It may have the callbacks that are still in main, but may not fully.
//Controller should have the globals as a member.
//Controller also needs calls to the shader manager.


void SubdivController::handleKey(unsigned char aKey) {
    Camera& camera = pScene->getCamera();

    switch (aKey) {
  case 'q': 
      exit(0);
      break;
  case 'p':
      glutReshapeWindow(800,600);
      unPush(aKey);
      break;
  case 'w':
      camera.moveForward(camera.getSpeed());
      break;
  case 'a':
      camera.moveLeft(camera.getSpeed());
      break;
  case 's':
      camera.moveForward(-camera.getSpeed() );
      break;
  case 'd':
      camera.moveLeft(-camera.getSpeed() );
      break;
  case '+':
  case '=':
      pScene->nextScenario();
      pScene->init();
      unPush(aKey);
      break;
  case 'n':
      pScene->split();
      unPush(aKey);
      break;
  case 'm':
      pScene->nextMode();
      unPush(aKey);
      break;
  case 'v':
      pShader->nextShader(GL_VERTEX_SHADER);
      unPush(aKey);
      break;
  case 'f':
      pShader->nextShader(GL_FRAGMENT_SHADER);
      unPush(aKey);
      break;
  default:
      break;
    }
}


void SubdivController::handleSpecialKey(int key) {

    Camera& camera = pScene->getCamera();
    switch (key) {
  case GLUT_KEY_UP:
      camera.rotateUp(camera.getAngleSpeed());
      break;
  case GLUT_KEY_LEFT:
      camera.rotateLeft(camera.getAngleSpeed());
      break;
  case GLUT_KEY_DOWN:
      camera.rotateUp(-camera.getAngleSpeed());
      break;
  case GLUT_KEY_RIGHT:
      camera.rotateLeft(-camera.getAngleSpeed());
      break;      
  default:
      break;
    }
}

void SubdivController::handleMouse(int key, int state, bool newState, int x, int y) {

    Camera camera = pScene->getCamera();    

    if ((state == GLUT_DOWN) && newState ) {
        if (key == GLUT_LEFT_BUTTON) {
            if (pScene->getSelectedVertex()) {
                pScene->repositionSelectedVertex(x,y);
                pScene->unselectVertex();
            } else {
                pScene->selectVertex(x, y, Globals::getWidth(), 
                    Globals::getHeight(), Globals::getFovy());
            }
        }
        if (key == GLUT_RIGHT_BUTTON) {
            if(pScene->selectVertex(x, y, Globals::getWidth(), 
                Globals::getHeight(), Globals::getFovy()))
                pScene->getSelectedVertex()->getData().colour(1.0,1.0,1.0);
        } 
    }
}