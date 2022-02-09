#include "polygon.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <simplecpp>

extern int screen_width;
extern int screen_height;

namespace simplecpp {
Turtle::Turtle(Composite *owner) {
    double pts_body[4][2] = {{0, 15}, {20, 0}, {0, -15}, {0, 15}};
    glColor3f(1.0f, 0.0f, 0.0f);
    // @remove
    setColor(COLOR("red"), false);
    setColor({1,0,0} /*red*/, false);

    setFill();
    penDown();
    reset(screen_width / 2.0f, screen_height / 2.0f, pts_body, 4, owner);
}
} // namespace simplecpp
