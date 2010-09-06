#pragma once

#include <string>
#include <vector>
#include <GL/glut.h>
#include "Util.h"

class TextureData {

private:
    std::string pFilename;
protected:
    unsigned int pHeight;
    unsigned int pWidth;

public:
    TextureData(std::string aFile) : pFilename(aFile),
        pHeight(0),
        pWidth(0) {};

    //pure virtual functions to be subclassed
    virtual unsigned char* getData() = 0;
    virtual void setGLParams(GLuint aId) = 0;

    virtual unsigned int getHeight() { return pHeight; }
    virtual unsigned int getWidth() { return pWidth; }

    const std::string& getFilename() { return pFilename; };

    virtual ~TextureData() {};

};

class BMPData : public TextureData {
private:
    unsigned char * pData;
    enum { BMP_DATA_OFFSET = 0x36 };
public:

    BMPData(std::string aFilename);

    unsigned char* getData();

    void setGLParams(GLuint aId);

    ~BMPData();
};

class RAWData : public TextureData {
private:
    unsigned char * pData;
public:
    
    RAWData(std::string aFilename,
            unsigned int aHeight,
            unsigned int aWidth);
    
    unsigned char* getData();

    void setGLParams(GLuint aId);

    ~RAWData();
};
