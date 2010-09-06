#include "Vec.h"
#include "TriangleMesh.h"

class Ray {
  Vec start;
  Vec end;

  Ray(Vec s, Vec e) start(s), end(e) {};

  intersect( TriangleFace face);
}