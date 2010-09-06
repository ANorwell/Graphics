#include "HalfEdge.h"


//////////////////////////////////////////////////////////////
//            Vertex Class
//////////////////////////////////////////////////////////////


//is the vertex on a boundary? sets provided iterator, if any, to the first edge
bool Vertex::isBoundary(Vertex::iterator* itr_p) {

  Vertex::iterator itr = this->current();
  if (itr_p != NULL)
    itr = *itr_p;
  Vertex::iterator end = this->current();

  //whether or not the faces around the vertex are incomplete
  bool boundary = false;

  // "rewind" backwards to the start of the possibly incomplete set of faces.
    do {
      if(itr->previous->pair){
 	--itr;
      } else {
	boundary = true;
	break;
      }
    } while ( itr != end );

    return boundary;
}

Vertex::iterator Vertex::begin() {
  Vertex::iterator itr = current();
  isBoundary(&itr);
  return itr;
}
  
/*
  Returns a pointer to the edge connecting this vertex to other, or null if 
  it does not exist
  Pre: Either the vertex is completely surrounded by faces, or the faces around
  the vertex are all adjacent.  
*/
Edge* Vertex::findEdge(Vertex& other) {
  Vertex::iterator itr = this->begin();

  for(; !itr.isDone(); ++itr) {
    if( itr->next) {
      Vertex * oppositeVertex = itr->next->vertex;
      if (oppositeVertex == &other) {
	return &(*itr);
      }
    }
  }
  return NULL;
}

void Vertex::print(int i) const {
  printf("Vertex %i is at (%f, %f, %f)\n", i, getData().p.x, 
	 getData().p.y, getData().p.z);
}


Vertex* Face::operator[](unsigned int i) {
  Edge* e = this->edge;
  while (i>0) {
    i--;
    e = e->next;
  }

  return e->vertex;
}

std::list<Vec> Face::getPositionList() {
  std::list<Vec> positions;

  for(Face::iterator itr = begin(); !itr.isDone(); ++itr)
    positions.push_back( itr->getData().p );

  return positions;
}



/////////////////////////////////////////////////////////////
//            HalfEdge Class
//////////////////////////////////////////////////////////////

Edge& HalfEdge::addEdge() {
  Edge* edge = new Edge();
  edgeList.push_back(edge);
  return *edge;
}

Edge& HalfEdge::addEdge(Vertex& v) {
  
  Edge *edge = new Edge(v);
  edgeList.push_back(edge);

  if (v.edge == NULL)
    v.edge = edge;

  return *edge;
}
  
Vertex& HalfEdge::addVertex(const VertexData& vertex) {
  Vertex* v = new Vertex(vertex);
  vertexList.push_back(v);
  return *v;
}

Face& HalfEdge::addFace() {
  Face* face = new Face();
  faceList.push_back(face);
  return *face;
};

Face& HalfEdge::addFace(Edge& e) {
  Face* face = new Face(e);
  faceList.push_back(face);
  return *face;
}

const std::vector<Vertex*>& HalfEdge::getVertexList() const {
  return vertexList;
}

const std::vector<Face*>& HalfEdge::getFaceList() const {
  return faceList;
}


/*
  Begins drawing a polygon (from scratch, starting at the vertex vdata
*/
bool HalfEdge::beginPolygon(VertexData& vdata) {
  if (currentFace)
    return false;


  Vertex& v = addVertex(vdata);
  Edge& e = addEdge(v);

  Face& f = addFace(e);
  currentFace = &f;
  e.face = currentFace;

  firstEdge = &e;
  previousEdge = &e;
  return true;
}

/*
  Begins drawing a polygon that will start from startEdge, so that 
  startEdge->pair->face is this polygon
*/

bool HalfEdge::beginPolygon(Edge& startEdge) {
  if (currentFace)
    return false;

  Edge& startPair = addEdge(*startEdge.next->vertex);
  startPair.pair = &startEdge;
  startEdge.pair = &startPair;
  Edge& e = addEdge(*startEdge.vertex);
  Face& f = addFace(e);
  startPair.next = &e;
  e.previous = &startPair;
  
  e.face = &f;
  startPair.face = &f;

  currentFace = &f;
  firstEdge = &startPair;
  previousEdge = &e;
  return true;
}


/*
  Add a vertex to the current polygon.  Returns the edge out of the vertex.
*/
Edge& HalfEdge::nextVertex(VertexData& vdata) {
  Vertex& next = addVertex(vdata);
  Edge& e = addEdge(next); //edge out of next
  previousEdge->next = &e;
  e.previous = previousEdge;
  e.face = currentFace;

  previousEdge = &e;
  return e;
}

/*
  Finish drawing the polygon. Connects the last point to the first point, and 
  returns the Face of the polygon.  The edge of the face is the first edge
  added to the polygon
*/
Face& HalfEdge::endPolygon() {
  previousEdge->next = firstEdge;
  firstEdge->previous = previousEdge;

  firstEdge = NULL;
  previousEdge = NULL;

  Face& toReturn = *currentFace;
  currentFace = NULL;

  return toReturn;
}


/*
  Given two vertices with edges out of them, where the first follows the second,
  sets up the pointers in this configuration.  Checks for an opposite edge from
  the first to the second (ie, opposite direction on the opposite face), and 
  makes that the pair of the first edge.
  Helper function.
 */
void HalfEdge::setPointersOnEdgePair(Vertex& currVertex, Edge& currEdge,
				     Vertex& prevVertex, Edge& prevEdge ) {
  currEdge.previous = &prevEdge;
  prevEdge.next = &currEdge;
  
  //check for adjacencies and set pointers
  if (currVertex.edge != NULL) {
    Edge* opposite = currVertex.findEdge(prevVertex);
    if (opposite) {
      printf("opposite found\n");
      prevEdge.pair = opposite;
      opposite->pair = &prevEdge;
    } else {
      printf("no opposite found\n");
    } 
  } else {
    printf("vertex has no edge pointer: ");
    currVertex.print();
    currVertex.edge = &currEdge;
  }
}

/*
  Given an ordered list of input pointers to vertices v1, v2, ... as parameters
  begins a polygon
  with edges from v1 to v2 to ....  For the HDS to be consistent, it is 
  necessary that edges from v2 to v1, v3 to v2, and so on, on the opposite
  faces, exist.
*/

Face& HalfEdge::makePolygon(int numberOfVertices, Vertex** vertexList) {

  
  printf("starting make polygon\n");

  Vertex& firstVertex = *vertexList[0];
  firstVertex.print();
  Edge& firstEdge = addEdge(firstVertex);
  Face& f = addFace(firstEdge);

  firstEdge.face = &f;
  Vertex* prevVertex = &firstVertex;
  Edge* prevEdge = &firstEdge;

  for(int i = 1; i< numberOfVertices; i++) {
    Vertex& currVertex = *vertexList[i];
    
    Edge& currEdge = addEdge(currVertex);
    setPointersOnEdgePair(currVertex, currEdge, *prevVertex, *prevEdge);
    currEdge.face = &f;
    prevVertex = &currVertex;
    prevEdge = &currEdge;

  }
  
  //clean up the pointer out of the last vertex
  setPointersOnEdgePair(firstVertex, firstEdge, *prevVertex, *prevEdge);
  prevEdge->face = &f; 
  
  return f;
}



void HalfEdge::print() {
  printf("HDS has %i vertices, %i edges, and %i faces\n",
	 vertexList.size(), edgeList.size(), faceList.size() );
}

/*
  Empties the HalfEdge
*/
void HalfEdge::clear() {
  for( std::vector<Edge*>::iterator itr = edgeList.begin();
    itr != edgeList.end(); ++itr) 
      delete *itr;
  for( std::vector<Face*>::iterator itr = faceList.begin();
    itr != faceList.end(); ++itr) 
    delete *itr;
  for( std::vector<Vertex*>::iterator itr = vertexList.begin();
    itr != vertexList.end(); ++itr)
      delete *itr;

  edgeList.clear();
  faceList.clear();
  vertexList.clear();
 
}

HalfEdge::~HalfEdge() {
  clear();
}

