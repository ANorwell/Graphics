#pragma once
#include "Controller.h"

class Scene {

public:
    virtual void init();
    virtual void draw();
    //void setController(Controller* aController) { pController = aController; };
    //Controller* getController() {return pController;}
private:
    //Controller* pController;
};

