#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

typedef float point3[3];

struct Color {
    float r, g, b;
};

struct Point { 
    float x, y, z;
    Color color;
};

const int N = 50;
vector<vector<Point>> points;

// 1.0, because:
// 0 <= u <= 1.0
// 0 <= v <= 1.0
float step = 1.0 / N;

// 1- points, 2- mesh, 3 - fulfilled triangles
int mode = 2;

static GLfloat theta[] = {0.0, 0.0, 0.0};

void resizeVectors() {

    points.resize(N + 1);

    for (int i = 0; i < N + 1; i++)
    {
        points[i].resize(N + 1);
    }
}

float calcX(float u, float v) {
    return (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(M_PI * v);
}

float calcY(float u, float v) {
    return 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2) - 4.0; // minus 4.0 to fit inside the window
}

float calcZ(float u, float v) {
    return (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(M_PI * v);
}

void randomizeColors() {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            points[i][j].color.r = rand() / float(RAND_MAX);
            points[i][j].color.g = rand() / float(RAND_MAX);
            points[i][j].color.b = rand() / float(RAND_MAX);
        }
    }
}

void calculatePoints() {

    float u, v;

    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < N + 1; j++) {
            u = i * step;
            v = j * step;

            points[i][j].x = calcX(u, v);
            points[i][j].y = calcY(u, v);
            points[i][j].z = calcZ(u, v);
        }
    }
}

void printPoints() {

    glColor3f(1, 1, 0);

    glBegin(GL_POINTS);

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                Point point = points[i][j];
                glVertex3f(point.x, point.y, point.z);
            }
        }

    glEnd();
}

void printLines() {

    glColor3f(0, 1, 0);

    glBegin(GL_LINES);

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                Point point = points[i][j];
                Point point2 = points[i][j + 1];
                Point point3 = points[i + 1][j + 1];
                Point point4 = points[i + 1][j];

                // horizontally
                glVertex3f(point.x, point.y, point.z);
                glVertex3f(point2.x, point2.y, point2.z);

                // diagonally
                glVertex3f(point.x, point.y, point.z);
                glVertex3f(point3.x, point3.y, point3.z);

                // vertically
                glVertex3f(point.x, point.y, point.z);
                glVertex3f(point4.x, point4.y, point4.z);
            }
        }

    glEnd();
}

void printTriangles() {

    glColor3f(0, 0, 1);

    glBegin(GL_TRIANGLES);

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                Point point = points[i][j];
                Point point2 = points[i][j + 1];
                Point point3 = points[i + 1][j];
                Point point4 = points[i + 1][j + 1];

                glColor3f(point.color.r, point.color.g, point.color.b);
                glVertex3f(point.x, point.y, point.z);
                glColor3f(point2.color.r, point2.color.g, point2.color.b);
                glVertex3f(point2.x, point2.y, point2.z);
                glColor3f(point3.color.r, point3.color.g, point3.color.b);
                glVertex3f(point3.x, point3.y, point3.z);

                glColor3f(point4.color.r, point4.color.g, point4.color.b);
                glVertex3f(point4.x, point4.y, point4.z);
                glColor3f(point2.color.r, point2.color.g, point2.color.b);
                glVertex3f(point2.x, point2.y, point2.z);
                glColor3f(point3.color.r, point3.color.g, point3.color.b);
                glVertex3f(point3.x, point3.y, point3.z);
            }
        }

    glEnd();
}

void Egg() {

    calculatePoints();

    switch(mode) {
        case 1: 
            printPoints();
            break;
        case 2:
            printLines();
            break;
        case 3: 
            printTriangles();
            break;
    }
}

void Axes() {

    point3 x_min = {-5.0, 0.0, 0.0};
    point3 x_max = {5.0, 0.0, 0.0};

    point3 y_min = {0.0, -5.0, 0.0};
    point3 y_max = {0.0, 5.0, 0.0};

    point3 z_min = {0.0, 0.0, -5.0};
    point3 z_max = {0.0, 0.0, 5.0};

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);        
        glVertex3fv(x_min);
        glVertex3fv(x_max);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f); 
    glBegin(GL_LINES);           
        glVertex3fv(y_min);
        glVertex3fv(y_max);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f); 
    glBegin(GL_LINES);           
        glVertex3fv(z_min);
        glVertex3fv(z_max);
    glEnd();
}

void RenderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    Axes();

    glRotatef(theta[0], 1.0, 0.0, 0.0);

    glRotatef(theta[1], 0.0, 1.0, 0.0);

    glRotatef(theta[2], 0.0, 0.0, 1.0);

    Egg();

    glFlush();

    glutSwapBuffers();
}

void keys(unsigned char key, int x, int y) {

    if (key == 'p')
        mode = 1;
    if (key == 'w')
        mode = 2;
    if (key == 's')
        mode = 3;

    RenderScene();
}

void spinEgg() {

    theta[0] -= 0.5;
    
    if (theta[0] > 360.0)
        theta[0] -= 360.0;

    theta[1] -= 0.5;

    if (theta[1] > 360.0)
        theta[1] -= 360.0;

    theta[2] -= 0.5;

    if (theta[2] > 360.0)
        theta[2] -= 360.0;

    glutPostRedisplay();
}

void MyInit(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {
    
    GLfloat AspectRatio;

    if (vertical == 0) 
        vertical = 1;

    glViewport(0, 0, horizontal, vertical);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

    if (horizontal <= vertical)
        glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
    else
        glOrtho(-7.5 * AspectRatio, 7.5 * AspectRatio, -7.5, 7.5, 10.0, -10.0);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);

    resizeVectors();

    srand(time(NULL));

    randomizeColors();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 800);

    glutCreateWindow("3D Egg - 241269");

    glutKeyboardFunc(keys);

    glutIdleFunc(spinEgg);

    glutDisplayFunc(RenderScene);

    glutReshapeFunc(ChangeSize);

    MyInit();

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}
