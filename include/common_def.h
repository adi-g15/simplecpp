#ifndef _COMMON_DEF_INCLUDED_
#define _COMMON_DEF_INCLUDED_

#include <GL/freeglut.h>
#include <GL/freeglut_std.h>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>

#include <X11/Xlib.h>
#include <ostream>

using namespace std;

#ifndef PI
#define PI 3.14159265358979323846
#endif

namespace simplecpp {
struct Position {
    double x, y;

  public:
    Position() {
        x = 0.0;
        y = 0.0;
    }
    Position(double x1, double y1) {
        x = x1;
        y = y1;
    }
    Position operator+(Position p) { return Position(x + p.x, y + p.y); }

    double getX() { return x; }

    double getY() { return y; }
    double GetXDistance() { return x; }
    double GetYDistance() { return y; }
    void print(char *msg = nullptr) {
        cout << msg << ": " << x << ", " << y << endl;
    }
};

// ONLY TO MAINTAIN COMPATIBILITY, previously since X11 was used, it's y
// coordinate is 0 at top and increases downwards. While in OpenGL, y coordinate
// is 0 at bottom, and increases upwards So changing y to be same as what would
// it be if top is y=0
// @adi - Remove this once completely moved
static void MakePositionOpenGLCompatible(Position &p) {
    p.y = glutGet(GLUT_WINDOW_HEIGHT) - p.y;
}
// @adi - Remove this once completely moved
static void MakePositionOpenGLCompatible(XPoint &p) {
    p.y = glutGet(GLUT_WINDOW_HEIGHT) - p.y;
}
static void MakePositionOpenGLCompatible(int &y) {
    y = glutGet(GLUT_WINDOW_HEIGHT) - y;
}
static void MakePositionOpenGLCompatible(short &y) {
    y = glutGet(GLUT_WINDOW_HEIGHT) - y;
}

struct RectBox {
    Position top_left;
    Position bottom_right;
    RectBox() {}
    RectBox(Position tl, Position br) {
        top_left = tl;
        bottom_right = br;
    }
    bool intersects(RectBox b2) {
        return (!((top_left.getX() > b2.bottom_right.getX()) ||
                  (top_left.getY() > b2.bottom_right.getY()) ||
                  (bottom_right.getX() < b2.top_left.getX()) ||
                  (bottom_right.getY() < b2.top_left.getY())));
    }
    RectBox boxUnion(RectBox s) {
        RectBox r;
        r.top_left.x = min(top_left.x, s.top_left.x);
        r.top_left.y = min(top_left.y, s.top_left.y);
        r.bottom_right.x = max(bottom_right.x, s.bottom_right.x);
        r.bottom_right.y = max(bottom_right.y, s.bottom_right.y);
        return r;
    }
};

// primitive array (float[3]) cannot be assigned again (wholly)
using u8 = uint8_t;

struct Color {
    // NOTE: To directly pass these values to OpenGL, use glColor3ub, not any other, glColor3b or 3i etc. (they take different range of values)
    u8 r = 127;
    u8 g = 127;
    u8 b = 127;

    Color(unsigned int r, unsigned int g, unsigned int b) : r(r), g(g), b(b) {}

    // For backward compatibility with X APIs
    operator unsigned long() { return r << 16 | g << 8 | b; }

};

    // For printing color component values with cout, etc
    inline std::ostream& operator<<(std::ostream& s, Color& clr) {
        s << "Colour: {" << int(clr.r) << ", " << int(clr.g) << ", " << int(clr.b) << "}";
        return s;
    }
    // For printing point coord with cout, etc
    inline std::ostream& operator<<(std::ostream& s, XPoint& pnt) {
        s << "Point: {" << int(pnt.x) << ", " << int(pnt.y) << "}";
        return s;
    }

} // namespace simplecpp
#endif
