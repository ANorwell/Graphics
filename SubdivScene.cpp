#include "SubdivScene.h"

SubdivScene::SubdivScene() : scenario(0), currentMode(0) {
    camera = new Camera();
    MODE_LIST[0] = GL_POLYGON;
    MODE_LIST[1] = GL_LINE_LOOP;
}

SubdivScene::~SubdivScene() {
    delete camera;
}

//////////////////////////////////////////////////////////
//  The definitions of the scenarios
//////////////////////////////////////////////////////////

// A triangle
void SubdivScene::scenario1() {

  VertexData w1(1,0,0);
  w1.colour(1,0,0);
  VertexData w2(0,1,0);
  w2.colour(1,1,0);
  VertexData w3(0,0,0);
  w3.colour(0,0,1);
  VertexData w4(0,1,1);
  HDS.beginPolygon(w1);
  HDS.nextVertex(w2);
  Edge& e = HDS.nextVertex(w3);
  Face& f = HDS.endPolygon();
}

//Two triangles, sharing an edge
void SubdivScene::scenario2() {

  VertexData w1(1,0,0);
  w1.colour(1,0,0);
  VertexData w2(0,1,0);
  w2.colour(1,1,0);
  VertexData w3(0,0,0);
  w3.colour(0,0,1);
  VertexData w4(0,1,1);
  
  HDS.beginPolygon(w1);
  HDS.nextVertex(w2);
  Edge& e = HDS.nextVertex(w3);
  Face& f = HDS.endPolygon();

  HDS.beginPolygon(e);
  Edge& e2 = HDS.nextVertex(w4);
  HDS.endPolygon(); 
  
}

//A triangular prism (no boundary edges)
void SubdivScene::scenario3() {

  VertexData w1(5,0,0);
  w1.colour(1,0,0);
  VertexData w2(0,0,10);
  w2.colour(1,1,0);
  VertexData w3(-5,0,0);
  w3.colour(0,0,1);
  VertexData w4(0,10,0);
  
  HDS.beginPolygon(w1);
  HDS.nextVertex(w2);
  Edge& e = HDS.nextVertex(w3); //edge from w3 to w1
  e.vertex->print();
  Face& f = HDS.endPolygon();

  HDS.beginPolygon(e);
  HDS.nextVertex(w4);
  HDS.endPolygon(); 
  const std::vector<Vertex*>& vList = HDS.getVertexList();
  Vertex* list[3] = { vList[2], vList[1], vList[3] };
  HDS.makePolygon(3, list);
  Vertex* list2[3] = { vList[1], vList[0], vList[3] };
  HDS.makePolygon(3, list2);

  const std::vector<Face*> fList = HDS.getFaceList();

  for(unsigned int i=0; i< fList.size(); i++) {
    HDS.checkConsistency(*fList[i]);
  }
}

void SubdivScene::initCamera() {
    camera->setPosition(0,0,10);
    camera->setLook(0,0,-1);
    camera->setSpeed(3);
    camera->setAngleSpeed(20);
}


///////////////////////////////////////////
//  Main logic for constructing the HDS
////////////////////////////////////////
void SubdivScene::init() {

    initCamera();
  
  //clear the HDS and list
  HDS.clear();
  if (glIsList(scene_list_id))
    glDeleteLists(scene_list_id, 1);

  //make a new list
  scene_list_id = glGenLists(1);

  redrawRequired = true;

  switch(scenario) {
  case 0:
    printf("==Scenario 1:==\n");
    scenario1();
    break;
  case 1:
    printf("==Scenario 2:==\n");
    scenario2();
    break;
  case 2:
    printf("==Scenario 3:==\n");
    scenario3();
    break;
  default:
    break;
  }

  HDS.print();
}

void SubdivScene::drawData(const VertexData& data) {

  if (data.hasColour)
    glColor3f( data.c.r, data.c.g, data.c.b);

  glVertex3f( data.p.x, data.p.y, data.p.z);
}

/////////////////////////////////////////////  
// Draw the HDS
///////////////////////////////////////////
void SubdivScene::drawHDS(bool select) {

  GLuint faceCounter = 0;

  LOG(Log::DEBUG) << "Drawing the HDS";
  FaceIterator fitr = HDS.faceBegin();

  for(; fitr != HDS.faceEnd(); fitr++) {
    
    if (select)
      glLoadName(faceCounter++);

    glBegin(MODE_LIST[currentMode]);

    Face::iterator itr = (*fitr)->begin();
    Face::iterator end = (*fitr)->begin();
    
    int iteration = 0;
    do {
      if (iteration > 6) {
	      printf("ERROR IN DRAWING\n");
	      break;
      }
      drawData(itr->getData());
      ++itr;
      ++iteration;
    } while (itr != end);
    
    glEnd();

  }
}

/*
  process the hits in the selection buffer.  Return the index of the nearest hit.

*/
int SubdivScene::processHits(GLuint hitlist, GLuint buffer[]) {

  GLuint* ptr = (GLuint *) buffer;
  GLuint names;
  
  GLuint nr;
  GLuint fr;
  GLuint result;
  bool found = false;

//  vector<GLuint> hitList;

  printf("Processing hits: %i to process.\n", hitlist);

  for( unsigned int i=0; i<hitlist; i++) {
    names = *ptr;  ++ptr;
    printf("  for hit %i, found %i names.\n", i, names);

    nr = *ptr; ++ptr;
    fr = *ptr; ++ptr;

    result = *ptr;
    found = true;
    ptr += names;

    printf ("\n");

  }

  if (found)
    return (int) result;
  else
    return -1;
}

void SubdivScene::nextScenario() {
    scenario = (++scenario) % NUMBER_OF_SCENARIOS;
    redrawRequired = true;
    selectedVertex = NULL;
  }

 
void SubdivScene::nextMode() {
    currentMode = 1 - currentMode;
    redrawRequired = true;
}

void SubdivScene::split() {

  //split the mesh
  HDS.split();
  HDS.print();

  redrawRequired = true;
  

  }

void SubdivScene::draw() {

    if (redrawRequired) {
        //save the new scene as a list
        glNewList(scene_list_id, GL_COMPILE);
        drawHDS();
        glEndList();
        redrawRequired = false;
    }


    glCallList(scene_list_id);
}

  //Given a single face, draw 3 quads as selection targets for the 3 vertexes of the face.
void SubdivScene::drawQuads(Face* f) {
    Vec centre;
    std::vector<Vertex*> vertexList;

    for( Face::iterator itr = f->begin(); !itr.isDone(); ++itr) {
        vertexList.push_back(&*itr);
        centre = centre + itr->getData().p;
    }

 
    GLuint i = 0;
    for( Face::iterator itr = f->begin(); !itr.isDone(); ++itr) {
      glLoadName(i);
      glBegin(GL_QUADS);
      Vec v = itr->getData().p;

      Vec next = (v + vertexList[ ((i + 1) % vertexList.size() )]->getData().p ) / 2;
      Vec prev = (v + vertexList[ ((i + vertexList.size()- 1) % vertexList.size() )]->getData().p ) / 2;
      prev.print();
      glVertex3f( v.x, v.y, v.z );
      glVertex3f( next.x, next.y, next.z);
      glVertex3f(centre.x, centre.y, centre.z);
      glVertex3f(prev.x, prev.y, prev.z);

      glEnd();
      i++;
    }
}

int SubdivScene::select(int x, int y, int width, int height, float fovy, Face* f) {
    GLuint buffer[SELECT_BUFFER_SIZE];
    glSelectBuffer(SELECT_BUFFER_SIZE, buffer);
    glRenderMode(GL_SELECT);
    glInitNames();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, (GLint*) &viewport);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(x,y, 2, 2, (GLint*) &viewport);
    gluPerspective(fovy, (GLfloat) width/height, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    
    glPushName(0);

    //draw the scene we will select from
    if (f)
      drawQuads(f);
    else
      drawHDS(true);
      

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    const GLuint hitlist = glRenderMode(GL_RENDER);
    return processHits(hitlist, buffer);
}


Face* SubdivScene::selectFace(int x, int y, int width, int height, float fovy) {
    int hit = select(x,y,width, height, fovy);

    if (hit<0)
      return NULL;

    Face* f = HDS.getFaceList()[hit];
    redrawRequired = true;
    return f;
}


bool SubdivScene::selectVertex(int x, int y, int width, int height, float fovy) {
    Face* f = selectFace(x,y,width, height, fovy);

    if (!f)
      return false;

    //select from the 3 quads corresponding the face selected.  
    int faceHit = select(x,y,width, height, fovy, f);

    if (faceHit<0) {
      printf("ERROR: found a face but not a vertex in selectVertex");
      return false;
    }
    
    this->selectedVertex = (*f)[ (unsigned int) faceHit];
    return true;
}

void SubdivScene::unselectVertex() {
    this->selectedVertex = NULL;
}


bool SubdivScene::repositionSelectedVertex(int x, int y) {
    if (!selectedVertex)
        return false;

    printf("Repositioning vertex, (%i, %i)...\n", x,y);
    //In the plane perpendicular to camera->look, and through which the vertex passes, 
    //move vertex from where it is to where the mouse is

    //Use current vertex position to calculate the center of the screen
    Vec camPos = camera->getPos();
    Vec vPos = selectedVertex->getData().p;
    Vec diff = vPos - camPos;
    Vec look = camera->getLook().normalize();
    float cosAngle = sqrt(diff.normalize().dot(look));
    float lookLength = diff.length()*cosAngle;
    Vec centreDiff = look*lookLength;
    Vec diffInPlane = vPos - (camPos + centreDiff);
printf(" diffInPlane is"); diffInPlane.print();
printf(" Cam pos is"); camPos.print();


    float anglex = (x - Globals::getWidth()/2)*Globals::getFovx() * Globals::PI/180/Globals::getWidth();
    float angley = (y - Globals::getHeight()/2)*Globals::getFovy() * Globals::PI/180/Globals::getHeight();
    float hLength = tan(anglex)*centreDiff.length();
    float vLength = tan(angley)*centreDiff.length();
printf(" anglex is %f and angley is %f, fovx is %f\n", anglex, angley, Globals::getFovx());
printf(" hLength is %f and vLength is %f, hrat is %i\n", hLength, vLength, x - Globals::getWidth()/2);


    //calculate the shifts in the horiz and vertical directions
    Vec right = camera->getLook().cross(camera->getUp()).normalize()*hLength;
    Vec up = camera->getUp().normalize()*vLength;

printf(" Right is"); right.print();
printf(" up is"); up.print();

    Vec newPos = vPos + right + up - diffInPlane;
    selectedVertex->getData().p = newPos;

    printf("  Old position was (%f, %f, %f)\n", vPos.x, vPos.y, vPos.z);
    printf("  New position is (%f, %f, %f)\n", newPos.x, newPos.y, newPos.z);

    redrawRequired = true;
}

Vertex* SubdivScene::getSelectedVertex() { return selectedVertex; }

void SubdivScene::moveVertexToCamera(Vertex* aVertex) {
    aVertex->getData().p = camera->getPos() + camera->getLook().normalize();
}

//set some lighting parameters
void SubdivScene::lighting (void) {
    GLfloat LightPosition[] = { 9.0, 0.0, 9.0, 1.0};

    GLfloat DiffuseLight[] = {1.0, 0.3, 0.3}; 
    GLfloat AmbientLight[] = {0.3, 0.3, 0.3};
    GLfloat SpecularLight[] = {1.0, 1.0, 1.0};

    glLightfv (GL_LIGHT0, GL_SPECULAR, SpecularLight); 
    glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); 
    glLightfv (GL_LIGHT0, GL_AMBIENT, AmbientLight); 
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

    GLfloat mShininess[] = {32};

    GLfloat DiffuseMaterial[] = {1.0, 0.0, 0.0}; 
    GLfloat AmbientMaterial[] = {0.2, 0.2, 0.2};
    GLfloat SpecularMaterial[] = {1.0, 1.0, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DiffuseMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, AmbientMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SpecularMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess); 
}


Camera& SubdivScene::getCamera() { return *camera; };
