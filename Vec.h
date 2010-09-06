#pragma once

#include <cmath>
#include <cstdio>
#include "Globals.h"

class Vec {

public:

  union {
    struct {
      float x,y,z;
    };
    struct {
      float r,g,b;
    };
  };


  Vec() : x(0.0), y(0.0), z(0.0) {};
  Vec(float xx, float yy, float zz)  : x(xx), y(yy), z(zz) {};

  Vec operator*(Vec other);
  Vec operator*(float c);
  Vec operator/(float c);
  Vec operator+(Vec other);
  Vec operator-(Vec other);
  bool operator==(Vec other);

  Vec cross(Vec other);
  float dot(Vec other);
  Vec normalize();
  float length();

  friend Vec operator*(float c, Vec v);

  Vec rotate(Vec v, float angle);

  virtual void print();
};

  /*
  A quaternion class for use by the Camera.  Allows for easy rotation of the 
  Quaternion around another Quaternion.
*/
  class Quaternion : public Vec {

 public:
  float w;
  
  Quaternion() {};
  Quaternion(float xx, float yy, float zz);
  Quaternion(Vec v);

  Quaternion rotate(Vec v, float angle);

  Quaternion qprod(Quaternion other);
  Quaternion conjugate();
  Quaternion qnorm();

  void print();
};
