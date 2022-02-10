#include <GL/freeglut.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <chrono>
#include <iostream>
#include <simplecpp>
#include <thread>

int main() {
    turtleSim("Turtle Simulator", 1000, 400);
    // turtleSim("Turtle Simulator", 600, 600);

    forward(100);

    drawCircle({200, 200}, 40, COLOR("red"));

    spriteStatus();
    glutCloseFunc([]() { std::cout << "Should close;" << std::endl; });

    cout << "Window width: " << glutGet(GLUT_WINDOW_WIDTH) << endl;
    cout << "Window height: " << glutGet(GLUT_WINDOW_HEIGHT) << endl;

    drawCircle({550, 550}, 100, COLOR("green"));
    drawText(400,500, "Text", COLOR("blue"));

    glFlush();

    glutReshapeFunc([](int w, int h){
        cout << "Reshaped to " << w << "x" << h << endl;

        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();
        gluOrtho2D(0, w, 0, h);

        glutPostRedisplay();
    });

    // glutDisplayFunc([](){ glClearColor(1.0, 1.0, 1.0, 1.0);
    // glClear(GL_COLOR_BUFFER_BIT); glutSwapBuffers(); });
    std::this_thread::sleep_for(std::chrono::seconds(2));
    glutDisplayFunc([](){});
    glutMainLoopEvent();

    // Yes, past events are also captured by each MainLoopEvent
    // glutMainLoopEvent();

    drawLine({561,338}, {431, 138}, COLOR("blue"));

    getClick();
    spriteStatus();
}
