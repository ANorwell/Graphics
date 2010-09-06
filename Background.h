#pragma once

#include <string>
#include <vector>
#include <GL/glut.h>
#include "Texture.h"

class Background {

    //TODO this class sucks
private:
  std::vector<TextureData*> pTextures;

public:

    Background() {
        glEnable(GL_TEXTURE_2D);
    }

    void addBMPTexture(std::string aFileName) {
        GLuint id = (GLuint) pTextures.size();
        BMPData* tex = new BMPData(aFileName);
        tex->setGLParams(id);
        pTextures.push_back(tex);

    }

    ~Background() {
        for (std::vector<TextureData*>::iterator itr = pTextures.begin();
             itr != pTextures.end(); ++itr)
            delete *itr;
    }
};
