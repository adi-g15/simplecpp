#include "polygon.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <simplecpp>

extern int window_width;
extern int window_height;

namespace simplecpp {
Turtle::Turtle(Composite *owner) {
    double pts_body[4][2] = {{0, 15}, {20, 0}, {0, -15}, {0, 15}};
    setColor(COLOR("red"), false);
    setFill();
    penDown();
    reset(window_width / 2.0f, window_height / 2.0f, pts_body, 4, owner);
}
} // namespace simplecpp
