#pragma once

#include "ShaderLoader.h"
#include "TriangleMesh.h"
#include "Camera.h"
#include <vector>
#include <cmath>
#include <GL/glut.h>

class SubdivScene {

public:
  SubdivScene();
 ~SubdivScene();

  void nextScenario();
 
  void nextMode();
  void split();

  void init();
  void draw();

  //Given a single face, draw 3 quads as selection targets for the 3 vertexes of the face.
  void drawQuads(Face* f);
  int select(int x, int y, int width, int height, float fovy, Face* f = NULL);
  Face* selectFace(int x, int y, int width, int height, float fovy);
  bool selectVertex(int x, int y, int width, int height, float fovy);
  Vertex* getSelectedVertex();
  void unselectVertex();
  void moveVertexToCamera(Vertex* aVertex);
  bool repositionSelectedVertex(int x, int y);

  //set some lighting parameters
  void lighting (void);
  Camera& getCamera();

private:

    //The scenario that is currently selected
    int scenario;
    static const int NUMBER_OF_SCENARIOS = 3;


    typedef std::vector<Face*>::iterator FaceIterator;
    typedef std::vector<Edge*>::iterator EdgeIterator;

    //The triangle mesh object containing the 
    TriangleMesh HDS;


    //for selecting between the two modes
    int currentMode;
    GLenum MODE_LIST[2]; // = { GL_POLYGON, GL_LINE_LOOP};;

    //the scene list containing the scene
    GLuint scene_list_id;

    bool redrawRequired;

    //TODO: possibly should be related to something? 
    static const GLuint SELECT_BUFFER_SIZE = 10240;
    Vertex* selectedVertex; //Currently selected vertex
    Camera* camera; //The camera for the scene.  

    // A triangle
    void scenario1();
    //Two triangles, sharing an edge
    void scenario2();
    //A triangular prism (no boundary edges)
    void scenario3();

    void initCamera();

    void drawData(const VertexData& data);
    void drawHDS(bool select = false);

    //process the hits in the selection buffer.
    //return index of nearest hit.
    int processHits(GLuint hitlist, GLuint buffer[]);  

};
