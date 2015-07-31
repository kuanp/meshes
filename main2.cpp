#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

using namespace std;
#include <fstream>
#include <iostream>
#include <vector>
#include "SimpleImage.h"
#include "util.h"

const int DISP_SIZE = 512;
int xstart;
int ystart;

Point3f viewPt, viewCtr, viewUp;
vector<Point3f> vertices;
vector<Point3f> normals;
vector<Point2f> textures;
vector<Triangle3f> faces;

void readFile(string filename) {
    ifstream input(filename);
    string linebuffer;

    while (input && getline(input, linebuffer)) {
	//cout << linebuffer.substr(0,2) << "\n";
	if(linebuffer.length() == 0) continue;
	if(linebuffer[0] == '#') continue;
	if(linebuffer.substr(0,2) == "v ") {
	    Point3f v;
	    //cout << "actual line: " << linebuffer;
	    sscanf(linebuffer.c_str(), "v %f %f %f", &v.x, &v.y, &v.z);
	    //cout << " " << v.x << " " << v.y << " " << v.z << "\n";
	    vertices.push_back(v);
	} else if (linebuffer.substr(0, 2) == "vn") {
	    Point3f v;
	    sscanf(linebuffer.c_str(), "vn %f %f %f", &v.x, &v.y, &v.z);
	    normals.push_back(v);
	} else if (linebuffer.substr(0, 2) == "vt") {
	    Point2f v;
	    sscanf(linebuffer.c_str(), "vt %f %f", &v.x, &v.y);
	    textures.push_back(v);
	} else if (linebuffer.substr(0, 2) == "vp") {
	} else if (linebuffer.substr(0, 2) == "f ") {
	    Triangle3f tri;

	    char s1[20], s2[20], s3[20];
	    cout << "actual line: " << linebuffer;
	    // cout << v1<< v2<< v3<< t1<< t2<< t3<< n1 << n2 << n3 <<" \n";
	    sscanf(linebuffer.c_str(), "f %*d%s %s %s", s1, s2, s3);
	    // cout << s1 << s2 << s3 << "\n";
	    string test = (string) s1;

	    // do test to determine the format of the faces.
	    if (s1[0] == '/') {
		if (s1[1] == '/') {
		    cout << "case double slash\n";
		} else if (test.find('/', 1) != -1){
		    cout << "all three here \n";
		} else {
		    cout << "no normal \n";
		}
	    } else {
		cout << "only vertex data\n";
		int a, b, c;
		sscanf(linebuffer.c_str(), "f %d %d %d", &a, &b, &c);
//		cout << a << b << c <<"\n";
		tri = Triangle3f(vertices[a], vertices[b], vertices[c]);
		cout << tri.a.x << "\n";
	    }
	}
    }
}

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
	glTranslatef(viewCtr.x, viewCtr.y, viewCtr.z);
	glRotatef(xstart - x, viewUp.x, viewUp.y, viewUp.z);
	glTranslatef(-viewCtr.x, -viewCtr.y, -viewCtr.z);
    }

    if ( y != ystart) {
	glTranslatef(viewCtr.x, viewCtr.y, viewCtr.z);
	glRotatef(ystart - y, 1, 0, 0);
	glTranslatef(-viewCtr.x, -viewCtr.y, -viewCtr.z);
    }

    glutPostRedisplay();
    xstart = x;
    ystart = y;
}

void keyPressed(unsigned char key, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    if (key == 'w') {
	glScalef(2, 2, 1);
    } else if (key == 's') {
	glScalef(.5, .5, 1);
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

int main(int argc, char *argv[]) {
    if (argc > 1) {
        readFile(string(argv[1]));
    }
    glutInit(&argc,argv);
    glutInitWindowSize(DISP_SIZE, DISP_SIZE);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("Hello World");
    initOpenGL();

    glutDisplayFunc(display);
    glutMouseFunc(mouseClicked);
    glutKeyboardFunc(keyPressed);
    glutMotionFunc(mouseMoved);
    glutMainLoop();
    return 0;
}