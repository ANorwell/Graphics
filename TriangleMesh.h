#ifndef TRIANGLEMESH_
#define TRIANGLEMESH_

#include "HalfEdge.h"
#include "Globals.h"
#include <iostream>
#include <cmath>

/*
  A triangle Face is a face implementation that stores its generation.
  Parts of the sqrt(3) implementation depend on the generation of the face.
*/
class TriangleFace : public Face {
  int generation;

 public:
 TriangleFace() : generation(0), Face() {};
 TriangleFace(Edge& e) : generation(0), Face(e) {};
 TriangleFace(Edge& e, int gen) : generation(gen), Face(e) {};
  void incrementGeneration() { ++generation; }
  int getGeneration() { return generation; }
};


class TriangleMesh : public HalfEdge {
  int faceValence; //Number of edges on a face to be added, must be 3

 public:

 TriangleMesh() : faceValence(0), HalfEdge() {};

  //methods for adding triangles
  bool beginPolygon(VertexData& vdata);
  bool beginPolygon(Edge& e);
  Edge& nextVertex(VertexData& vdata);

  //assert the given face is consistent
  bool checkConsistency(Face& f);

  //add a face
  Face& addFace();
  Face& addFace(Edge& e);
  Face& addFace(Edge& e, int gen);

 private:
  //The implementation of the sqrt(3) algorithm
  void relaxVertex(Vertex& v);
  void updateVertex(Vertex& v);
  Face** splitFace(Face& f);
  void swapEdge(Face& f);

  //apply sqrt(3) to the entire mesh.
 public:
  void relaxAllVertices();
  void updateAllVertices();
  void splitAllFaces();
  void swapAllEdges();


  //The main sqrt(3) step
  void split();
};

#endif
