#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <set>
#include <simplecpp>
#include <stdexcept>
#include <string>
#include <thread>

#include "canvas.h"
#include "common_def.h"
#include "rectangle.h"
#include "sprite.h"

using namespace std;

static SpriteManager __sprite_mgr;
#ifndef REMOVE_DEPRECATED
struct LtSprite {
    bool operator()(const Sprite *s1, const Sprite *s2) const {
        return s1->getZIndex() < s2->getZIndex();
        //    return s1 < s2;
    }
};
std::multiset<Sprite *, LtSprite> spriteSet;
#endif

namespace simplecpp {

// clang-format off
// !My opinion is this can be removed now
double randuv(double u, double v) {return u + (v - u) * rand() / (1.0 + RAND_MAX);}
void abort() { std::abort(); }
// clang-format on

void wait(float duration) {
    std::this_thread::sleep_for(
        std::chrono::seconds(static_cast<long>(duration)));
}

int initCanvas(const char window_title[], int width, int height,
               bool is_centered) {
    int argc = 0;
    char **argv = {};

    bool already_init = glutGet(GLUT_INIT_STATE) != 0;
    if (!already_init) {
        glutInit(&argc, argv);
    } else {
        // WARN: Already initialised, nothing to do
        // !deprecated, initCanvas should not be called multiple times
    }

    auto screen_width = glutGet(GLUT_SCREEN_WIDTH);
    auto screen_height = glutGet(GLUT_SCREEN_HEIGHT);
    // Default is a 8:5 aspect ratio window
    if (width == -1) {
        width = screen_width / 4;
    }
    if (height == -1) {
        height = 2 * screen_height / 5;
    }

    // Start in center
    Position top_left(0, 0);

    if (is_centered) {
        top_left.x = (screen_width - width) / 2.0f;
        top_left.y = (screen_height - height) / 2.0f;
    }

    // Observation:
    // Composite objects with too many are the problem

    // Incompletes:
    // SpriteManager

    // Optimisations:
    // 1. Don't glFlush everywhere
    // 2. Double buffered windows are MUCH MUCH faster
    // 3. Using Vertex Buffer Objects, instead of sending each point individually is faster

    // glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(top_left.x, top_left.y);
    glViewport(0, 0, width, height);

    if (!already_init) {
        if (glutCreateWindow(window_title) == 0) {
            cerr << "Unable to create window\n";
            return 1;
        }
    } else {
        // Already init, modify existing window
        glutSetWindowTitle(window_title);
        glutReshapeWindow(width, height);
    }

    { // Initialise
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, width, 0, height);

        // I want smoother lines and polygons, Enable aliasing
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
    }

    glutCloseFunc([]() { std::cout << "Should close;" << std::endl; });
    glutReshapeFunc([](int w, int h){
        cout << "Reshaped to " << w << "x" << h << endl;

        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION_MATRIX);
        glLoadIdentity();
        gluOrtho2D(0, w, 0, h);

        glutPostRedisplay();
    });

    glutDisplayFuncUcall(
        [](void *sm) {
            auto *sprite_manager = (SpriteManager *)sm;

            sprite_manager->renderFrame();
        },
        &__sprite_mgr);

    __sprite_mgr.init();

    // @adi This should be repeated on each render
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    // Only to handle reshape event
    glutMainLoopEvent();

    return 0;
}

void imprintLine(short x1, short y1, short x2, short y2, Color line_color,
                 unsigned int line_width) {
    MakePositionOpenGLCompatible(y1);
    MakePositionOpenGLCompatible(y2);
    glLineWidth(line_width);
    glColor3ub(line_color.r, line_color.g, line_color.b);
    // Draw line from (x1,y1) to (x2,y2)
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
}

Color COLOR(const char *color_string) {
    string s(color_string);
    for (auto &c : s) {
        c = tolower(c);
    }

    // Colors supported by ncurses, these are the 8 ANSI colors, there are no
    // 'standard' names for other colors Ref: man page - curs_color(3x)
    if (s == "red") {
        return {255, 0, 0};
    } else if (s == "green") {
        return {0, 255, 0};
    } else if (s == "blue") {
        return {0, 0, 255};
    } else if (s == "white") {
        return {255, 255, 255};
    } else if (s == "black") {
        return {0, 0, 0};
    } else if (s == "yellow") {
        return {255, 255, 0};
    } else if (s == "magenta") {
        return {255, 0, 255};
    } else if (s == "cyan") {
        return {0, 255, 255};
    } else if (s == "pink") {
        return {255, 192, 203};
    } else if (s == "purple") {
        return {128, 0, 128};
    } else if (s == "grey" || s == "gray") {
        return {128, 128, 128};
    } else {
        cout << "Warning: Don't know the colour (Non standard colour): "
             << color_string << '\n';
        return {127, 127, 127}; /*DEFAULT_COLOR*/
    }
}

Color COLOR(unsigned int red, unsigned int green, unsigned int blue) {
    return {red, green, blue};
}

void drawPointNew(Position point, Color point_color,
                  bool called_by_sprite_manager) {
    // cout << "Going to draw: " << __func__ << point_color << endl;

    MakePositionOpenGLCompatible(point);
    glColor3ub(point_color.r, point_color.g, point_color.b);
    glBegin(GL_POINTS);
    glVertex2d(point.x, point.y);
    glEnd();
}

void drawLineNew(Position start, Position end, Color line_color,
                 unsigned int line_width) {
    // cout << "Going to draw: " << __func__ << line_color << endl;

    MakePositionOpenGLCompatible(start);
    MakePositionOpenGLCompatible(end);
    if (line_width == 0) {
        line_width = 1;
    }
    glLineWidth(line_width);
    glColor3ub(line_color.r, line_color.g, line_color.b);
    // Draw line
    glBegin(GL_LINES);
    glVertex2d(start.x, start.y);
    glVertex2d(end.x, end.y);
    glEnd();
    // glFlush();
}

void drawCircleNew(Position centre, int radius, Color fill_color, bool fill,
                   unsigned int line_width, bool called_by_sprite_manager) {
    return drawEllipseNew(centre, radius, radius, fill_color, fill, line_width,
                          called_by_sprite_manager);
}
void drawEllipseNew(Position centre, int rx, int ry, Color fill_color,
                    bool fill, unsigned int line_width,
                    bool called_by_sprite_manager) {
    // cout << "Going to draw: " << __func__ << fill_color << endl;

    // Algorithm: https://stackoverflow.com/a/34735255/12339402
    // Idea is to draw a circle with radius 1, then scale the coordinates

    /*
     TODO: OPTIMISATION: If Vertex Buffer Objects are used, then won't have to
     send each vertex, so will be faster
    */

    MakePositionOpenGLCompatible(centre);
    glLineWidth(line_width);
    glColor3ub(fill_color.r, fill_color.g, fill_color.b);
    int num_segments = 100; // approximate a circle by drawing num_segments
                            // sides of polygon, THIS MUST BE A LARGE VALUE, if
                            // too much large, then it will hit performance
                            // 500 and 10,000 both give same result on my laptop

    // precompute cos and sin: https://stackoverflow.com/q/5886628/12339402
    auto theta = 2 * PI / num_segments;
    auto cos1 = cos(theta);
    auto sin1 = sin(theta);

    float x = 1; // starting at angle=0
    float y = 0;

    if (fill) {
        // For a filled circle, GL_TRIANGLE_FAN is the best way
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(centre.x, centre.y);
        // num_segments+1 so the circle will be drawn 'completely'*/
        for (int i = 0; i < num_segments + 1; i++) {
            // The `x` & `y` above is considering center at (0,0), so moving it
            glVertex2d(x * rx + centre.x, y * ry + centre.y);

            // Apply rotation matrix
            auto t = x;
            x = x * cos1 - y * sin1;
            y = y * cos1 + t * sin1;
        }
        glEnd();
    } else {
        // For just the circumference, GL_LINE_LOOP is the best way in this API
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < num_segments; i++) {
            glVertex2d(x * rx + centre.x, y * ry + centre.y);

            // Apply rotation matrix
            auto t = x;
            x = x * cos1 - y * sin1;
            y = y * cos1 + t * sin1;
        }
        glEnd();
    }
    // glFlush();
}

void drawPolygonNew(vector<Position> &points, Color fill_color, bool fill,
                    unsigned int line_width, bool called_by_sprite_manager) {
    // cout << "Draw: " << __func__ << ": ";
    // for (const auto &p : points) {
    //     cout << p.x << "," << p.y << "; ";
    // }
    // cout << '\n';

    glLineWidth(line_width);
    glColor3ub(fill_color.r, fill_color.g, fill_color.b);
    if (fill) {
        glBegin(GL_POLYGON);
        for (auto &pnt : points) {
            MakePositionOpenGLCompatible(pnt);
            glVertex2d(pnt.x, pnt.y);
        }
        glEnd();
    } else {
        glBegin(GL_LINE_LOOP);
        for (auto &pnt : points) {
            MakePositionOpenGLCompatible(pnt);
            glVertex2d(pnt.x, pnt.y);
        }
        glEnd();
    }

    // glFlush();
}

bool globalRepaintFlag = true;
void beginFrame() { globalRepaintFlag = false; }
void endFrame() {
    glutSwapBuffers();
    globalRepaintFlag = true;
    repaint();
    glutSwapBuffers();
}

void repaintNew() {
    // GLUT will optimise repeated calls too, so no need for globalRepaintFlag
    glutPostRedisplay();
}

void drawTextNew(Position position, string message, Color clr,
                 bool called_by_sprite_manager) {
    // TODO: Positioning
    // TODO: Fonts: GLUT_BITMAP_9_BY_15 or GLUT_BITMAP_HELVETICA_18
    glColor3ub(clr.r, clr.g, clr.b);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18,
                     (const unsigned char *)message.c_str());
}

void drawText(float x, float y, string text, Color clr,
              bool called_by_sprite_manager) {
    // cout << "Going to draw: " << __func__ << clr << endl;
    drawTextNew({x, y}, text, clr);
}

uint32_t getClick() {
    glFlush();
    int click_coords[2] = {-1, -1};

    /**
     * Notes:
     * 1. All buttons (left,middle,right, etc) on mouse will count as mouse
     *click
     * 2. GLUT_UP or GLUT_DOWN both are being treated same
     **/
    glutMouseFuncUcall(
        [](int, int, int x, int y, void *click_coords) {
            ((int *)click_coords)[0] = x;
            ((int *)click_coords)[1] = y;
        },
        &click_coords);

    while ((click_coords[0] == -1) || (click_coords[1] == -1)) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds((1000 / 60) /*60 fps*/));

        // glutMainLoopEvent will look for events, and when mouse click is
        // found, GLUT itself calls the callback assigned above
        glutMainLoopEvent();
    }

    return click_coords[0] * 65536 + click_coords[1];
}

void SpriteManager::spritesVecOptimiser() {
    // TODO
    if (dirty_writes == 0) {
        // Skip, since no sprite was added
        return;
    }

    std::lock_guard<std::mutex> lock(_vec_mutex);

    // For the type comparisons to work, both types must have a virtual
    // function https://stackoverflow.com/a/28766385/12339402
    {
        // Circles
        for (size_t i = 0; i < sprites.size(); ++i) {
            for (size_t j = 0; j < sprites.size(); ++j) {
                if ((dynamic_cast<Circle *>(sprites[i]) != nullptr) ||
                    (dynamic_cast<Circle *>(sprites[i]) != nullptr)) {
                }
            }
        }
    }
    {
        // Polygons
        for (size_t i = 0; i < sprites.size(); ++i) {
            for (size_t j = 0; j < sprites.size(); ++j) {
                if ((dynamic_cast<Polygon *>(sprites[i]) != nullptr) ||
                    (dynamic_cast<Polygon *>(sprites[i]) != nullptr)) {
                }
            }
        }
    }
    {
        // Lines
        for (size_t i = 0; i < sprites.size(); ++i) {
            for (size_t j = 0; j < sprites.size(); ++j) {
                if ((dynamic_cast<Line *>(sprites[i]) != nullptr) ||
                    (dynamic_cast<Line *>(sprites[i]) != nullptr)) {
                }
            }
        }
    }
    {
        // Text (ignore, not much benefit unless you use too much which
        // is hardly the case and same is not advised here)
    }
}

void SpriteManager::pushBordersSprite(Sprite *s) {
    // sprite that are only to be shown borders of (ie. not filled)
}

void SpriteManager::pushFillSprite(Sprite *s) {
    // sprite that are to be filled
    sprites.push_back(s);
    this->dirty_writes++;
}

void SpriteManager::renderFrame() {
    static int i = 1;
    cout << i++ << ": render requested\n";
    // TODO
}

#ifndef REMOVE_DEPRECATED
// !deprecated: No use now, will do anything needed in destructor of
// SpriteManager
void closeCanvas() {}
void addSprite(Sprite *t) {
    if (t) {
        spriteSet.insert(t);
    }
}
void removeSprite(Sprite *t) {
    if (t) {
        for (auto iter = spriteSet.begin(); iter != spriteSet.end(); iter++) {
            if ((*iter) == t) {
                spriteSet.erase(iter);
                break;
            }
        }
    }
    repaint();
}
void c_imprint(Sprite *s) { s->paint(); }
void repaint() {
    if (globalRepaintFlag) {
        for (auto iter = spriteSet.begin(); iter != spriteSet.end(); iter++) {
            (*iter)->paint();
        }
        glFlush();
    }
}

// clang-format off
void nextEvent(XEvent &event, Display *display) {throw std::logic_error("Removed, all X-based functionality is removed");}
char charFromEvent(XEvent &event, Display *display) {throw std::logic_error("Removed, all X-based functionality is removed");}
void echoKey(XEvent &event, Color clr, Display *display) {throw std::logic_error("Removed, please get the character, convert to string and pass it to drawText yourself");}
int textWidth(char op) { return textWidth(std::to_string(op)); }
int textWidth(string text) {throw std::logic_error("Removed, all X-based functionality is removed");}
int textHeight() {throw std::logic_error("Removed, all X-based functionality is removed");}
int textDescent() {throw std::logic_error("Removed, all X-based functionality is removed");}
void spriteStatus() {cout << "Count: " << 0 << endl;}
// clang-format off
/*
 ! deprecated -> These are no more usable, since the XEvent parameter will
 *  require to be entered by the caller, nor was used anywhere in code
*/
bool keyPressEvent(XEvent &event) { return event.type == KeyPress; }
bool mouseDragEvent(XEvent &event) { return event.type == MotionNotify; }
bool mouseButtonPressEvent(XEvent &event) { return event.type == ButtonPress; }
bool mouseButtonReleaseEvent(XEvent &event) {return event.type == ButtonRelease;}
bool checkEvent(XEvent &event, Display* display = nullptr) {throw std::logic_error("Removed, all X-based functionality is removed");}
// clang-format on
#endif
} // namespace simplecpp
