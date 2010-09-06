#include "ShaderLoader.h"

ShaderLoader::ShaderLoader(void)
{
    program_ = glCreateProgram();
    if (!program_) {
        LOG(Log::ERR) << "Unable to create shader program";
    } 
    checkErrors();
    vShaderPos_ = -1;
    fShaderPos_ = -1;
}

ShaderLoader::~ShaderLoader(void)
{
}

/*
const char* ShaderLoader::slurp(const char* fn) {

	FILE *fp;
	fp = fopen(fn,"rt");

	if (fp == NULL)
		throw NoFileException();

    fseek(fp, 0, SEEK_END);
    int count = ftell(fp);
	rewind(fp);
	char* content = new char[count+1];
	count = fread(content,sizeof(char),count,fp);
	content[count] = '\0';

	std::cout << count << std::endl;

	std::cout << content << std::endl;
	
	fclose(fp);
	return content;
}

*/
GLuint ShaderLoader::addShader(GLuint shaderType, std::string filename) {


	return addShader(shaderType, filename.c_str() );
}

GLuint ShaderLoader::addShader(GLuint shaderType, const char *filename) {
    LOG(Log::INFO) << "Adding shader type " << shaderType
                << " filename " << filename;
    const char* shaderCode;
	
	try {
        shaderCode = Util::slurp(filename);
	} catch (std::exception e) {
		std::cerr << "Err: shader File not found: " << filename << std::endl;
        LOG(Log::ERR) << "Filename " << filename <<
            "Not found";
	}

	
	std::cout << shaderCode << std::endl;

	GLuint shaderHandle = glCreateShader(shaderType);
    checkErrors();
	glShaderSource(shaderHandle, 1, &shaderCode, NULL);

  glCompileShader(shaderHandle);
  checkErrors();
  if (shaderType == GL_VERTEX_SHADER)
      vShaders_.push_back(shaderHandle);
  else if (shaderType == GL_FRAGMENT_SHADER)
      fShaders_.push_back(shaderHandle);
  else
      std::cerr << "Err: Unknown shader type:" << shaderType << std::endl;

  return shaderHandle;
}

void ShaderLoader::attachShader(GLuint shaderType, int shaderIndex) {
    LOG(Log::INFO) << "Attaching Shader " << shaderType
                   << "with index " << shaderIndex;
  if (shaderType == GL_VERTEX_SHADER) {
    glAttachShader(program_, vShaders_[shaderIndex] );
    vShaderPos_ = shaderIndex;
  }
  else if (shaderType == GL_FRAGMENT_SHADER) {
    glAttachShader(program_, fShaders_[shaderIndex] );
    fShaderPos_ = shaderIndex;
  } else {
    std::cerr << "Err: Unknown shader type in attach shader." << std::endl;
  }
  checkErrors();
  LOG(Log::INFO) << "shader attached";
}

void ShaderLoader::detachShader(GLuint shaderType) {
    LOG(Log::INFO) << "Detaching Shader " << shaderType;

  if (shaderType == GL_VERTEX_SHADER) {
    if (vShaderPos_ == -1) 
      return;
    glDetachShader(program_, vShaders_[vShaderPos_] );
    vShaderPos_ = -1;
  }
  else if (shaderType == GL_FRAGMENT_SHADER) {
    if (fShaderPos_ == -1) 
      return;
    glDetachShader(program_, fShaders_[fShaderPos_] );
    fShaderPos_ = -1;
  } else {
    std::cerr << "Err: Unknown shader type in detatch shader." << std::endl;
  }
  checkErrors();
}

//needs useprogram call
void ShaderLoader::setShader(GLuint shaderType, int shaderIndex) {
    std::cout << "Setting shader: " << shaderIndex
              << std::endl;
    LOG(Log::INFO) "Setting shader: " << shaderIndex;
    detachShader(shaderType);
    attachShader(shaderType, shaderIndex);
}

void ShaderLoader::nextShader(GLuint shaderType) {
  if (shaderType == GL_VERTEX_SHADER)
    setShader(shaderType, (getShaderPos(shaderType) + 1) % vShaders_.size() );
  else if (shaderType == GL_FRAGMENT_SHADER)
    setShader(shaderType, (getShaderPos(shaderType) + 1) % fShaders_.size() );
  else
    std::cerr << "Err: Unknown shader type in setshader." << std::endl;

  useProgram();
}

int ShaderLoader::getShaderPos(GLuint shaderType) {
  if (shaderType == GL_VERTEX_SHADER)
    return vShaderPos_;
  else if (shaderType == GL_FRAGMENT_SHADER)
    return fShaderPos_;
  else
    std::cerr << "Err: Unknown shader type in getshaderpos." << std::endl;
  return -1;
}


void ShaderLoader::useProgram() {

    if (glIsProgram(program_)) {
        glLinkProgram(program_);
        checkErrors();
    } else {
        LOG(Log::WARNING) << "Program not a program";
    }
	glUseProgram(program_);
    checkErrors();
}

void ShaderLoader::checkErrors() {
	GLuint err;
	std::cerr << "Checking errors:" << std::endl;
	while (err = glGetError()) {

		switch (err) {
			case GL_INVALID_OPERATION:
				std::cerr << "-Invalid operation:" << std::endl;
                LOG(Log::WARNING) << "Invalid operation error";
				break;
			case GL_INVALID_VALUE:
				std::cerr << 
					"-Invalid value (Probably referenced a missing shader/program"
					<< std::endl;
                LOG(Log::WARNING) << "Invalid value error";
				break;
			default:
				std::cerr << "-Some other error" <<std::endl;
                LOG(Log::WARNING) << "Some other shader error";
		}
	}
	std::cerr << "-No more errors" << std::endl;
    checkInfoLogs();
}

inline void checkLog(GLuint aId) {

    int maxlogSize;
    if (glIsShader(aId)) {
        glGetShaderiv(aId, GL_INFO_LOG_LENGTH, &maxlogSize);
    } else {
        glGetProgramiv(aId, GL_INFO_LOG_LENGTH, &maxlogSize);
    }

    char infoLog[maxlogSize];
    int logSize;
    if (glIsShader(aId)) {
        LOG(Log::INFO) << "Info Log for shader (id of " << aId
                       << ")";
        glGetShaderInfoLog(aId, maxlogSize, &logSize, infoLog);
    } else {
        LOG(Log::INFO) << "Info Log for program (id of " << aId
                       << ")";
        glGetProgramInfoLog(aId, maxlogSize, &logSize, infoLog);
    }

    LOG(Log::INFO) << infoLog;
}

void ShaderLoader::checkInfoLogs() {
    LOG(Log::INFO) << "Checking shader info logs";

    LOG(Log::INFO) << "Checking program:";
    checkLog(program_);

    LOG(Log::INFO) << "Checking vertex shaders:";
    for( std::vector<GLuint>::iterator itr = vShaders_.begin();
         itr != vShaders_.end(); ++itr)
        checkLog(*itr);
    
    LOG(Log::INFO) << "Checking fragment shaders:";
    for( std::vector<GLuint>::iterator itr = fShaders_.begin();
         itr != fShaders_.end(); ++itr)
        checkLog(*itr);
}
    
