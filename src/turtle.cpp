#include "canvas.h"
#include "polygon.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <simplecpp>

namespace simplecpp {
Turtle::Turtle(Composite *owner) {
    double pts_body[3][2] = {{0, 15}, {20, 0}, {0, -15}};
    setColor(COLOR("red"), false);
    setFill();
    penDown();
    reset(canvas_width() / 2.0f, canvas_height() / 2.0f, pts_body, 3, owner);
}
} // namespace simplecpp
