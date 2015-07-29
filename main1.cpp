void initOpenGL () {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1,1,-1,1,1,5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,0,0,0,-1,0,1,0);
}

void display() {
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
    glutInitWindowSize(512, 512);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow("Hello World");
    initOpenGL();

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}