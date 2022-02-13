#include <chrono>
#include <iostream>
#include <simplecpp>
#include <thread>

int main() {
    // turtleSim("Turtle Simulator", 1000, 400);
    turtleSim("Turtle Simulator");

    forward(100);

    drawCircle({200, 200}, 40, COLOR("red"));

    spriteStatus();

    drawCircle({550, 550}, 100, COLOR("green"));
    drawText(400,500, "Text", COLOR("blue"));

    drawLine({561,338}, {431, 138}, COLOR("blue"));

    getClick();
    spriteStatus();
}
