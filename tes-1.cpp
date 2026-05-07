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

// Mode View
bool is_isometric = false;

// Rotasi Maze Isometric
float mazeRotX = -60.0f;
float mazeRotZ = 45.0f;

// Animasi NIM
int nimRotAxis = 3; // DEFAULT 3 (Sumbu Z) agar langsung mutar otomatis saat 3D
float nimAngle = 0.0f;

// ================= STRUKTUR DATA MAZE =================
struct Rect { float minX, minY, maxX, maxY; };

Rect mazeOuter[6] = {
    { 0.0,  4.0,  5.0,  5.0 }, { 4.0, -5.0,  5.0,  5.0 },
    { 0.0, -6.0,  5.0, -5.0 }, {-6.0, -6.0, -1.0, -5.0 },
    {-6.0, -5.0, -5.0,  5.0 }, {-6.0,  4.0, -1.0,  5.0 }
};

Rect maze1_walls[11] = {
    {-2.0, -3.0, -1.0, -1.0}, {-2.0, -4.0,  0.0, -3.0},
    { 2.0, -4.0,  4.0, -3.0}, { 2.0, -2.0,  3.0,  3.0},
    { 0.0, -2.0,  3.0, -1.0}, { 0.0, -5.0,  1.0, -1.0},
    {-2.0,  2.0,  0.0,  3.0}, { 0.0,  0.0,  1.0,  3.0},
    {-4.0,  0.0,  1.0,  1.0}, {-4.0, -5.0, -3.0,  1.0},
    {-5.0,  2.0, -3.0,  3.0}
};

Rect maze2_walls[11] = {
    { 3.0,  0.0,  4.0,  1.0}, { 2.0,  0.0,  3.0,  2.0},
    { 0.0,  2.0,  3.0,  3.0}, {-4.0, -4.0, -2.0, -3.0},
    {-2.0, -5.0, -1.0, -3.0}, { 2.0, -4.0,  4.0, -3.0},
    { 0.0, -5.0,  1.0,  1.0}, {-3.0, -2.0,  3.0, -1.0},
    {-4.0, -2.0, -3.0,  2.0}, {-4.0,  2.0, -2.0,  3.0},
    {-2.0,  0.0, -1.0,  3.0}
};

float nimOx = -4.0f, nimOy = -5.0f;

// ================= LOGIKA TABRAKAN & POSISI ACAK =================
bool isOverlap(float minX1, float minY1, float maxX1, float maxY1, Rect r) {
    return !(maxX1 <= r.minX || minX1 >= r.maxX || maxY1 <= r.minY || minY1 >= r.maxY);
}

// Fungsi deteksi pintar: mengecek apakah sebuah kotak 1x1 bebas dari tembok
bool isCellEmpty(float cx, float cy) {
    // Beri sedikit margin agar tidak error pada garis batas
    float cMinX = cx + 0.1f, cMaxX = cx + 0.9f;
    float cMinY = cy + 0.1f, cMaxY = cy + 0.9f;

    // Cek tembok pinggir luar
    for (int i = 0; i < 6; i++) {
        if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, mazeOuter[i])) return false;
    }

    // Cek tembok lorong di dalam
    if (rencent_maze == 1) {
        for (int i = 0; i < 11; i++) if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, maze1_walls[i])) return false;
    } else {
        for (int i = 0; i < 11; i++) if (isOverlap(cMinX, cMinY, cMaxX, cMaxY, maze2_walls[i])) return false;
    }

    // Mencegah NIM diletakkan persis di titik awal player (pintu masuk)
    if (cx == -1.0f && cy == 4.0f) return false;
    if (cx ==  0.0f && cy == 4.0f) return false;

    return true; // Kotak aman dan kosong!
}

void pickRandomNimPos() {
    bool posisiKetemu = false;
    // Terus mencari secara acak sampai menemukan sel lorong yang benar-benar kosong
    while (!posisiKetemu) {
        // Area dalam maze ada pada koordinat X: -5 s/d 4 dan Y: -5 s/d 4
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

    for (int i = 0; i < 6; i++) if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, mazeOuter[i])) return true;

    if (rencent_maze == 1) {
        for (int i = 0; i < 11; i++) if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, maze1_walls[i])) return true;
    } else {
        for (int i = 0; i < 11; i++) if (isOverlap(pMinX, pMinY, pMaxX, pMaxY, maze2_walls[i])) return true;
    }
    return false;
}

// ================= KODE NIM DOSEN / KELOMPOK (MURNI TANPA UBAHAN) =================
void drawBox(float x1, float y1, float z1, float x2, float y2, float z2) {
    glBegin(GL_QUADS);
        // Sisi depan (front face) - z = z2
        glVertex3f(x1, y1, z2); glVertex3f(x2, y1, z2); glVertex3f(x2, y2, z2); glVertex3f(x1, y2, z2);
        // Sisi belakang (back face) - z = z1
        glVertex3f(x2, y1, z1); glVertex3f(x1, y1, z1); glVertex3f(x1, y2, z1); glVertex3f(x2, y2, z1);
        // Sisi kiri (left face) - x = x1
        glVertex3f(x1, y1, z1); glVertex3f(x1, y1, z2); glVertex3f(x1, y2, z2); glVertex3f(x1, y2, z1);
        // Sisi kanan (right face) - x = x2
        glVertex3f(x2, y1, z2); glVertex3f(x2, y1, z1); glVertex3f(x2, y2, z1); glVertex3f(x2, y2, z2);
        // Sisi bawah (bottom face) - y = y1
        glVertex3f(x1, y1, z1); glVertex3f(x2, y1, z1); glVertex3f(x2, y1, z2); glVertex3f(x1, y1, z2);
        // Sisi atas (top face) - y = y2
        glVertex3f(x1, y2, z2); glVertex3f(x2, y2, z2); glVertex3f(x2, y2, z1); glVertex3f(x1, y2, z1);
    glEnd();
}

void drawNIM3D() {
    float depth = 1.5f;

    // Angka Pertama
    glColor3f(0.0f, 0.0f, 1.0f);
    drawBox(-10.0f, 4.0f, 0.0f,  -8.0f, 6.0f, depth); drawBox( -8.0f, 4.0f, 0.0f,  -6.0f, 6.0f, depth);
    drawBox( -6.0f, 4.0f, 0.0f,  -4.0f, 6.0f, depth); drawBox( -6.0f, 2.0f, 0.0f,  -4.0f, 4.0f, depth);
    drawBox( -6.0f, 0.0f, 0.0f,  -4.0f, 2.0f, depth); drawBox( -6.0f,-2.0f, 0.0f,  -4.0f, 0.0f, depth);
    drawBox( -6.0f,-4.0f, 0.0f,  -4.0f,-2.0f, depth); drawBox( -8.0f,-4.0f, 0.0f,  -6.0f,-2.0f, depth);
    drawBox(-10.0f,-4.0f, 0.0f,  -8.0f,-2.0f, depth); drawBox(-10.0f,-2.0f, 0.0f,  -8.0f, 0.0f, depth);
    drawBox(-10.0f, 0.0f, 0.0f,  -8.0f, 2.0f, depth); drawBox(-10.0f, 2.0f, 0.0f,  -8.0f, 4.0f, depth);

    // Angka Kedua
    glColor3f(0.0f, 0.0f, 1.0f);
    drawBox(-2.0f, 4.0f, 0.0f,  0.0f, 6.0f, depth); drawBox( 0.0f, 4.0f, 0.0f,  2.0f, 6.0f, depth);
    drawBox( 2.0f, 4.0f, 0.0f,  4.0f, 6.0f, depth); drawBox( 2.0f, 2.0f, 0.0f,  4.0f, 4.0f, depth);
    drawBox( 2.0f, 0.0f, 0.0f,  4.0f, 2.0f, depth); drawBox( 2.0f,-2.0f, 0.0f,  4.0f, 0.0f, depth);
    drawBox( 2.0f,-4.0f, 0.0f,  4.0f,-2.0f, depth); drawBox( 0.0f,-4.0f, 0.0f,  2.0f,-2.0f, depth);
    drawBox(-2.0f,-4.0f, 0.0f,  0.0f,-2.0f, depth); drawBox(-2.0f,-2.0f, 0.0f,  0.0f, 0.0f, depth);
    drawBox(-2.0f, 0.0f, 0.0f,  0.0f, 2.0f, depth); drawBox(-2.0f, 2.0f, 0.0f,  0.0f, 4.0f, depth);

    // Angka Ketiga
    glColor3f(0.0f, 0.0f, 1.0f);
    drawBox( 6.0f, 0.0f, 0.0f,  8.0f, 2.0f, depth); drawBox( 8.0f, 0.0f, 0.0f, 10.0f, 2.0f, depth);
    drawBox( 6.0f, 2.0f, 0.0f,  8.0f, 4.0f, depth); drawBox( 6.0f, 4.0f, 0.0f,  8.0f, 6.0f, depth);
    drawBox( 8.0f, 4.0f, 0.0f, 10.0f, 6.0f, depth); drawBox(10.0f, 4.0f, 0.0f, 12.0f, 6.0f, depth);
    drawBox(10.0f, 2.0f, 0.0f, 12.0f, 4.0f, depth); drawBox(10.0f, 0.0f, 0.0f, 12.0f, 2.0f, depth);
    drawBox(10.0f,-2.0f, 0.0f, 12.0f, 0.0f, depth); drawBox(10.0f,-4.0f, 0.0f, 12.0f,-2.0f, depth);
    drawBox( 8.0f,-4.0f, 0.0f, 10.0f,-2.0f, depth); drawBox( 6.0f,-4.0f, 0.0f,  8.0f,-2.0f, depth);
}

// ================= RENDER DINDING MAZE =================
void drawMazeWall(float x1, float y1, float z1, float x2, float y2, float z2) {
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.4f, 1.0f); // Atas Biru
    glVertex3f(x1, y1, z2); glVertex3f(x2, y1, z2); glVertex3f(x2, y2, z2); glVertex3f(x1, y2, z2);
    glColor3f(0.1f, 0.2f, 0.5f); // Bawah
    glVertex3f(x1, y1, z1); glVertex3f(x1, y2, z1); glVertex3f(x2, y2, z1); glVertex3f(x2, y1, z1);

    glColor3f(1.0f, 0.2f, 0.2f); // Samping Merah
    glVertex3f(x1, y1, z1); glVertex3f(x2, y1, z1); glVertex3f(x2, y1, z2); glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z1); glVertex3f(x1, y2, z2); glVertex3f(x2, y2, z2); glVertex3f(x2, y2, z1);
    glVertex3f(x1, y1, z1); glVertex3f(x1, y1, z2); glVertex3f(x1, y2, z2); glVertex3f(x1, y2, z1);
    glVertex3f(x2, y1, z1); glVertex3f(x2, y2, z1); glVertex3f(x2, y2, z2); glVertex3f(x2, y1, z2);
    glEnd();
}

// ================= RENDER TAMPILAN =================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Mode View
    if (is_isometric) {
        glRotatef(mazeRotX, 1.0f, 0.0f, 0.0f);
        glRotatef(mazeRotZ, 0.0f, 0.0f, 1.0f);
    }

    // 1. Gambar Dinding
    for (int i = 0; i < 6; i++) drawMazeWall(mazeOuter[i].minX, mazeOuter[i].minY, 0.0f, mazeOuter[i].maxX, mazeOuter[i].maxY, 1.0f);

    if (rencent_maze == 1) {
        for (int i = 0; i < 11; i++) drawMazeWall(maze1_walls[i].minX, maze1_walls[i].minY, 0.0f, maze1_walls[i].maxX, maze1_walls[i].maxY, 1.0f);
    } else {
        for (int i = 0; i < 11; i++) drawMazeWall(maze2_walls[i].minX, maze2_walls[i].minY, 0.0f, maze2_walls[i].maxX, maze2_walls[i].maxY, 1.0f);
    }

    // 2. Gambar NIM 3D
    glPushMatrix();
    glTranslatef(nimOx + 0.5f, nimOy + 0.5f, 0.5f);
    if (is_isometric && nimRotAxis != 0) {
        // NIM AKAN LANGSUNG BERPUTAR OTOMATIS SAAT MODE 3D
        if (nimRotAxis == 1) glRotatef(nimAngle, 1, 0, 0);
        else if (nimRotAxis == 2) glRotatef(nimAngle, 0, 1, 0);
        else if (nimRotAxis == 3) glRotatef(nimAngle, 0, 0, 1);
    }
    glScalef(0.035f, 0.035f, 0.035f);
    glTranslatef(-1.0f, -1.0f, -0.75f);
    drawNIM3D();
    glPopMatrix();

    // 3. Gambar Player (Bola Hitam)
    glPushMatrix();
    glTranslatef(posX, posY, 0.3f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    glutSwapBuffers();
}

// ================= CONTROLLER (Keyboard & Mouse) =================
void input(unsigned char key, int x, int y) {
    float speed = 0.2f;
    float newX = posX;
    float newY = posY;

    if (key == 'q' || key == 'Q') exit(0);
    else if (key == 'v' || key == 'V') is_isometric = !is_isometric;
    else if (key == 'c' || key == 'C') {
        rencent_maze = (rencent_maze == 1) ? 2 : 1;
        posX = -0.5f; posY = 4.5f;
        pickRandomNimPos(); // <--- Tiap ganti maze, posisi ditaruh di tempat kosong yang baru
    }

    // Gerakan (WASD)
    else if (key == 'w' || key == 'W') newY += speed;
    else if (key == 's' || key == 'S') newY -= speed;
    else if (key == 'd' || key == 'D') newX += speed;
    else if (key == 'a' || key == 'A') newX -= speed;

    // Rotasi Manual Maze (Hanya di mode Isometrik)
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
        // Kontrol mouse dipertahankan (sesuai rubrik dosen) jika ingin ganti gaya putar
        if (button == GLUT_LEFT_BUTTON) nimRotAxis = 1;       // Putar Sumbu X
        else if (button == GLUT_MIDDLE_BUTTON) nimRotAxis = 2; // Putar Sumbu Y
        else if (button == GLUT_RIGHT_BUTTON) nimRotAxis = 3;  // Putar Sumbu Z
    }
}

void idle() {
    if (is_isometric && nimRotAxis != 0) {
        nimAngle += 1.5f;
        if (nimAngle >= 360.0f) nimAngle -= 360.0f;
        glutPostRedisplay();
    }
}

// ================= SETUP WINDOW & PROYEKSI =================
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
