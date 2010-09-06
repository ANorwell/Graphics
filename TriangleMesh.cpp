#include "TriangleMesh.h"

Face& TriangleMesh::addFace() {
  TriangleFace* face = new TriangleFace();
  faceList.push_back(face);
  return *face;
};

Face& TriangleMesh::addFace(Edge& e) {
  TriangleFace* face = new TriangleFace(e);
  faceList.push_back(face);
  return *face;
}

Face& TriangleMesh::addFace(Edge& e, int gen) {
  TriangleFace* face = new TriangleFace(e, gen);
  faceList.push_back(face);
  return *face;
}  

bool TriangleMesh::beginPolygon(VertexData& vdata) {
  faceValence = 1;
  return HalfEdge::beginPolygon(vdata);
}

bool TriangleMesh::beginPolygon(Edge& e) {
  faceValence = 2;
  return HalfEdge::beginPolygon(e);
}

Edge& TriangleMesh::nextVertex(VertexData& vdata) {
  faceValence++;
  if (faceValence>3)
    printf("ERROR in TriangleMesh: face valence above 3\n");
  return HalfEdge::nextVertex(vdata);
}

inline VertexData VDataAverage(int length, VertexData* vDataList) {
  VertexData average;
  for (int i=0; i<length; i++)
    average = average + vDataList[i];

  return average/length;
}

/*
  Check several properties of the face, and print diagnostic information.
*/
bool TriangleMesh::checkConsistency(Face& f) {
  bool ret = true;
  std::cout << "Face " << &f << std::endl;
  if ((f.edge->next->next->next == f.edge) && 
      (f.edge->previous->previous->previous == f.edge))
    std::cout << " is a triangle" << std::endl;
  else 
    ret = false;

  if ( (f.edge->face == &f) && (f.edge->next->face == &f) &&
       (f.edge->next->next->face == &f) )
       std::cout << " the faces of its edges are correct\n";
  else
    ret = false;

  if (f.edge->pair) {
    std::cout << " is not a boundary on main edge\n";
    if (f.edge->next->vertex == f.edge->pair->vertex)
      std::cout << " its edge and its pair are opposites\n";
    else
      ret = false;
  }

  if (f.edge->pair && f.edge->next->pair && f.edge->next->next->pair)
    std::cout << "f is an internal face\n";
  else
    std::cout << "f is NOT an internal face\n";
      
  return ret;
}

/*
  Relaxes the vertex v to a new position
  We will store v's new relaxed position as the oddVertexData
  We only relax non-boundary vertices
  PRE: v has an even generation
       v is not a boundary edge
  
*/
void TriangleMesh::relaxVertex(Vertex& v) {
  
  if (v.isBoundary())
    return;

  Vertex::iterator vitr = v.current();

  //The (elementwise) sum of the vertices around v.
  VertexData vertexSum;

  int vertexCounter = 0;

  for(; !vitr.isDone(); ++vitr) {  
    vertexSum = vertexSum + vitr->next->vertex->getData();
    ++vertexCounter;
  }

  //the combination value alpha, as determined by the sqrt(3) algorithm.
  float alpha = (4 - 2*cos(2*Globals::PI/vertexCounter) )/9.0f;

  //Save the new, relaxed value in the "odd" Vertex Data 
  v.addOddData(v.getData()*(1-alpha) + vertexSum*(alpha/vertexCounter));

}

/*
  Swaps the newly computed ("odd") data of the vertex to be the primary ("even")
  data.
*/
void TriangleMesh::updateVertex(Vertex& v) {
  if (v.getHolder().hasOddData()) {
    v.swapData();
  }
}

/*
  Splits each face into three faces around a centre vertex.  
  PRE: The face has an even generation number.
  POST: Each new face has an odd generation number, and the edge pointers of the
  new faces point to the edges that will need to be swapped in the edge swap 
  step.
*/
Face** TriangleMesh::splitFace(Face& f) {

  VertexData vDataList[3];
  
  TriangleFace::iterator fitr = f.begin();
  TriangleFace::iterator end = f.begin();

  //the three faces we will create
  Face** returnFaces = new Face*[3];

  int i = 0;
  do {
    vDataList[i] = fitr->getData();
    ++i;
    ++fitr;
  } while (fitr != end);

  //construct the new vertex
  VertexData newVertexData = VDataAverage(3, vDataList);
  Vertex& newVertex = addVertex(newVertexData);


  //build the 3 new faces
  Edge* startEdge = f.edge;
  Edge* currentEdge = f.edge;
  Edge* toNew[3];   //the three half edges on the three new faces to newVertex
  Edge* fromNew[3]; //from newVertex
  int newGeneration = f.getGeneration() + 1;

  for (int i=0; i<3; i++) {
    
    Edge* nextEdge = currentEdge->next;
    
    //add the face.  We will reuse f since we don't want to have to remove it.
    Face* newFace;
    if (i>0) {
      newFace = &addFace(*currentEdge, newGeneration);
    } else {
      newFace = &f;
      newFace->incrementGeneration();
    }

    returnFaces[i] = newFace;

    //add the new edges
    toNew[i] = &addEdge(*(nextEdge->vertex) );
    fromNew[i] = &addEdge(newVertex);

    //set up the next/prev pointers
    toNew[i]->next = fromNew[i];
    fromNew[i]->previous = toNew[i];
    currentEdge->next = toNew[i];
    toNew[i]->previous = currentEdge;
    fromNew[i]->next = currentEdge;
    currentEdge->previous = fromNew[i];

    //set the face pointers
    currentEdge->face = newFace;
    toNew[i]->face = newFace;
    fromNew[i]->face = newFace;

    currentEdge = nextEdge;
  }

  newVertex.edge = fromNew[0];

  //set up all the pairs on the newly added edges
  for (int i=0; i<3; i++) {
    toNew[i]->pair = fromNew[(i+1)%3];
    fromNew[(i+1)%3]->pair = toNew[i];
  }

  return returnFaces;
}

/*
  Swaps the edge given by the edgepointer of the provided face.
*/

void TriangleMesh::swapEdge(Face& f) {
  int generation = f.getGeneration();

  if ((generation%2)==0)
    return;
  
  Edge* toSwap = f.edge;
  Edge* toSwapPair = f.edge->pair;

  //increment generations
  f.incrementGeneration();

  if (toSwapPair == NULL) {
    //we are on a boundary, nothing to do
    printf("Face to split is on a boundary\n");
    return;
  }

    

  toSwapPair->face->incrementGeneration();

  //update the edge pointers of the vertices on the swapped edge
  toSwap->vertex->edge = toSwapPair->next;
  toSwapPair->vertex->edge = toSwap->next;

  //update next/previous of the non-swapped edges
  toSwap->previous->next = toSwapPair->next;
  toSwapPair->next->previous = toSwap->previous;
  toSwap->next->previous = toSwapPair->previous;
  toSwapPair->previous->next = toSwap->next;
  
  //update face-pointers
  toSwap->previous->face = toSwapPair->face;
  toSwapPair->previous->face = toSwap->face;

  //update the swapped edges' vertices
  toSwap->vertex = toSwap->previous->vertex;
  toSwapPair->vertex = toSwapPair->previous->vertex;
  
  //update the swapped edges next/prev
  Edge* tempSwapPairNext = toSwap->previous;
  toSwap->previous = toSwap->next;
  toSwap->next = toSwapPair->previous;
  toSwapPair->previous = toSwapPair->next;
  toSwapPair->next = tempSwapPairNext;

  //change the "outside loop" back to two triangles
  toSwap->previous->next = toSwap;
  toSwap->next->previous = toSwap;
  toSwapPair->previous->next = toSwapPair;
  toSwapPair->next->previous = toSwapPair;
}

void TriangleMesh::relaxAllVertices() {
  VertexIterator vitr = vertexBegin();
  VertexIterator end = vertexEnd();
  for(; vitr != end; ++vitr)
    relaxVertex(**vitr);
}

void TriangleMesh::updateAllVertices() {
  VertexIterator vitr = vertexBegin();
  VertexIterator end = vertexEnd();
  for(; vitr != end; ++vitr)
    updateVertex(**vitr);
}

void TriangleMesh::splitAllFaces() {
  const std::vector<Face*> faceListCopy = getFaceList();
  
  for(unsigned int i=0; i<faceListCopy.size(); ++i)
    splitFace(*faceList[i]);

}

void TriangleMesh::swapAllEdges() {
  const std::vector<Face*> faceListCopy = getFaceList();

  for(unsigned int i=0; i<faceListCopy.size(); ++i)
    swapEdge(*faceList[i]);
}

/*
  Subdivides the mesh as per the sqrt(3) algorithm.
*/
void TriangleMesh::split() {
  relaxAllVertices();
  splitAllFaces();
  updateAllVertices();
  swapAllEdges();
}

