#include "Camera.h"

//The arch-dependent stuff

inline void Camera::firstUpdate() {
#ifdef WIN32
  QueryPerformanceFrequency((LARGE_INTEGER*)&pFreq);
  QueryPerformanceCounter((LARGE_INTEGER*)&pLastUpdate);
#else
  gettimeofday(&pLastUpdate, NULL);
#endif
}

//should be called every frame.
void Camera::update() {
#ifdef WIN32
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  dt = (currTime - pLastUpdate) / (float)pFreq;
#else
  struct timeval currTime;
  gettimeofday(&currTime, NULL);
  time_t dsecs = currTime.tv_sec - pLastUpdate.tv_sec;
  suseconds_t dus = currTime.tv_usec - pLastUpdate.tv_usec;
  dt = dsecs + (float)dus * 0.000001;
#endif
  pLastUpdate = currTime;  
  placeCamera();
}

//end arch-dependent

Camera::Camera() {
  position.x = 0;
  position.y = 0;
  position.z = 1;
  
  up.x = 0.0f;
  up.y = 1.0f;
  up.z = 0.0f;

  look.x = 0;
  look.y = 0;
  look.z = -1;

  dt = 0.0f;

}

void Camera::moveForward(float speed) {
  //look has norm 1, so we do not need to normalize
  position = position + look*(dt*speed);
  pMoved = true;
}

void Camera::moveLeft(float speed) {
  
  Vec left  = up.cross(look);
  left.normalize();
  position = position + left*(dt*speed);
  pMoved = true;
}

//rotate to the left around up
void Camera::rotateLeft(float aSpeed) {
  look = look.rotate(up, dt*aSpeed);
  pMoved = true;
}

//rotate upwards, around up x look
void Camera::rotateUp(float aSpeed) {
  Vec left = look.cross(up);
  look = look.rotate(left, dt*aSpeed);
  up = up.rotate(left, dt*aSpeed);

  pMoved = true;
}

void Camera::translate(float dx, float dy, float dz) {
  position.x += dx;
  position.y += dy;
  position.z += dz;
  pMoved = true;
}


//move at an angle of angle in the {up x forward, forward} plane, 
//where angle=0 would be motion to the right, 
void Camera::move(float angle, float speed) {
  Vec right = look * up;
  Vec perp_up = right * look;

  Vec r = right.rotate(perp_up, angle);
  position = position + r*(dt*speed);
  pMoved = true;

}

void Camera::placeCamera() {
  gluLookAt(
	    //position
	    position.x, position.y, position.z,
	    
	    //lookat
	    position.x + look.x, 
	    position.y + look.y, 
	    position.z + look.z,

	    //up
	    up.x, up.y, up.z
	    );

}

void Camera::setPosition(float x, float y, float z) {
  position.x = x;
  position.y = y;
  position.z = z;
}

/**
 * setLook: sets the camera's look
 * @sdf
 */
void Camera::setLook(float x, float y, float z) {
  if (x==0 && y==0 && z==0)
    return;
  look.x = x;
  look.y = y;
  look.z = z;
  look.normalize();
}

