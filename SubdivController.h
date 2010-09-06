#pragma once
#include "Controller.h"
#include "SubdivScene.h"


class SubdivController : public Controller {
public:
    SubdivController(SubdivScene* aScene, ShaderLoader* aShader) : 
          pScene(aScene),
          pShader(aShader) {};
    void handleKey(unsigned char key);
    void handleSpecialKey(int key);
    void handleMouse(int key, int state, bool newState, int x, int y);
private:
    SubdivScene* pScene;
    ShaderLoader* pShader;
};