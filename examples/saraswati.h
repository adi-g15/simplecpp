#include <simplecpp>

void saraswati() {
    int n = 5, steps = 40;
    double sep = 100;
    Turtle t[n];

    for (int i = 0; i < n; i++) {
        t[i].penUp();
        t[i].move(-200 + sep * i, -200);
        t[i].penDown();
    }

    for (int i = 0; i < n; i++)
        t[i].left(60);

    for (int s = 0; s < 20; s++) {
        beginFrame();
        for (int i = 0; i < n; i++) {
            t[i].forward(3);
            t[i].left(18);
        }
        endFrame();
    }

    for (int i = 0; i < n; i++)
        t[i].right(180);

    for (int j = 0; j < steps; j++) {
        beginFrame();
        for (int i = 0; i < n; i++) {
            t[i].forward((50 + (i + 1) * sep) / steps);
        }
        endFrame();
    }

    for (int i = 0; i < n; i++)
        t[i].left(120);

    for (int j = 0; j < steps; j++) {
        beginFrame();
        for (int i = 0; i < n; i++)
            t[i].forward(((n - i) * sep) / steps);
        endFrame();
    }

    for (int i = 0; i < n; i++)
        t[i].left(120);

    for (int j = 0; j < steps; j++) {
        beginFrame();
        for (int i = 0; i < n; i++)
            t[i].forward((i + 1.5) * sep / steps);
        endFrame();
    }

    for (int s = 0; s < 10; s++) {
        beginFrame();
        for (int i = 0; i < n; i++) {
            t[i].forward(10);
            t[i].right(10);
        }
        endFrame();
    }
}
