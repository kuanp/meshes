#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include "SimpleImage.h"
:x

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

Point3f viewPt, viewCtr, viewUp;

void initOpenGL () {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1,1,-1,1,1,5);

    viewPt = Point3f(0,0,0);
    viewCtr = Point3f(0,0,-2);
    viewUp = Point3f(0,1,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(viewPt.x, viewPt.y, viewPt.z,
	    viewCtr.x, viewCtr.y, viewCtr.z,
	    viewUp.x, viewUp.y, viewUp.z);
}

void mouseClicked(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
	xstart = x;
	ystart = y;
    }
    std::cout << "x start" << xstart << " ystart: " << ystart << "\n";
}

void mouseMoved(int x, int y) {
    glMatrixMode(GL_MODELVIEW);

    if (x != xstart) {
	std::cout << "x " << viewCtr.x << " y: " << viewCtr.y << " z " << viewCtr.z << "\n";
	glTranslatef(viewCtr.x, viewCtr.y, viewCtr.z);
	glRotatef(xstart - x, viewUp.x, viewUp.y, viewUp.z);
	glTranslatef(-viewCtr.x, -viewCtr.y, -viewCtr.z);
    }

    if ( y != ystart) {

    }

    glutPostRedisplay();
    xstart = x;
    ystart = y;
}

void keyPressed(unsigned char key, int x, int y) {
    Point3f diff = (-(viewCtr - viewPt)).normal();
    if (key == 'w') {
	glTranslatef(diff.x, diff.y, diff.z);
	viewCtr = viewCtr + diff;
	viewPt = viewPt + diff;
    } else if (key == 's') {
	diff = -diff;
	glTranslatef(diff.x, diff.y, diff.z);
	viewCtr = viewCtr + diff;
	viewPt = viewPt + diff;
    }
    glutPostRedisplay();
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
    glutMouseFunc(mouseClicked);
    glutKeyboardFunc(keyPressed);
    glutMotionFunc(mouseMoved);
    glutMainLoop();
    return 0;
}