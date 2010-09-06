#ifndef VERTEX_DATA_
#define VERTEX_DATA_

#include "Vec.h"

/*
  Stores the vertex information
*/
class VertexData {
 public:
  //the position
  Vec p;

  Vec n;  //the normal

  //the colour
  bool hasColour; //whether or not the vertex has a colour specified
  Vec c;

 VertexData() {};
 VertexData(float xx, float yy, float zz) : p(xx,yy,zz), hasColour(false) {};

  void colour(float rr, float gg, float bb) {
    c = Vec(rr, gg, bb);
    hasColour = true;
  }

  bool operator==(const VertexData& other) {
    return (this->p == other.p);
  }

  VertexData operator+(const VertexData& v) const {
    VertexData sum(p.x + v.p.x, p.y + v.p.y, p.z + v.p.z);
    if (hasColour || v.hasColour)
      sum.colour(c.r + v.c.r, c.g + v.c.g, c.b + v.c.b);
    return sum;
  }

  VertexData operator*(float f) const {
    VertexData div(p.x*f, p.y*f, p.z*f);
    if (hasColour)
      div.colour(c.r*f, c.g*f, c.b*f);
    return div;
  }

  VertexData operator/(float f) const {
    return operator*(1.0f/f);
  }
    
};

/*
For the use of the algorithm, we want to maintain two different VertexDatas for each vertex at once.  The easiest way to do this is to abstract the actual vertexData away by one level with a container class.
 */

class VertexDataHolder {

  VertexData even;
  VertexData odd;
  bool hasOdd;
 public:
 VertexDataHolder(const VertexData& v) : even(v), hasOdd(false) {};
 VertexDataHolder(float x, float y, float z) : 
  even(x,y,z), hasOdd(false) {}; 
    
  
  void swap() {
    even = odd;
    hasOdd = false;
  }
  
  bool hasOddData() const {
    return hasOdd;
  }

  void addOddData(const VertexData toAdd) {
    odd = toAdd;
    hasOdd = true;
  }
  
  const VertexData& get() const { 
    return even;
  }

  VertexData& get() {
    return even;
  }
  
};

#endif
