#pragma once

/*
A thin wrapper around GL shader functions to allow for easy use of shaders.
*/

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glut.h>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <iostream>
#include "Util.h"

class ShaderLoader
{

public:
  ShaderLoader(void);
	~ShaderLoader(void);
	GLuint addShader(GLuint shaderType, const char* filename);
	GLuint addShader(GLuint shaderType, std::string filename);
    void attachShader(GLuint shaderType, int shaderIndex);
    void detachShader(GLuint shaderType);
    void setShader(GLuint shaderType, int shaderIndex);
    void nextShader(GLuint shaderType);
    int getShaderPos(GLuint shaderType);
    void useProgram();
    void checkErrors();
    void checkInfoLogs();
    //const char * slurp(const char* filename);

private:
	GLuint program_;
  int vShaderPos_;  //index into vShaders_
  int fShaderPos_;  //index into fShaders_
  std::vector<GLuint> vShaders_;
  std::vector<GLuint> fShaders_;

};
