#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <time.h>

// ================= VARIABEL GLOBAL =================
float posY = 4.5f;
float posX = -0.5f;
int rencent_maze = 1;

bool is_isometric = false;
float mazeRotX = -60.0f;
float mazeRotZ = 45.0f;

// Sumbu 1 = Sumbu X (Putaran Atas ke Bawah)
int nimRotAxis = 1;
float nimAngle = 0.0f;
float nimOx = -4.0f, nimOy = -5.0f;

// ================= LOGIKA TABRAKAN & POSISI ACAK =================
bool isOverlap(float minX1, float minY1, float maxX1, float maxY1, float wMinX, float wMinY, float wMaxX, float wMaxY) {
    return !(maxX1 <= wMinX || minX1 >= wMaxX || maxY1 <= wMinY || minY1 >= wMaxY);
}

bool isCellEmpty(float cx, float cy) {
    float cMinX = cx + 0.1f, cMaxX = cx + 0.9f;
    float cMinY = cy + 0.1f, cMaxY = cy + 0.9f;

    // Dinding Luar
    if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  0.0,  4.0,  5.0,  5.0)) return false;
    if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  4.0, -5.0,  5.0,  5.0)) return false;
    if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  0.0, -6.0,  5.0, -5.0)) return false;
    if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -6.0, -6.0, -1.0, -5.0)) return false;
    if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -6.0, -5.0, -5.0,  5.0)) return false;
    if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -6.0,  4.0, -1.0,  5.0)) return false;

    // Dinding Dalam
    if (rencent_maze == 1) {
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -2.0, -3.0, -1.0, -1.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -2.0, -4.0,  0.0, -3.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  2.0, -4.0,  4.0, -3.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  2.0, -2.0,  3.0,  3.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  0.0, -2.0,  3.0, -1.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  0.0, -5.0,  1.0, -1.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -2.0,  2.0,  0.0,  3.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  0.0,  0.0,  1.0,  3.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -4.0,  0.0,  1.0,  1.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -4.0, -5.0, -3.0,  1.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -5.0,  2.0, -3.0,  3.0)) return false;
    } else {
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  3.0,  0.0,  4.0,  1.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  2.0,  0.0,  3.0,  2.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  0.0,  2.0,  3.0,  3.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -4.0, -4.0, -2.0, -3.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -2.0, -5.0, -1.0, -3.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  2.0, -4.0,  4.0, -3.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY,  0.0, -5.0,  1.0,  1.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -3.0, -2.0,  3.0, -1.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -4.0, -2.0, -3.0,  2.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -4.0,  2.0, -2.0,  3.0)) return false;
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, -2.0,  0.0, -1.0,  3.0)) return false;
    }

    // Titik Player Start
    if (cx == -1.0f && cy == 4.0f) return false;
    if (cx ==  0.0f && cy == 4.0f) return false;

    return true;
}

void pickRandomNimPos() {
    bool posisiKetemu = false;
    while (!posisiKetemu) {
        int randX = (rand() % 10) - 5;
        int randY = (rand() % 10) - 5;
        if (isCellEmpty((float)randX, (float)randY)) {
            nimOx = (float)randX;
            nimOy = (float)randY;
            posisiKetemu = true;
        }
    }
}

bool checkCollisionPlayer(float newX, float newY) {
    float s = 0.3f;
    float pMinX = newX - s, pMaxX = newX + s;
    float pMinY = newY - s, pMaxY = newY + s;

    if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  0.0,  4.0,  5.0,  5.0)) return true;
    if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  4.0, -5.0,  5.0,  5.0)) return true;
    if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  0.0, -6.0,  5.0, -5.0)) return true;
    if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -6.0, -6.0, -1.0, -5.0)) return true;
    if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -6.0, -5.0, -5.0,  5.0)) return true;
    if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -6.0,  4.0, -1.0,  5.0)) return true;

    if (rencent_maze == 1) {
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -2.0, -3.0, -1.0, -1.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -2.0, -4.0,  0.0, -3.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  2.0, -4.0,  4.0, -3.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  2.0, -2.0,  3.0,  3.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  0.0, -2.0,  3.0, -1.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  0.0, -5.0,  1.0, -1.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -2.0,  2.0,  0.0,  3.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  0.0,  0.0,  1.0,  3.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -4.0,  0.0,  1.0,  1.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -4.0, -5.0, -3.0,  1.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -5.0,  2.0, -3.0,  3.0)) return true;
    } else {
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  3.0,  0.0,  4.0,  1.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  2.0,  0.0,  3.0,  2.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  0.0,  2.0,  3.0,  3.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -4.0, -4.0, -2.0, -3.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -2.0, -5.0, -1.0, -3.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  2.0, -4.0,  4.0, -3.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY,  0.0, -5.0,  1.0,  1.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -3.0, -2.0,  3.0, -1.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -4.0, -2.0, -3.0,  2.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -4.0,  2.0, -2.0,  3.0)) return true;
        if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, -2.0,  0.0, -1.0,  3.0)) return true;
    }
    return false;
}

// ================= RENDER NIM 3D & 2D =================
void drawBox3D(float x1, float y1, float z1, float x2, float y2, float z2) {
    glBegin(GL_QUADS);
    glVertex3f(x1, y1, z2); glVertex3f(x2, y1, z2); glVertex3f(x2, y2, z2); glVertex3f(x1, y2, z2);
    glVertex3f(x2, y1, z1); glVertex3f(x1, y1, z1); glVertex3f(x1, y2, z1); glVertex3f(x2, y2, z1);
    glVertex3f(x1, y1, z1); glVertex3f(x1, y1, z2); glVertex3f(x1, y2, z2); glVertex3f(x1, y2, z1);
    glVertex3f(x2, y1, z2); glVertex3f(x2, y1, z1); glVertex3f(x2, y2, z1); glVertex3f(x2, y2, z2);
    glVertex3f(x1, y1, z1); glVertex3f(x2, y1, z1); glVertex3f(x2, y1, z2); glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z2); glVertex3f(x2, y2, z2); glVertex3f(x2, y2, z1); glVertex3f(x1, y2, z1);
    glEnd();
}

void drawNIM3D() {
    float d = 1.5f;
    glColor3f(0.0f, 0.0f, 1.0f);
    drawBox3D(-10, 4, 0, -8, 6, d); drawBox3D(-8, 4, 0, -6, 6, d); drawBox3D(-6, 4, 0, -4, 6, d);
    drawBox3D(-6, 2, 0, -4, 4, d); drawBox3D(-6, 0, 0, -4, 2, d); drawBox3D(-6,-2, 0, -4, 0, d);
    drawBox3D(-6,-4, 0, -4,-2, d); drawBox3D(-8,-4, 0, -6,-2, d); drawBox3D(-10,-4, 0,-8,-2, d);
    drawBox3D(-10,-2, 0,-8, 0, d); drawBox3D(-10, 0, 0,-8, 2, d); drawBox3D(-10, 2, 0,-8, 4, d);

    drawBox3D(-2, 4, 0,  0, 6, d); drawBox3D( 0, 4, 0,  2, 6, d); drawBox3D( 2, 4, 0,  4, 6, d);
    drawBox3D( 2, 2, 0,  4, 4, d); drawBox3D( 2, 0, 0,  4, 2, d); drawBox3D( 2,-2, 0,  4, 0, d);
    drawBox3D( 2,-4, 0,  4,-2, d); drawBox3D( 0,-4, 0,  2,-2, d); drawBox3D(-2,-4, 0,  0,-2, d);
    drawBox3D(-2,-2, 0,  0, 0, d); drawBox3D(-2, 0, 0,  0, 2, d); drawBox3D(-2, 2, 0,  0, 4, d);

    drawBox3D( 6, 0, 0,  8, 2, d); drawBox3D( 8, 0, 0, 10, 2, d); drawBox3D( 6, 2, 0,  8, 4, d);
    drawBox3D( 6, 4, 0,  8, 6, d); drawBox3D( 8, 4, 0, 10, 6, d); drawBox3D(10, 4, 0, 12, 6, d);
    drawBox3D(10, 2, 0, 12, 4, d); drawBox3D(10, 0, 0, 12, 2, d); drawBox3D(10,-2, 0, 12, 0, d);
    drawBox3D(10,-4, 0, 12,-2, d); drawBox3D( 8,-4, 0, 10,-2, d); drawBox3D( 6,-4, 0,  8,-2, d);
}

void drawBox2D(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1); glVertex2f(x2, y1); glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
}

void drawNIM2D() {
    glColor3f(0.0f, 0.0f, 1.0f);
    drawBox2D(-10, 4, -8, 6); drawBox2D(-8, 4, -6, 6); drawBox2D(-6, 4, -4, 6);
    drawBox2D(-6, 2, -4, 4); drawBox2D(-6, 0, -4, 2); drawBox2D(-6,-2, -4, 0);
    drawBox2D(-6,-4, -4,-2); drawBox2D(-8,-4, -6,-2); drawBox2D(-10,-4, -8,-2);
    drawBox2D(-10,-2, -8, 0); drawBox2D(-10, 0, -8, 2); drawBox2D(-10, 2, -8, 4);

    drawBox2D(-2, 4,  0, 6); drawBox2D( 0, 4,  2, 6); drawBox2D( 2, 4,  4, 6);
    drawBox2D( 2, 2,  4, 4); drawBox2D( 2, 0,  4, 2); drawBox2D( 2,-2,  4, 0);
    drawBox2D( 2,-4,  4,-2); drawBox2D( 0,-4,  2,-2); drawBox2D(-2,-4,  0,-2);
    drawBox2D(-2,-2,  0, 0); drawBox2D(-2, 0,  0, 2); drawBox2D(-2, 2,  0, 4);

    drawBox2D( 6, 0,  8, 2); drawBox2D( 8, 0, 10, 2); drawBox2D( 6, 2,  8, 4);
    drawBox2D( 6, 4,  8, 6); drawBox2D( 8, 4, 10, 6); drawBox2D(10, 4, 12, 6);
    drawBox2D(10, 2, 12, 4); drawBox2D(10, 0, 12, 2); drawBox2D(10,-2, 12, 0);
    drawBox2D(10,-4, 12,-2); drawBox2D( 8,-4, 10,-2); drawBox2D( 6,-4,  8,-2);
}

// ================= RENDER DINDING MAZE =================
void drawMazeWall(float x1, float y1, float z1, float x2, float y2, float z2) {
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.4f, 1.0f);
    glVertex3f(x1, y1, z2); glVertex3f(x2, y1, z2); glVertex3f(x2, y2, z2); glVertex3f(x1, y2, z2);
    glColor3f(0.1f, 0.2f, 0.5f);
    glVertex3f(x1, y1, z1); glVertex3f(x1, y2, z1); glVertex3f(x2, y2, z1); glVertex3f(x2, y1, z1);
    glColor3f(1.0f, 0.2f, 0.2f);
    glVertex3f(x1, y1, z1); glVertex3f(x2, y1, z1); glVertex3f(x2, y1, z2); glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z1); glVertex3f(x1, y2, z2); glVertex3f(x2, y2, z2); glVertex3f(x2, y2, z1);
    glVertex3f(x1, y1, z1); glVertex3f(x1, y1, z2); glVertex3f(x1, y2, z2); glVertex3f(x1, y2, z1);
    glVertex3f(x2, y1, z1); glVertex3f(x2, y2, z1); glVertex3f(x2, y2, z2); glVertex3f(x2, y1, z2);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (is_isometric) {
        glRotatef(mazeRotX, 1.0f, 0.0f, 0.0f);
        glRotatef(mazeRotZ, 0.0f, 0.0f, 1.0f);
    }

    drawMazeWall( 0.0,  4.0, 0.0,  5.0,  5.0, 1.0);
    drawMazeWall( 4.0, -5.0, 0.0,  5.0,  5.0, 1.0);
    drawMazeWall( 0.0, -6.0, 0.0,  5.0, -5.0, 1.0);
    drawMazeWall(-6.0, -6.0, 0.0, -1.0, -5.0, 1.0);
    drawMazeWall(-6.0, -5.0, 0.0, -5.0,  5.0, 1.0);
    drawMazeWall(-6.0,  4.0, 0.0, -1.0,  5.0, 1.0);

    if (rencent_maze == 1) {
        drawMazeWall(-2.0, -3.0, 0.0, -1.0, -1.0, 1.0); drawMazeWall(-2.0, -4.0, 0.0,  0.0, -3.0, 1.0);
        drawMazeWall( 2.0, -4.0, 0.0,  4.0, -3.0, 1.0); drawMazeWall( 2.0, -2.0, 0.0,  3.0,  3.0, 1.0);
        drawMazeWall( 0.0, -2.0, 0.0,  3.0, -1.0, 1.0); drawMazeWall( 0.0, -5.0, 0.0,  1.0, -1.0, 1.0);
        drawMazeWall(-2.0,  2.0, 0.0,  0.0,  3.0, 1.0); drawMazeWall( 0.0,  0.0, 0.0,  1.0,  3.0, 1.0);
        drawMazeWall(-4.0,  0.0, 0.0,  1.0,  1.0, 1.0); drawMazeWall(-4.0, -5.0, 0.0, -3.0,  1.0, 1.0);
        drawMazeWall(-5.0,  2.0, 0.0, -3.0,  3.0, 1.0);
    } else {
        drawMazeWall( 3.0,  0.0, 0.0,  4.0,  1.0, 1.0); drawMazeWall( 2.0,  0.0, 0.0,  3.0,  2.0, 1.0);
        drawMazeWall( 0.0,  2.0, 0.0,  3.0,  3.0, 1.0); drawMazeWall(-4.0, -4.0, 0.0, -2.0, -3.0, 1.0);
        drawMazeWall(-2.0, -5.0, 0.0, -1.0, -3.0, 1.0); drawMazeWall( 2.0, -4.0, 0.0,  4.0, -3.0, 1.0);
        drawMazeWall( 0.0, -5.0, 0.0,  1.0,  1.0, 1.0); drawMazeWall(-3.0, -2.0, 0.0,  3.0, -1.0, 1.0);
        drawMazeWall(-4.0, -2.0, 0.0, -3.0,  2.0, 1.0); drawMazeWall(-4.0,  2.0, 0.0, -2.0,  3.0, 1.0);
        drawMazeWall(-2.0,  0.0, 0.0, -1.0,  3.0, 1.0);
    }

    // ================= NIM RENDER =================
    glPushMatrix();
    glTranslatef(nimOx + 0.5f, nimOy + 0.5f, 0.5f);

    if (is_isometric) {
        // Mode 3D: Putaran dari ATAS ke BAWAH (Sumbu X)
        if (nimRotAxis == 1) glRotatef(nimAngle, 1, 0, 0);
        else if (nimRotAxis == 2) glRotatef(nimAngle, 0, 1, 0);
        else if (nimRotAxis == 3) glRotatef(nimAngle, 0, 0, 1);

        glScalef(0.035f, 0.035f, 0.035f);
        glTranslatef(-1.0f, -1.0f, -0.75f);
        drawNIM3D();
    } else {
        // Mode 2D
        glScalef(0.035f, 0.035f, 1.0f);
        glTranslatef(-1.0f, -1.0f, -0.75f);
        drawNIM2D();
    }
    glPopMatrix();

    // Player
    glPushMatrix();
    glTranslatef(posX, posY, 0.3f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    glutSwapBuffers();
}

// ================= KONTROL & ANIMASI =================
void input(unsigned char key, int x, int y) {
    float speed = 0.2f;
    float newX = posX, newY = posY;

    if (key == 'q' || key == 'Q') exit(0);
    else if (key == 'v' || key == 'V') is_isometric = !is_isometric;
    else if (key == 'c' || key == 'C') {
        rencent_maze = (rencent_maze == 1) ? 2 : 1;
        posX = -0.5f; posY = 4.5f;
        pickRandomNimPos();
    }
    else if (key == 'w' || key == 'W') newY += speed;
    else if (key == 's' || key == 'S') newY -= speed;
    else if (key == 'd' || key == 'D') newX += speed;
    else if (key == 'a' || key == 'A') newX -= speed;

    if (is_isometric) {
        if (key == 'j' || key == 'J') mazeRotZ += 5.0f;
        else if (key == 'l' || key == 'L') mazeRotZ -= 5.0f;
        else if (key == 'i' || key == 'I') mazeRotX += 5.0f;
        else if (key == 'k' || key == 'K') mazeRotX -= 5.0f;
    }

    if (!checkCollisionPlayer(newX, newY)) { posX = newX; posY = newY; }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (is_isometric && state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) nimRotAxis = 1;
        else if (button == GLUT_MIDDLE_BUTTON) nimRotAxis = 2;
        else if (button == GLUT_RIGHT_BUTTON) nimRotAxis = 3;
    }
}

void idle() {
    if (is_isometric && nimRotAxis != 0) {
        // Kecepatan putaran diperlambat (awalnya 1.5f menjadi 0.3f)
        nimAngle += 0.3f;
        if (nimAngle >= 360.0f) nimAngle -= 360.0f;
        glutPostRedisplay();
    }
}

// ================= SETUP WINDOW =================
void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)w / (float)h;
    if (w <= h) glOrtho(-8.0, 8.0, -8.0 / aspect, 8.0 / aspect, -30.0, 30.0);
    else glOrtho(-8.0 * aspect, 8.0 * aspect, -8.0, 8.0, -30.0, 30.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void myinit() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    srand((unsigned int)time(NULL));
    pickRandomNimPos();
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Maze 3D - NIM Update");

    myinit();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(input);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
