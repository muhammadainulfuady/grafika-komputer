#include <windows.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <time.h>

float posY = 4.5f;  // start tengah atas
float posX = -0.5f; // start tengah atas
bool switch_maze = true;
bool press_c = true;
int rencent_maze = 1;


// posisi NIM acak di area putih dalam maze
// nimOx, nimOy adalah offset yang menggantikan posX,posY pada blok NIM asli
// NIM asli render di: x=[nimOx-0.885 .. nimOx-0.115], y=[nimOy+4.325 .. nimOy+4.675]
// Untuk muncul di world center (cx,cy): nimOx = cx+0.5, nimOy = cy-4.5
// NIM lebar ~0.77, tinggi ~0.35 -- semua entry di bawah sudah diverifikasi FIT

// Area putih maze 1 (8 titik, terverifikasi brute-force tidak menabrak dinding)
// NIM bbox: x[nimOx-0.885, nimOx-0.115]  y[nimOy+4.325, nimOy+4.675]
float nimSafeMaze1[8][2] = {
    { -4.0f,  -5.0f },  // A: NIM world x[-4.885,-4.115] y[-0.675,-0.325] -- OK
    { -4.0f,  -7.0f },  // B: NIM world x[-4.885,-4.115] y[-2.675,-2.325] -- OK
    { -4.0f,  -9.0f },  // C: NIM world x[-4.885,-4.115] y[-4.675,-4.325] -- OK
    { -2.0f,  -3.0f },  // D: NIM world x[-2.885,-2.115] y[1.325,1.675]   -- OK
    { -2.0f,  -9.0f },  // E: NIM world x[-2.885,-2.115] y[-4.675,-4.325] -- OK
    {  2.0f,  -1.0f },  // F: NIM world x[1.115,1.885]   y[3.325,3.675]   -- OK
    {  2.0f,  -7.0f },  // G: NIM world x[1.115,1.885]   y[-2.675,-2.325] -- OK
    {  4.0f,  -3.5f },  // H: NIM world x[3.115,3.885]   y[0.825,1.175]   -- OK
};

// Area putih maze 2 (8 titik, terverifikasi brute-force tidak menabrak dinding)
float nimSafeMaze2[8][2] = {
    { -4.0f,  -2.0f },  // A: NIM world x[-4.885,-4.115] y[2.325,2.675]   -- OK
    { -4.0f,  -5.0f },  // B: NIM world x[-4.885,-4.115] y[-0.675,-0.325] -- OK
    { -4.0f,  -9.0f },  // C: NIM world x[-4.885,-4.115] y[-4.675,-4.325] -- OK
    { -2.0f,  -9.0f },  // D: NIM world x[-2.885,-2.115] y[-4.675,-4.325] -- OK
    {  2.0f,  -3.5f },  // E: NIM world x[1.115,1.885]   y[0.825,1.175]   -- OK
    {  2.0f,  -5.0f },  // F: NIM world x[1.115,1.885]   y[-0.675,-0.325] -- OK (ganti dari -6.0 yg nabrak)
    {  2.0f,  -9.0f },  // G: NIM world x[1.115,1.885]   y[-4.675,-4.325] -- OK
    {  2.0f,  -8.0f },  // H: NIM world x[1.115,1.885]   y[-3.675,-3.325] -- OK
};

float nimOx = -4.0f;
float nimOy = -5.0f;

void pickRandomNimPos() {
    int idx = rand() % 8; // FIX: gunakan semua 8 posisi (asli hanya % 5)
    if (rencent_maze == 1) {
        nimOx = nimSafeMaze1[idx][0];
        nimOy = nimSafeMaze1[idx][1];
    } else {
        nimOx = nimSafeMaze2[idx][0];
        nimOy = nimSafeMaze2[idx][1];
    }
}

// ================= FUNGSI RESHAPE  =================
void reshape(int w, int h) {
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)w / (float)h;

    if (w <= h) {
        gluOrtho2D(-10.0, 10.0, -10.0 / aspect, 10.0 / aspect);
    } else {
        gluOrtho2D(-10.0 * aspect, 10.0 * aspect, -10.0, 10.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ================= GRID =================
void drawGrid() {
    glColor3f(0.0, 0.0, 0.0); // hitam
    glLineWidth(2.0);

    float size = 12.0;
    int n = 12;

    glBegin(GL_LINES);

    //Horizontal
    for (int i = 0; i <= n; i++) {
        float y = -size + i * (2 * size / n);
        glVertex2f(-size, y);
        glVertex2f(size, y);
    }

    // Vertical
    for (int i = 0; i <= n; i++) {
        float x = -size + i * (2 * size / n);
        glVertex2f(x, -size);
        glVertex2f(x, size);
    }

    glEnd();
}

void maze1(){

    //MAZE DALAM 2
    glBegin(GL_POLYGON);
        glVertex2f(-1.0, -3.0);
        glVertex2f(-2.0, -3.0);
        glVertex2f(-2.0, -1.0);
        glVertex2f(-1.0, -1.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.0, -3.0);
        glVertex2f(-2.0, -3.0);
        glVertex2f(-2.0, -4.0);
        glVertex2f(0.0, -4.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(2.0, -3.0);
        glVertex2f(4.0, -3.0);
        glVertex2f(4.0, -4.0);
        glVertex2f(2.0, -4.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(2.0, -2.0);
        glVertex2f(3.0, -2.0);
        glVertex2f(3.0, 3.0);
        glVertex2f(2.0, 3.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.0, -2.0);
        glVertex2f(3.0, -2.0);
        glVertex2f(3.0, -1.0);
        glVertex2f(0.0, -1.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.0, -5.0);
        glVertex2f(1.0, -5.0);
        glVertex2f(1.0, -1.0);
        glVertex2f(0.0, -1.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.0, 3.0);
        glVertex2f(-2.0, 3.0);
        glVertex2f(-2.0, 2.0);
        glVertex2f(0.0, 2.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.0, 3.0);
        glVertex2f(1.0, 3.0);
        glVertex2f(1.0, 0.0);
        glVertex2f(0.0, 0.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-4.0, 0.0);
        glVertex2f(-4.0, 1.0);
        glVertex2f(1.0, 1.0);
        glVertex2f(1.0, 0.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-4.0, -5.0);
        glVertex2f(-4.0, 1.0);
        glVertex2f(-3.0, 1.0);
        glVertex2f(-3.0, -5.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-5.0, 3.0);
        glVertex2f(-5.0, 2.0);
        glVertex2f(-3.0, 2.0);
        glVertex2f(-3.0, 3.0);
    glEnd();
}

void maze2(){
    // Maze 2

    //MAZE DALAM
    glBegin(GL_POLYGON);
        glVertex2f(4.0, 1.0);
        glVertex2f(3.0, 1.0);
        glVertex2f(3.0, 0.0);
        glVertex2f(4.0, 0.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(3.0, 2.0);
        glVertex2f(2.0, 2.0);
        glVertex2f(2.0, 0.0);
        glVertex2f(3.0, 0.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.0, 3.0);
        glVertex2f(0.0, 2.0);
        glVertex2f(3.0, 2.0);
        glVertex2f(3.0, 3.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-2.0, -3.0);
        glVertex2f(-4.0, -3.0);
        glVertex2f(-4.0, -4.0);
        glVertex2f(-2.0, -4.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-1.0, -3.0);
        glVertex2f(-2.0, -3.0);
        glVertex2f(-2.0, -5.0);
        glVertex2f(-1.0, -5.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(4.0, -4.0);
        glVertex2f(4.0, -3.0);
        glVertex2f(2.0, -3.0);
        glVertex2f(2.0, -4.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.0, 1.0);
        glVertex2f(1.0, 1.0);
        glVertex2f(1.0, -5.0);
        glVertex2f(0.0, -5.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-3.0, -1.0);
        glVertex2f(-3.0, -2.0);
        glVertex2f(3.0, -2.0);
        glVertex2f(3.0, -1.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-4.0, 2.0);
        glVertex2f(-3.0, 2.0);
        glVertex2f(-3.0, -2.0);
        glVertex2f(-4.0, -2.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-2.0, 3.0);
        glVertex2f(-2.0, 2.0);
        glVertex2f(-4.0, 2.0);
        glVertex2f(-4.0, 3.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-1.0, 3.0);
        glVertex2f(-1.0, 0.0);
        glVertex2f(-2.0, 0.0);
        glVertex2f(-2.0, 3.0);
    glEnd();
}

// ================= DISPLAY =================
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity(); // Memastikan posisi reset tiap gambar ulang

    // ===== RENDER DINDING LUAR (biru) dulu =====
    glColor3f(0.0, 0.0, 1.0);

    glBegin(GL_POLYGON);
        glVertex2f(0.0, 5.0);
        glVertex2f(0.0, 4.0);
        glVertex2f(5.0, 4.0);
        glVertex2f(5.0, 5.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(5.0, 5.0);
        glVertex2f(5.0, -5.0);
        glVertex2f(4.0, -5.0);
        glVertex2f(4.0, 5.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.0, -6.0);
        glVertex2f(0.0, -5.0);
        glVertex2f(5.0, -5.0);
        glVertex2f(5.0, -6.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-1.0, -6.0);
        glVertex2f(-1.0, -5.0);
        glVertex2f(-6.0, -5.0);
        glVertex2f(-6.0, -6.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-6.0, -5.0);
        glVertex2f(-6.0, 5.0);
        glVertex2f(-5.0, 5.0);
        glVertex2f(-5.0, -5.0);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-1.0, 5.0);
        glVertex2f(-1.0, 4.0);
        glVertex2f(-6.0, 4.0);
        glVertex2f(-6.0, 5.0);
    glEnd();

    // ===== RENDER MAZE DALAM (biru) =====
    glColor3f(0.0, 0.0, 1.0); // FIX: dinding dalam harus biru, bukan dari warna sebelumnya
    if(rencent_maze == 1){
        maze1();
    }else if(rencent_maze == 2){
        maze2();
    }

    // ===== NIM STATIS - DIRENDER SETELAH DINDING agar tidak tertimpa =====
    // FIX: dipindah ke sini dari posisi sebelum dinding di kode asli
    glColor3f(0.0, 1.0, 0.0);
    glLineWidth(4);

    // Angka pertama
    glBegin(GL_POLYGON);
        glVertex2f(-0.885f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.675f + nimOy);
        glVertex2f(-0.885f + nimOx,  4.675f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.815f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.745f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.745f + nimOx,  4.675f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.675f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.745f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.675f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.675f + nimOx,  4.675f + nimOy);
        glVertex2f(-0.745f + nimOx,  4.675f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.745f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.675f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.675f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.745f + nimOx,  4.605f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.745f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.675f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.675f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.745f + nimOx,  4.535f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.745f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.675f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.675f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.745f + nimOx,  4.465f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.745f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.675f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.675f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.745f + nimOx,  4.395f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.815f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.745f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.745f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.395f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.885f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.885f + nimOx,  4.395f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.885f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.885f + nimOx,  4.465f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.885f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.885f + nimOx,  4.535f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.885f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.815f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.885f + nimOx,  4.605f + nimOy);
    glEnd();

    // Angka kedua
    glBegin(GL_POLYGON);
        glVertex2f(-0.605f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.675f + nimOy);
        glVertex2f(-0.605f + nimOx,  4.675f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.535f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.465f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.465f + nimOx,  4.675f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.675f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.465f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.395f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.395f + nimOx,  4.675f + nimOy);
        glVertex2f(-0.465f + nimOx,  4.675f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.465f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.395f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.395f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.465f + nimOx,  4.605f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.465f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.395f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.395f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.465f + nimOx,  4.535f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.465f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.395f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.395f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.465f + nimOx,  4.465f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.465f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.395f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.395f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.465f + nimOx,  4.395f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.535f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.465f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.465f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.395f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.605f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.605f + nimOx,  4.395f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.605f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.605f + nimOx,  4.465f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.605f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.605f + nimOx,  4.535f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.605f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.535f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.605f + nimOx,  4.605f + nimOy);
    glEnd();

    // Angka ketiga
    glBegin(GL_POLYGON);
        glVertex2f(-0.255f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.185f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.185f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.255f + nimOx,  4.535f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.325f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.255f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.255f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.325f + nimOx,  4.535f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.325f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.255f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.255f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.325f + nimOx,  4.605f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.325f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.255f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.255f + nimOx,  4.675f + nimOy);
        glVertex2f(-0.325f + nimOx,  4.675f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.255f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.185f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.185f + nimOx,  4.675f + nimOy);
        glVertex2f(-0.255f + nimOx,  4.675f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.185f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.115f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.115f + nimOx,  4.675f + nimOy);
        glVertex2f(-0.185f + nimOx,  4.675f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.185f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.115f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.115f + nimOx,  4.605f + nimOy);
        glVertex2f(-0.185f + nimOx,  4.605f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.185f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.115f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.115f + nimOx,  4.535f + nimOy);
        glVertex2f(-0.185f + nimOx,  4.535f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.185f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.115f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.115f + nimOx,  4.465f + nimOy);
        glVertex2f(-0.185f + nimOx,  4.465f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.185f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.115f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.115f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.185f + nimOx,  4.395f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.255f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.185f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.185f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.255f + nimOx,  4.395f + nimOy);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.325f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.255f + nimOx,  4.325f + nimOy);
        glVertex2f(-0.255f + nimOx,  4.395f + nimOy);
        glVertex2f(-0.325f + nimOx,  4.395f + nimOy);
    glEnd();

    // ===== PLAYER (kotak hijau) - dirender paling akhir agar selalu di atas =====
    float s = 0.45f;
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(posX - s, posY - s);
        glVertex2f(posX + s, posY - s);
        glVertex2f(posX + s, posY + s);
        glVertex2f(posX - s, posY + s);
    glEnd();

    glFlush();
}

void input(unsigned char key, int x, int y){
    if(key == 'q') {exit(0);}
    if(key == 'w') {
        posY += 0.2;
    }
    if(key == 's') {
        posY -= 0.2;
    }
    else if(key == 'd') {
        posX += 0.2;

    }
    else if(key == 'a') {
        posX -= 0.2;

    }else if (key == 'c'){
        if(rencent_maze == 1){
            rencent_maze = 2;
        }else{
            rencent_maze = 1;
        }
        // Reset posisi player ke tengah atas
        posX = -0.5f;
        posY =  4.5f;
        // ===== TAMBAHAN: pilih posisi NIM acak di dalam maze baru =====
        pickRandomNimPos();
    }
    display();

}


// ================= INIT =================
void myinit(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    srand((unsigned int)time(NULL));
    pickRandomNimPos(); // ===== TAMBAHAN: posisi NIM acak saat startup =====
}

// ================= MAIN =================
int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Grid Puzzle - Anti Gepeng");

    myinit();

    glutDisplayFunc(display);
    // Daftarkan fungsi reshape ke GLUT
    glutReshapeFunc(reshape);
    glutKeyboardFunc(input);

    glutMainLoop();

    return 0;
}
