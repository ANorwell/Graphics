#ifndef HALFEDGE_
#define HALFEDGE_

#include "VertexData.h"
#include <vector>
#include <list>
#include <stdio.h>
#include <cstdarg>

class Vertex;
class Face;
class Edge;

typedef std::vector<Face*>::iterator FaceIterator;
typedef std::vector<Edge*>::iterator EdgeIterator;
typedef std::vector<Vertex*>::iterator VertexIterator;



//////////////////////////////////////////////////////////////////////////////////////////
//                            Edge
//////////////////////////////////////////////////////////////////////////////////////////

//An edge of a polygon
class Edge {
  friend class HalfEdge;
 private:
 Edge() : vertex(NULL), pair(NULL), next(NULL), previous(NULL), face(NULL){};
 Edge(Vertex& v) : vertex(&v), 
    pair(NULL), next(NULL), previous(NULL), face(NULL) {};
 public:
  Edge* pair;
  Edge* next;
  Edge* previous;
  Vertex* vertex;  //the origin of the edge
  Face* face;
};


//////////////////////////////////////////////////////////////////////////////////////////
//                            Vertex
//////////////////////////////////////////////////////////////////////////////////////////

//A vertex of a polygon
class Vertex {
  friend class HalfEdge;
  friend class TriangleMesh;
 private:
   Edge* edge; //an edge leaving the vertex
   VertexDataHolder vdata; //The main data of the vertex

   //We give mutable access to the VertexDataHolder only to the conainer class
   //This encapsulates the holder from the user.
   VertexDataHolder& getHolder() {return vdata;};

   //Constructors are private.  The container class acts as a factory. 
 Vertex(const VertexData& v) : vdata(v), edge(NULL) {};
 Vertex(float x, float y, float z) : vdata(x,y,z), edge(NULL) {};
   
   // swaps the vertex data, as part of the sqrt(3) algorithm
   void swapData() { vdata.swap(); }
   /*
     Adds data to the Odd slot in the vertex , as part of the sqrt(3) algorithm
   */
   void addOddData(VertexData v) { vdata.addOddData(v); };

 public:

   //retrieves the vertex data
   const VertexData& getData() const { return vdata.get(); }
   VertexData& getData() { return vdata.get(); }


   //prints basic debug info about the vertex
   void print(int vertexNumber = -1) const;

 
   //Looks for an edge from the vertex to other.  Returns it, or NULL if none.
   Edge* findEdge(Vertex& other);

   //iterates over the outwards edges of the vertex
  class iterator {
    Edge* currentEdge;
    Edge* startEdge;
    bool done;
  public:
  iterator(Edge* edge) : startEdge(edge), currentEdge(edge), done(false) {};

    bool isDone() {
      return done;
    }

    iterator& operator++() {
      if (done) {
	      printf("ERR: incrementing a finished iterator\n");
	      return *this;
      }

      if (currentEdge->pair && currentEdge->pair->next)
	      currentEdge = currentEdge->pair->next;
      else
	      done = true;

      if (currentEdge == startEdge)
	      done = true;

      return *this;
    }
    
    iterator& operator--() {
      currentEdge = currentEdge->previous->pair;
      return *this;
    }

    Edge& operator*() {
      return *currentEdge;
    }
    Edge* operator->() {
      return currentEdge;
    }
    bool operator==(const iterator& other) {
      return (this->currentEdge == other.currentEdge);
    }
    bool operator!=(const iterator& other) {
      return !operator==(other);
    }
  };

  iterator current() { return iterator(edge); };
  iterator begin();
  bool isBoundary(Vertex::iterator * itr = NULL);

};



//////////////////////////////////////////////////////////////////////////////////////////
//                            Face
//////////////////////////////////////////////////////////////////////////////////////////
//a face of a polygon
class Face {
  friend class HalfEdge;
  friend class Vertex;
  friend class Edge;

 protected:
 Face() : edge(NULL) {};
 Face(Edge& e) : edge(&e) {}; 



 public:
  Edge* edge;
  virtual void incrementGeneration() {}; //not implemented in base class
  virtual int getGeneration() {return 0;}; //not implemented in base class

  //select the vertex at that position, 
  //using the order given by Face->edge->vertex .  is mod number of vertices
   Vertex* operator[](unsigned int i);

   //A list of all the positions of all vertices
   std::list<Vec> getPositionList();

  //iterate over the edges around the face
  class iterator {
    Edge * currentEdge;
    Edge * beginEdge;
    bool done;
    
  public:
    iterator(Edge* edge) : done(false) {
      currentEdge = beginEdge = edge;

    }

    bool isDone() {
      return done;
    }
    iterator& operator++() {
      currentEdge = currentEdge->next;

      if (currentEdge == beginEdge)
        done = true;

      return *this;
    }
    Vertex& operator*() {
      return *currentEdge->vertex;
    }
    Vertex* operator->() {
      return currentEdge->vertex;
    }
    bool operator==(const iterator& other) {
      return (this->currentEdge == other.currentEdge);
    }
    bool operator!=(const iterator& other) {
      return !operator==(other);
    }
  };

  iterator begin() {
    return iterator(edge);
  }
}; 


//////////////////////////////////////////////////////////////////////////////////////////
//                            HalfEdge
//////////////////////////////////////////////////////////////////////////////////////////
//The container class
class HalfEdge {

 protected:
  std::vector<Edge*> edgeList; //The list of edges
  std::vector<Face*> faceList; //The list of faces
  std::vector<Vertex*> vertexList; //The list of vertices
  
  //allow for creation of polygons via a list of Vertexes passed to HDS.
  Edge* firstEdge;
  Edge* previousEdge;
  Face* currentFace;

  void setPointersOnEdgePair(Vertex& a, Edge& b, Vertex& c, Edge& d);

 public:

 HalfEdge() : firstEdge(NULL), previousEdge(NULL), currentFace(NULL) {};

  //access the elements
  const std::vector<Vertex*>& getVertexList() const;
  const std::vector<Face*>& getFaceList() const;
  
  //methods for adding members to the HDS
  Edge& addEdge(Vertex& v);
  Edge& addEdge();
  Vertex& addVertex(const VertexData& vertex);
  virtual Face& addFace();
  virtual Face& addFace(Edge& e);

  //Ease of use methonds: draw a polygon as part of the HDS
  virtual bool beginPolygon(VertexData& v);
  virtual bool beginPolygon(Edge& e);
  virtual Edge& nextVertex(VertexData& next);
  virtual Face& endPolygon();
  virtual Face& makePolygon(int numberOfVertices, Vertex** vertexList);

  //allow iteration over the faces of the HDS
  FaceIterator faceBegin() { return faceList.begin(); }
  FaceIterator faceEnd() { return faceList.end();}
  EdgeIterator edgeBegin() { return edgeList.begin(); }
  EdgeIterator edgeEnd() { return edgeList.end(); }
  VertexIterator vertexBegin() { return vertexList.begin(); }
  VertexIterator vertexEnd() { return vertexList.end(); }

  //empty the container
  void clear();
  
  void print();

  ~HalfEdge();
};

#endif
