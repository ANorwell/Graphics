#include "Texture.h"
#include <iostream>

//////////////////////////////
//  BMPData
//////////////////////////////

unsigned char* BMPData::getData() {
    return &pData[BMP_DATA_OFFSET];
}

//reads a uint little endian from the pointer pos
inline unsigned int readInt(const unsigned char* aPointer){
    LOG(Log::DEBUG) << "aPointer[0] is" << (unsigned int) aPointer[0];
    LOG(Log::DEBUG) << "aPointer[1] is" << (unsigned int) aPointer[1];
    LOG(Log::DEBUG) << "aPointer[2] is" << (unsigned int) aPointer[2];
    LOG(Log::DEBUG) << "aPointer[3] is" << (unsigned int) aPointer[3];
    return ((unsigned int) aPointer[0]) +
        ((unsigned int) aPointer[1] << 8) +
        ((unsigned int) aPointer[2] << 16) +
        ((unsigned int) aPointer[3] << 24);
}

BMPData::BMPData(std::string aFilename) : TextureData(aFilename) {
    pData = (unsigned char *) Util::slurp(aFilename.c_str(), "rb");

    pWidth = readInt(pData + 0x12);
    pHeight = readInt(pData + 0x16);
    
    unsigned int filesize = readInt(pData + 0x2);
    LOG(Log::INFO) << "Loading bmp file:" << aFilename;
    LOG(Log::DEBUG) << "width is " << pWidth
                    << ", height is " << pHeight
                    << ", filesize is " << filesize;

    // 0x1E contains file type.
    // 0 - RGB
    // TODO: other formats
    if (readInt(pData + 0x1E) != 0x0)
        LOG(Log::WARNING) << "BMP file format is not RGB.  Only RGB BMPs are supported.";

    //file is bgr.  convert to rgb
    unsigned char temp;
    unsigned char* data = pData + BMP_DATA_OFFSET;
    for(unsigned int i=0; i<pWidth*pHeight; i++) {
        //each pixel takes 3 bytes
        //swap B (0th byte) with R (2nd)
        //TODO: Some files might take 4 bytes?
        temp = data[3*i];
        data[3*i] = data[3*i + 2];
        data[3*i + 2] = temp;
    }

}

void BMPData::setGLParams(GLuint aId) {
    glBindTexture(GL_TEXTURE_2D, aId);

    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
              GL_MODULATE);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the first MIP map
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  //replace with GL_CLAMP/GL_REPEAT for clamp/repeat
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                   GL_CLAMP);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                   GL_CLAMP);

  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, pWidth,
                     pHeight, GL_RGB, GL_UNSIGNED_BYTE, getData() );

}

BMPData::~BMPData() {
    delete[] pData;
}


////////////////////////////////////////
// RAWData
////////////////////////////////////////

RAWData::RAWData( std::string aFileName,
                  unsigned int aHeight,
                  unsigned int aWidth) :
    TextureData(aFileName) {
    pData = (unsigned char *) Util::slurp(aFileName.c_str(), "rb");
    LOG(Log::INFO) << "Making RAW texture with filename: " << aFileName;
    pWidth = aWidth;
    pHeight = aHeight;
    
}

void RAWData::setGLParams(GLuint aId) {
    glBindTexture(GL_TEXTURE_2D, aId);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
              GL_MODULATE);
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the first MIP map
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  //replace with GL_CLAMP to clamp
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                   GL_CLAMP);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                   GL_CLAMP);

  gluBuild2DMipmaps( GL_TEXTURE_2D, 3, pWidth,
    pHeight, GL_RGB, GL_UNSIGNED_BYTE, pData );

}

unsigned char* RAWData::getData() { return pData; }

RAWData::~RAWData() {
    delete[] pData;
}
