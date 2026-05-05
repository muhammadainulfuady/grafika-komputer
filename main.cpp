#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


void drawBox(float x1, float y1, float z1,
             float x2, float y2, float z2) {

    glBegin(GL_QUADS);

        // Sisi depan (front face) - z = z2
        glVertex3f(x1, y1, z2);
        glVertex3f(x2, y1, z2);
        glVertex3f(x2, y2, z2);
        glVertex3f(x1, y2, z2);

        // Sisi belakang (back face) - z = z1
        glVertex3f(x2, y1, z1);
        glVertex3f(x1, y1, z1);
        glVertex3f(x1, y2, z1);
        glVertex3f(x2, y2, z1);

        // Sisi kiri (left face) - x = x1
        glVertex3f(x1, y1, z1);
        glVertex3f(x1, y1, z2);
        glVertex3f(x1, y2, z2);
        glVertex3f(x1, y2, z1);

        // Sisi kanan (right face) - x = x2
        glVertex3f(x2, y1, z2);
        glVertex3f(x2, y1, z1);
        glVertex3f(x2, y2, z1);
        glVertex3f(x2, y2, z2);

        // Sisi bawah (bottom face) - y = y1
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y1, z1);
        glVertex3f(x2, y1, z2);
        glVertex3f(x1, y1, z2);

        // Sisi atas (top face) - y = y2
        glVertex3f(x1, y2, z2);
        glVertex3f(x2, y2, z2);
        glVertex3f(x2, y2, z1);
        glVertex3f(x1, y2, z1);

    glEnd();
}

// ================= DISPLAY =================
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float depth = 1.5f;

    // Angka Pertama
    glColor3f(0.0f, 0.0f, 1.0f);

    // Segmen atas
    drawBox(-10.0f, 4.0f, 0.0f,  -8.0f, 6.0f, depth);
    drawBox( -8.0f, 4.0f, 0.0f,  -6.0f, 6.0f, depth);
    drawBox( -6.0f, 4.0f, 0.0f,  -4.0f, 6.0f, depth);

    // Segmen kanan atas
    drawBox( -6.0f, 2.0f, 0.0f,  -4.0f, 4.0f, depth);
    drawBox( -6.0f, 0.0f, 0.0f,  -4.0f, 2.0f, depth);

    // Segmen kanan bawah
    drawBox( -6.0f,-2.0f, 0.0f,  -4.0f, 0.0f, depth);
    drawBox( -6.0f,-4.0f, 0.0f,  -4.0f,-2.0f, depth);

    // Segmen bawah
    drawBox( -8.0f,-4.0f, 0.0f,  -6.0f,-2.0f, depth);
    drawBox(-10.0f,-4.0f, 0.0f,  -8.0f,-2.0f, depth);

    // Segmen kiri bawah
    drawBox(-10.0f,-2.0f, 0.0f,  -8.0f, 0.0f, depth);
    drawBox(-10.0f, 0.0f, 0.0f,  -8.0f, 2.0f, depth);

    // Segmen kiri atas
    drawBox(-10.0f, 2.0f, 0.0f,  -8.0f, 4.0f, depth);

    // Angka Kedua
    glColor3f(0.0f, 0.0f, 1.0f);

    // Segmen atas
    drawBox(-2.0f, 4.0f, 0.0f,  0.0f, 6.0f, depth);
    drawBox( 0.0f, 4.0f, 0.0f,  2.0f, 6.0f, depth);
    drawBox( 2.0f, 4.0f, 0.0f,  4.0f, 6.0f, depth);

    // Segmen kanan atas
    drawBox( 2.0f, 2.0f, 0.0f,  4.0f, 4.0f, depth);
    drawBox( 2.0f, 0.0f, 0.0f,  4.0f, 2.0f, depth);

    // Segmen kanan bawah
    drawBox( 2.0f,-2.0f, 0.0f,  4.0f, 0.0f, depth);
    drawBox( 2.0f,-4.0f, 0.0f,  4.0f,-2.0f, depth);

    // Segmen bawah
    drawBox( 0.0f,-4.0f, 0.0f,  2.0f,-2.0f, depth);
    drawBox(-2.0f,-4.0f, 0.0f,  0.0f,-2.0f, depth);

    // Segmen kiri bawah
    drawBox(-2.0f,-2.0f, 0.0f,  0.0f, 0.0f, depth);
    drawBox(-2.0f, 0.0f, 0.0f,  0.0f, 2.0f, depth);

    // Segmen kiri atas
    drawBox(-2.0f, 2.0f, 0.0f,  0.0f, 4.0f, depth);

    //Angka Ketiga
    glColor3f(0.0f, 0.0f, 1.0f);

    // Segmen tengah
    drawBox( 6.0f, 0.0f, 0.0f,  8.0f, 2.0f, depth);
    drawBox( 8.0f, 0.0f, 0.0f, 10.0f, 2.0f, depth);

    // Segmen kiri atas
    drawBox( 6.0f, 2.0f, 0.0f,  8.0f, 4.0f, depth);
    drawBox( 6.0f, 4.0f, 0.0f,  8.0f, 6.0f, depth);

    // Segmen atas
    drawBox( 8.0f, 4.0f, 0.0f, 10.0f, 6.0f, depth);
    drawBox(10.0f, 4.0f, 0.0f, 12.0f, 6.0f, depth);

    // Segmen kanan atas
    drawBox(10.0f, 2.0f, 0.0f, 12.0f, 4.0f, depth);
    drawBox(10.0f, 0.0f, 0.0f, 12.0f, 2.0f, depth);

    // Segmen kanan bawah
    drawBox(10.0f,-2.0f, 0.0f, 12.0f, 0.0f, depth);
    drawBox(10.0f,-4.0f, 0.0f, 12.0f,-2.0f, depth);

    // Segmen bawah
    drawBox( 8.0f,-4.0f, 0.0f, 10.0f,-2.0f, depth);
    drawBox( 6.0f,-4.0f, 0.0f,  8.0f,-2.0f, depth);

    glFlush();
}

void myinit() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-14.0, 14.0, -14.0, 14.0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glRotatef(20.0f, 1.0f, 0.0f, 0.0f); // rotasi sumbu X
    //glRotatef(-15.0f, 0.0f, 1.0f, 0.0f); // rotasi sumbu Y

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_FLAT);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 500);

    glutCreateWindow("NIM 3D");

    glutDisplayFunc(display);
    myinit();
    glutMainLoop();

    return 0;
}
