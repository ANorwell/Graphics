#include "Vec.h"

Vec Vec::cross(Vec o) {
  Vec r;
  r.x = y*o.z - z*o.y;
  r.y = z*o.x - x*o.z;
  r.z = x*o.y - y*o.x;
  return r;
}

Vec Vec::operator*(Vec o) {
  Vec r;
  r.x = x*o.x;
  r.y = y*o.y;
  r.z = z*o.z;
  return r;
}

float Vec::dot(Vec o) {
  return x*o.x + y*o.y + z*o.z;
}

Vec Vec::operator*(float c) {
  Vec r;
  r.x = x*c;
  r.y = y*c;
  r.z = z*c;
  return r;
}

Vec Vec::operator/(float c) {
  return (*this)*(1/c);
}

Vec Vec::operator+(Vec o) {
  Vec r;
  r.x = x + o.x;
  r.y = y + o.y;
  r.z = z + o.z;
  return r;
}

Vec Vec::operator-(Vec o) {
  return (*this) + ( (-1.0f) * o);
}


Vec Vec::normalize() {
  float len = sqrt( dot(*this));
  
  Vec ret;
  ret.x = x/len;
  ret.y = y/len;
  ret.z = z/len;

  return ret;
}

bool Vec::operator==(Vec o) {
  return ( (x==o.x) && (y==o.y) && (z==o.z));
}

Vec operator*(float c, Vec v) {
  return v*c;
}

void Vec::print() {
  printf(" (%f, %f, %f)\n", x, y, z);
}

float Vec::length() {
    return sqrt(x*x + y*y + z*z);
}


Quaternion::Quaternion(float xx, float yy, float zz) {
  float len = sqrt(xx*xx + yy*yy + zz*zz);
  x = xx/len;
  y = yy/len;
  z = zz/len;
  w = 0.0f;
}

Quaternion::Quaternion(Vec v) {
  Vec vn = v.normalize();
  x = vn.x;
  y = vn.y;
  z = vn.z;
  w = 0.0f;
  qnorm();
}


Quaternion Quaternion::qprod(Quaternion a) {
  Quaternion ret;
  ret.x = w*a.x + x*a.w + y*a.z - z*a.y;
  ret.y = w*a.y - x*a.z + y*a.w + z*a.x;
  ret.z = w*a.z + x*a.y - y*a.x + z*a.w;
  ret.w = w*a.w - x*a.x - y*a.y - z*a.z;

  return ret.qnorm();
}

Quaternion Quaternion::qnorm() {
  float len = sqrt(x*x + y*y + z*z + w*w);

  Quaternion ret;
  ret.x = x/len;
  ret.y = y/len;
  ret.z = z/len;
  ret.w = w/len;
  return ret;
}


    
Quaternion Quaternion::conjugate() {
  Quaternion ret;
  ret.x = -x;
  ret.y = -y;
  ret.z = -z;
  ret.w = w;
  return ret;
}

Vec Vec::rotate(Vec v, float angle) {
  float angleRad = angle/180*Globals::PI;
  Quaternion R;
  R.x = v.x*sin(angleRad/2);
  R.y = v.y*sin(angleRad/2);
  R.z = v.z*sin(angleRad/2);
  R.w = cos(angleRad/2);

  Quaternion asQ(*this);

  return R.qprod(asQ.qprod(R.conjugate()) );
}



void Quaternion::print() {
  printf(" (%f, %f, %f, %f)\n", x, y, z, w);
}