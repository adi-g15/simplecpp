#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <chrono>
#include <iostream>
#include <simplecpp>
#include <thread>

int main() {
    spriteStatus();
    turtleSim("adi", 800, 600);
    spriteStatus();

    forward(100);

    drawCircle({200, 200}, 40, COLOR("red"));

    spriteStatus();
    glutCloseFunc([]() { std::cout << "Should close;" << std::endl; });

    // glutDisplayFunc([](){});
    // glutMainLoopEvent();

    // Yes, past events are also captured by each MainLoopEvent
    std::this_thread::sleep_for(std::chrono::seconds(4));
    // glutMainLoopEvent();

    getClick();
    spriteStatus();
}
