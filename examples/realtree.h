#include <simplecpp>

void rtree(Turtle &t, int levels, double length, double scale) {
    if (levels > 0) {
        t.forward(length);
        Turtle l = t;
        l.left(30);
        rtree(l, levels - 1, length * scale, scale);
        t.forward(length * scale);
        //    Turtle     r = t;
        t.right(30);
        rtree(t, levels - 2, length * scale, scale);
    } else {
        t.scale(0.5);
        t.imprint();
    }
}

void realtree() {
    Turtle t;
    t.left(90);
    t.penUp();
    t.forward(-200);
    t.penDown();
    rtree(t, 10, 50, 0.8);
}
