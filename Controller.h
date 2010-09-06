#pragma once
#include <map>
#include <GL/glew.h>
#include <GL/glut.h>

#include "Globals.h"

//The global Callback_ functions (required by GLUT)
void keyCallback_(unsigned char key, int x, int y);
void upKeyCallback_(unsigned char key, int x, int y);
void specialCallback_(int key, int x, int y);
void upSpecialCallback_(int key, int x, int y);
void mouseCallback_(int key, int state, int x, int y);


/*
All this does is change from the GLUT keyboard/mouse events to 
handling everything for all pushed keys whenever control() is called.  User should 
implement handleKey(char key), handleSpecial(int key), and handleMouse(key, state, newEvent, x, y)
*/

class Controller {
protected:
    //Not a singleton, but we need a single active controller at a time.
    static Controller* pCurrentController;

    struct mouseData {
        int pushed;
        int x;
        int y;
        bool newEvent;

        mouseData() : pushed(GLUT_UP), x(0.0), y(0.0), newEvent(false) {};
        mouseData(int p, int xx, int yy) : pushed(p), x(xx), y(yy), newEvent(true) {};
    };

/*struct keyData {
    bool pushed;
    bool newEvent;
}*/

    std::map<unsigned char, bool> keyRegistry;
    std::map<int, bool> specialRegistry;
    std::map<int, mouseData> mouseRegistry;

public:

    static Controller* getCurrentController() { 
        return pCurrentController;
    }

    void push(unsigned char key) {
        keyRegistry[key] = true;
    }
    void unPush(unsigned char key) {
        keyRegistry[key] = false;
    }

    void push(int key) {
        specialRegistry[key] = true;
    }
    void unPush(int key) {
        specialRegistry[key] = false;
    }

    void mousePush(int key, int state, int x, int y) {
        //glut sees (0,0) as the upper left corner, while opengl has it as the 
        //lower left.
        if (mouseRegistry.find(key) != mouseRegistry.end()) {
            mouseRegistry[key].x = x;
            mouseRegistry[key].y = Globals::getHeight() - y;
            mouseRegistry[key].newEvent = (mouseRegistry[key].pushed != state);
            mouseRegistry[key].pushed = state;
        } else {
            mouseRegistry[key] = mouseData(state, x, y);
        }

    }

    void setGLUTCallbacks() {
        pCurrentController =  this;
        glutKeyboardFunc(keyCallback_);
        glutSpecialFunc(specialCallback_);
        glutKeyboardUpFunc(upKeyCallback_);
        glutSpecialUpFunc(upSpecialCallback_);
        glutMouseFunc(mouseCallback_);
    }


    //The function that should be called every frame.
    //possibly keys should get called for new Events or something
    void control() {
        for( std::map<unsigned char, bool>::iterator itr = keyRegistry.begin(); 
            itr != keyRegistry.end(); ++itr) {
                if (itr->second)
                    handleKey(itr->first);
        }


        for(std::map<int, bool>::iterator itr = specialRegistry.begin(); 
            itr != specialRegistry.end(); ++itr) {
            if (itr->second)
                handleSpecialKey(itr->first);
        }
    }

    void controlMouse() {
        for (std::map<int, mouseData>::iterator itr = mouseRegistry.begin(); 
            itr != mouseRegistry.end(); ++itr) {
                handleMouse(itr->first, itr->second.pushed, itr->second.newEvent, 
                    itr->second.x, itr->second.y);
                itr->second.newEvent = false;
        }
    }


    //Should handle any normal key
    virtual void handleKey(unsigned char key) {};

    //should handle GLUT_UP, GLUT_DOWN, GLUT_LEFT, GLUT_RIGHT, some others
    virtual void handleSpecialKey(int key) {};


    //should handle GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON
    virtual void handleMouse(int key, int state, bool newState, int x, int y) {};

};


