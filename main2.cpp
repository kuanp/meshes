#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include "SimpleImage.h"
#include "util.h"

void screenshot(){
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    float out[3 * w * h];
    RGBColor BG(0,0,0);
    SimpleImage shot(w, h, BG);
    glReadPixels(0, 0, w, h, GL_RGB, GL_FLOAT, &out[0]);
    for(int y = 0; y < h; ++y){
	for(int x = 0; x < w; ++x){
	    int index = (3*w*y) + 3*x;
	    float red = out[index];
	    float green = out[index + 1];
	    float blue = out[index + 2];
	    shot.set(x,h-y,RGBColor(red, green, blue));
	}
    }
    shot.save("screenshot.png");
}

const int DISP_SIZE = 512;
int xstart;
int ystart;

Vector viewPt;
Vector viewCenter;
Vector viewUp;

void initOpenGL () {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1,1,-1,1,1,5);

    viewPt.v1 = 0;
    viewPt.v2 = 0;
    viewPt.v3 = 0;
    viewCenter.v1 = 0;
    viewCenter.v2 = 0;
    viewCenter.v3 = -1;
    viewUp.v1 = 0;
    viewUp.v2 = 1;
    viewUp.v3 = 0;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,0,0,0,-1,0,1,0);
//    gluLookAt(viewPt.v1, viewPt.v2, viewPt.v3,
//	    viewCenter.v1, viewCenter.v2, viewCenter.v3,
//	    viewUp.v1, viewUp.v2, viewUp.v3);
}

void mouseClicked(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
	xstart = x;
	ystart = y;
    }
}

void mouseMoved(int x, int y) {
    //Vector diff = subtractV(viewPt, viewCenter);
    glMatrixMode(GL_MODELVIEW);
    if (x != xstart) {
	std::cout << "x " << x << " y: " << y << "\n";
	glTranslatef(0,0,1);
	//glRotatef( (xstart-x), 0, 1, 0);
	glRotatef(45, 0, 1, 0);
	glTranslatef(.707,0,.707);
    }

    glutPostRedisplay();
    xstart = x;
    ystart = y;
}

void display() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(0,0,1);
    glVertex3f(-1,-1, -2);
    glVertex3f( 1,-1, -2);
    glVertex3f( 0, 1, -2);
    glEnd();
    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc,argv);
    glutInitWindowSize(DISP_SIZE, DISP_SIZE);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("Hello World");
    initOpenGL();

    std::cout<< "xxxxxxx\n";
    glutDisplayFunc(display);
    glutMotionFunc(mouseMoved);
    glutMainLoop();
    return 0;
}