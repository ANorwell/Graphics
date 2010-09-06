
#include "TestEnv.h"
#include <string>
#include "../Texture.h"
#include "../Util.h"


void display();

int main(int argc, char** argv) {
    testInitMain(argc, argv);

    glEnable(GL_TEXTURE_2D);
    Log::configure(std::string("test.log"), Log::DEBUG);
    std::string fname( (argc > 1) ? argv[1] : "src/tex2.bmp");
    BMPData bmp(fname);
    GLuint texId = 1;
    glGenTextures(1, &texId);
    bmp.setGLParams(texId);

    //    RAWData raw(std::string("src/texture.raw"), 512,512);
    //GLuint tex2 = 2;
    //glGenTextures(1, &tex2);
    //raw.setGLParams(tex2);

    
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

void display() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 2.0,
               0.0, 0.0, 0.0,
               0.0, 1.0, 0.0);     

    glBegin(GL_QUADS);
    //glColor3f(0.5,0.0,0.0);
    glTexCoord2f (0.0, 0.0);
    glVertex3f(0.0,0.0,0.0);

    glTexCoord2f (1.0, 0.0);
    glVertex3f(1.0,0.0,0.0);

    glTexCoord2f (1.0, 1.0);
    glVertex3f(1.0,1.0,0.0);

    glTexCoord2f (0.0, 1.0);
    glVertex3f(0.0,1.0,0.0);
    glEnd();
    glutSwapBuffers();
}

    
    
    
