#pragma once

#include <cmath>
#include <cstdlib>
#include <cstdio>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <GL/glut.h>
#include <ctime>
#include "Vec.h"



/*
  A camera class that provides relational movement functions, allowing for a
  camera that changes position easily.  
*/
class Camera {
  Vec position; //the position of the camera
  Vec up;       //the up vector of the camera
  Vec look; //the look direction, as a quaternion for easy rotation

  float dt;  //length of time since last update

  //The timing-specific stuff, arch-dependent
#ifdef WIN32
  __int64 pFreq;   //the frequency of the timer
  __int64 pLastUpdate; //last time of a time update (QueryPerformanceCounter)
#else
  struct timeval pLastUpdate;
#endif

   bool pMoved; //Whether the camera has moved 
  float pSpeed; //How fast the camera moves per move step
  float pAngleSpeed; //How fast the camera turns per move step

  inline void firstUpdate();
  void placeCamera();

 public:
  Camera();
 
  //Update the camera location (call every frame)
 
  //Modify the current location of the camera
  void rotate(float x, float y, float z, float angle);
  void translate(float dx, float dy, float dz);  
  void moveForward(float speed);
  void moveLeft(float speed);
  void rotateLeft(float aSpeed);
  void rotateUp(float aSpeed);
  void move(float angle, float speed);

  void update();

  //set the parameters of the camera
  void setPosition(float x, float y, float z);
  void setLook(float x, float y, float z);
  void setSpeed( float aSpeed) { pSpeed = aSpeed; }
  void setAngleSpeed( float aSpeed) { pAngleSpeed = aSpeed; }

   //getters
  Vec getPos() { return position; }
  Vec getLook() { return look; }
  Vec getUp() { return up; }
  float getSpeed() { return pSpeed;}
  float getAngleSpeed() { return pAngleSpeed;}

};


  
