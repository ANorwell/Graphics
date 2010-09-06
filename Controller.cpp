#include "Controller.h"
Controller* Controller::pCurrentController = NULL;

//The global Callback_ functions (required by GLUT)
void keyCallback_(unsigned char key, int x, int y) {
    Controller::getCurrentController()->push(key);
}
void upKeyCallback_(unsigned char key, int x, int y) {
    Controller::getCurrentController()->unPush(key);
}

void specialCallback_(int key, int x, int y) {
    Controller::getCurrentController()->push(key);
}

void upSpecialCallback_(int key, int x, int y) {
    Controller::getCurrentController()->unPush(key);
}

void mouseCallback_(int key, int state, int x, int y) {
    Controller::getCurrentController()->mousePush(key, state,x,y);
}
