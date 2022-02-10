#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/freeglut_std.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <set>
#include <simplecpp>
#include <thread>

#include <GL/freeglut.h>
#include <GL/gl.h>

#include "canvas.h"
#include "common_def.h"
#include "rectangle.h"
#include "sprite.h"

using namespace std;

Display *display = nullptr; // A connection to X server

// Basic information
// Draw* routines should not require info about the complete screen (ie.
// laptop/PC screen), instead need the current window's dimensions
int screen_number;
int window_width;
int window_height;

Window canvas_window;

Colormap cmap;

GC gc;
XGCValues gc_vals;

XFontStruct *xfs;

// Current drawable
Drawable curr_d;

Pixmap screenBG; // BG = background:to keep track of lines drawn by sprites.
                 // lines which are sprites are not drawn on this.

Pixmap screenTmp;

struct LtSprite {
    bool operator()(const Sprite *s1, const Sprite *s2) const {
        return s1->getZIndex() < s2->getZIndex();
        //    return s1 < s2;
    }
};

std::multiset<Sprite *, LtSprite> spriteSet;

namespace simplecpp {
double randuv(double u, double v) {
    return u + (v - u) * rand() / (1.0 + RAND_MAX);
}

// void srand(unsigned int seed){
//   std::srand(seed);
// }

void abort() { std::abort(); }

void wait(float duration) {
    std::this_thread::sleep_for(
        std::chrono::seconds(static_cast<long>(duration)));
}

int initCanvas(const char window_title[], int width, int height, bool is_centered) {

    int argc = 0;
    char **argv = {};
    // Calling glutGet for GLUT_INIT_STATE is safe, even before glutInit
    bool already_init = glutGet(GLUT_INIT_STATE) != 0;
    if (!already_init) {
        glutInit(&argc, argv);
    } else {
        // WARN: Already initialised, nothing to do
        // This should not be the case, and initCanvas must be called only once,
        // but handling this case for backward compatibility
    }

    // @remove
    display = XOpenDisplay(nullptr); // Connect X server by opening a display
    screen_number = DefaultScreen(display);

    cout << "Screen width: " << glutGet(GLUT_SCREEN_WIDTH) << endl;
    cout << "Screen height: " << glutGet(GLUT_SCREEN_HEIGHT) << endl;
    // Default is a 8:5 aspect ratio window
    if (width == -1) {
        width = glutGet(GLUT_SCREEN_WIDTH) / 4;

        // @Originally
        // window_width = (DisplayWidth(display, screen_number) - 100);
        // window_height = (DisplayHeight(display, screen_number) - 100);
    }
    if (height == -1) {
        height = 2 * glutGet(GLUT_SCREEN_HEIGHT) / 5;
    }

    // Start in center
    Position top_left(0,0);
    
    if( is_centered ) {
        top_left.x = (glutGet(GLUT_SCREEN_WIDTH) - width) / 2.0f;
        top_left.y = (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2.0f;
    }

    glutInitWindowSize(width, height);
    glutInitWindowPosition(top_left.x, top_left.y);
    glViewport(0, 0, width, height);

    if (!already_init) {
        if (glutCreateWindow(window_title) == 0) {
            cerr << "Unable to create window\n";
            return 1;
        }
    } else {
        // Already init, instead of creating a new window, set properties on
        // existing itself
        glutSetWindowTitle(window_title);
        glutReshapeWindow(width, height);
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    { // Initialise
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, width, 0, height);

        // I want smoother lines and polygons, so enable aliasing
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);

        // Not equating to `width` and `height`, let glut say what it thinks is the
        // window size
        window_width = glutGet(GLUT_WINDOW_WIDTH);
        window_height = glutGet(GLUT_WINDOW_HEIGHT);
    }
    // @remove
    canvas_window = XCreateSimpleWindow(
        display, RootWindow(display, screen_number), 0,
        0, // left top corner.  doesnt work.
        width, height, 1, BlackPixel(display, screen_number),
        WhitePixel(display, screen_number));
    XSetStandardProperties(display, canvas_window, window_title, window_title,
                           None, nullptr, 0, nullptr);
    XSelectInput(display, canvas_window,
                 ExposureMask | ButtonPressMask | ButtonReleaseMask |
                     KeyPressMask | ButtonMotionMask);
    XMapWindow(display, canvas_window); // Map canvas window to display
    XSync(display, True); // flush all output requests and discards events
    gc_vals.function = GXcopy;
    //  gc_vals.foreground = Color(COLOR("black"));
    gc_vals.line_width = 2;
    gc_vals.line_style = LineSolid;
    gc_vals.cap_style = CapButt;
    gc_vals.join_style = JoinMiter;
    gc = XCreateGC(display, canvas_window, 0, 0); // Create GC
    if ((long int)gc < 0) {
        // if(gc < 0){
        cerr << "Unable to create graphic context.\n";
        return 2; // ERROR CODE 2: gc error
    }

    // Use fonts: GLUT_BITMAP_9_BY_15 or GLUT_BITMAP_HELVETICA_18
    // @remove
    xfs = XLoadQueryFont(display, "-*-helvetica-bold-r-normal-*-24-*");
    if (!xfs) {
        xfs = XLoadQueryFont(display, "fixed");
    }
    if (!xfs) {
        cout << "Cannot load font.  You will not be able to use Text.\n";
    }

    // @remove
    cmap = DefaultColormap(display, screen_number);
    XSync(display, False);
    screenBG =
        XCreatePixmap(display, canvas_window, window_width, window_height,
                      XDefaultDepth(display, screen_number));
    wait(0.1);
    XSync(display, False);
    screenTmp =
        XCreatePixmap(display, canvas_window, window_width, window_height,
                      XDefaultDepth(display, screen_number));
    curr_d = canvas_window;
    Rectangle r(window_width / 2.0, window_height / 2.0, window_width,
                window_height);
    r.setColor(COLOR("white"));
    r.setFill();
    r.imprint();

    // @adi This should be repeated on each render
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    return 0;
}

void closeCanvas() {
    XSync(display, True); // Flush all events to X server and wait
    XFreePixmap(display, screenBG);
    XFreePixmap(display, screenTmp);
    XFreeGC(display, gc); // Delete GC
    XDestroyWindow(display, canvas_window);
    XCloseDisplay(display);
    display = nullptr;
    spriteSet.clear(); // Delete all elements from set
}

void drawLine(XPoint start, XPoint end, Color line_color,
              unsigned int line_width) {
    // cout << "Going to draw: " << __func__ << line_color << endl;

    // @remove
    gc_vals.foreground = line_color;
    gc_vals.line_width = line_width;
    XChangeGC(display, gc, GCForeground | GCLineWidth, &gc_vals);
    XDrawLine(display, curr_d, gc, start.x, start.y, end.x, end.y);

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
    glFlush();
}

void imprintLine(short x1, short y1, short x2, short y2, Color line_color,
                 unsigned int line_width) {

    // @remove
    Drawable temp;
    temp = curr_d;
    curr_d = screenBG;
    gc_vals.foreground = line_color;
    gc_vals.line_width = line_width;
    XChangeGC(display, gc, GCForeground | GCLineWidth, &gc_vals);
    XDrawLine(display, curr_d, gc, x1, y1, x2, y2);

    // @remove - CAUTION
    curr_d = temp;

    MakePositionOpenGLCompatible(y1);
    MakePositionOpenGLCompatible(y2);
    glLineWidth(line_width);
    glColor3ub(line_color.r, line_color.g, line_color.b);
    // Draw line from (x1,y1) to (x2,y2)
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
    glFlush();
}

// TODO: Modify this to return {float, float, float} instead, DONT DELETE THIS,
// this is part of exported API
Color COLOR(const char *color_string) {
    string s(color_string);
    for (auto &c : s) {
        c = tolower(c);
    }

    // Colors supported by ncurses, these are the 8 ANSI colors, there are no 'standard' names for other colors
    // Ref: man page - curs_color(3x)
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
    } else {
        cout << "Warning: Don't know the colour: " << color_string << '\n';
        return {127, 127, 127}; /*DEFAULT_COLOR*/
    }
}

Color COLOR(unsigned int red, unsigned int green, unsigned int blue) {
    return {red, green, blue};
}

void drawPoint(XPoint point, Color point_color, int function) {
    // cout << "Going to draw: " << __func__ << point_color << endl;
    // cout << "Data: " << __func__ << point << endl;
    // @remove
    gc_vals.foreground = point_color;
    XChangeGC(display, gc, GCForeground, &gc_vals);
    XSync(display, false);
    XDrawPoint(display, curr_d, gc, point.x, point.y);
    XSync(display, false);

    MakePositionOpenGLCompatible(point);
    glColor3ub(point_color.r, point_color.g, point_color.b);
    // Draw point (point.x, point.y)
    glBegin(GL_POINTS);
    glVertex2d(point.x, point.y);
    glEnd();
    glFlush();
}

void drawCircle(XPoint centre, int radius, Color fill_color, bool fill,
                unsigned int line_width, int line_style, int cap_style,
                int join_style, int function) {
    // cout << "Going to draw: " << __func__ << fill_color << endl;
    // cout << "Data: " << __func__ << "; Center: " << centre
    //      << "; Radius: " << radius << endl;
    // @remove
    int new_radius = radius;
    if (fill) {
        line_width = radius;
        new_radius /= 2;
    }
    XGCValues local_gc_vals;
    local_gc_vals.function = function;
    local_gc_vals.foreground = fill_color;
    local_gc_vals.line_width = line_width;
    local_gc_vals.line_style = line_style;
    local_gc_vals.cap_style = cap_style;
    local_gc_vals.join_style = join_style;
    GC local_gc = XCreateGC(display, curr_d,
                            GCFunction | GCForeground | GCLineWidth |
                                GCLineStyle | GCCapStyle | GCJoinStyle,
                            &local_gc_vals);
    XSync(display, false);
    XDrawArc(display, curr_d, local_gc, centre.x - new_radius,
             centre.y - new_radius, new_radius * 2, new_radius * 2, 0, 23040);
    XSync(display, false);
    XFreeGC(display, local_gc);

    MakePositionOpenGLCompatible(centre);
    glLineWidth(line_width);
    glColor3ub(fill_color.r, fill_color.g, fill_color.b);
    // There is no 'direct way' to draw a circle in OpenGL, instead we draw a
    // polygon with MANY sides (num_segments), such that it 'seems' that is a
    // circle, that is no corner should be visible
    int num_segments = 500; // approximate a circle by drawing num_segments
                            // sides of polygon, THIS MUST BE A LARGE VALUE, if
                            // too much large, then it will hit performance
                            // 500 and 10,000 both give same result on my laptop

    if (fill) {
        // For a filled circle, GL_TRIANGLE_FAN is the best way
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(centre.x, centre.y);
        for (int i = 0; i < num_segments +1 /*+1 required else the circle will not be drawn completely*/; i++) {
            const auto angle =
                i * (2 * PI / num_segments); // angle 'wrt x-axis'
            const auto x = radius * cos(angle);
            const auto y = radius * sin(angle);

            // The `x` & `y` above is considering center at (0,0), so moving it
            // to correct coordinates by moving center to (center.x, center.y)
            glVertex2d(x + centre.x, y + centre.y);
        }
        glEnd();
    } else {
        // For just the circumference, GL_LINE_LOOP is the best way in this API
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < num_segments; i++) {
            const auto angle = i * (2 * PI / num_segments);
            const auto x = radius * cos(angle);
            const auto y = radius * sin(angle);

            glVertex2d(x + centre.x, y + centre.y);
        }
        glEnd();
    }
    glFlush();
}

void drawEllipse(XPoint centre, int width, int height, Color fill_color,
                 bool fill, unsigned int line_width, int line_style,
                 int cap_style, int join_style, int function) {
    // cout << "Going to draw: " << __func__ << fill_color << endl;

    gc_vals.foreground = fill_color;
    gc_vals.line_width = line_width;

    XChangeGC(display, gc, GCForeground | GCLineWidth, &gc_vals);

    //  XSync(display, false);

    if (fill)
        XFillArc(display, curr_d, gc, centre.x - width / 2,
                 centre.y - height / 2, width, height, 0, 23040);
    else
        XDrawArc(display, curr_d, gc, centre.x - width / 2,
                 centre.y - height / 2, width, height, 0, 23040);

    //  XSync(display, false);
    // TODO
}

void drawPolygon(vector<XPoint> &points, int npoints, Color fill_color,
                 bool fill, unsigned int line_width, int line_style,
                 int cap_style, int join_style, int fill_rule, int function) {
    // cout << "Going to draw: " << __func__ << fill_color << endl;

    // @remove - CAUTION: All these parameters should be supported as much as
    // can
    gc_vals.foreground = fill_color;
    gc_vals.line_width = line_width;
    gc_vals.fill_rule = fill_rule;
    XChangeGC(display, gc, GCForeground | GCLineWidth | GCFillRule, &gc_vals);

    glLineWidth(line_width);
    glColor3ub(fill_color.r, fill_color.g, fill_color.b);
    if (fill) {
        // @remove
        XFillPolygon(display, curr_d, gc, points.data(), npoints, Complex,
                     CoordModeOrigin);

        glBegin(GL_POLYGON);
        for (auto &pnt : points) {
            MakePositionOpenGLCompatible(pnt);
            glVertex2d(pnt.x, pnt.y);
        }
        glEnd();
    } else {
        // @remove
        vector<XPoint> pts(points);
        pts.push_back(points[0]); // Add first point again at end
        XDrawLines(display, curr_d, gc, pts.data(), npoints + 1,
                   CoordModeOrigin);

        glBegin(GL_LINE_LOOP);
        for (auto &pnt : points) {
            MakePositionOpenGLCompatible(pnt);
            glVertex2d(pnt.x, pnt.y);
        }
        glEnd();
    }

    glFlush();
}

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

void c_imprint(Sprite *s) {
    Drawable temp;
    temp = curr_d;
    curr_d = screenBG;
    s->paint();
    curr_d = temp;
}

bool globalRepaintFlag = true;
void beginFrame() { globalRepaintFlag = false; }
void endFrame() {
    globalRepaintFlag = true;
    repaint();
}

void repaint() {
    // GLUT will optimise repeated calls too, so no need for globalRepaintFlag
    glutPostRedisplay();

    // @remove
    if (!display) {
        cout << "Repaint: You must first call initCanvas before "
             << "using any graphics features.\n";
        exit(1);
    }
    if (globalRepaintFlag) {
        int tl_x = 0, tl_y = 0, width = window_width, height = window_height;
        XCopyArea(display, screenBG, screenTmp, gc, tl_x, tl_y, width, height,
                  tl_x, tl_y);

        curr_d = screenTmp;

        for (auto iter = spriteSet.begin(); iter != spriteSet.end(); iter++) {
            (*iter)->paint();
        }

        XCopyArea(display, screenTmp, canvas_window, gc, tl_x, tl_y, width,
                  height, tl_x, tl_y);

        XSync(display, false);
        curr_d = canvas_window;
    }
}

void nextEvent(XEvent &event) {
    XSync(display, False);
    XNextEvent(display, &event);
}

char charFromEvent(XEvent &event) {
    // char c = XKeycodeToKeysym(display, event.xkey.keycode, event.xkey.state);

    int keysyms_per_keycode_return;
    char c = *XGetKeyboardMapping(display, event.xkey.keycode, 1,
                                  &keysyms_per_keycode_return);
    return c;
}

void echoKey(XEvent &event, Color clr) {
    char c = charFromEvent(event);
    int tw = XTextWidth(xfs, &c, 1);
    drawText(event.xkey.x + tw / 2, event.xkey.y, string(1, c), clr);
    XWarpPointer(display, None, None, 0, 0, 0, 0, tw, 0);
}

int textWidth(char op) {
    string s;
    s = op;
    return textWidth(s);
}

int textWidth(string text) {
    return XTextWidth(xfs, text.c_str(), text.size());
}
int textHeight() { return xfs->ascent + xfs->descent; }
int textDescent() { return xfs->descent; }

void drawText(float x, float y, string text, Color clr) {
    // cout << "Going to draw: " << __func__ << clr << endl;
    XPoint p;
    p.x = x;
    p.y = y;
    drawText(p, text.c_str(), clr);
}

void drawText(XPoint position, string message, Color clr) {
    // cout << "Going to draw: " << __func__ << clr << endl;

    // @remove
    XGCValues local_gc_vals;
    local_gc_vals.foreground = clr;
    local_gc_vals.font = xfs->fid;
    GC local_gc =
        XCreateGC(display, curr_d, GCForeground | GCFont, &local_gc_vals);
    char *cstr = new char[message.size() + 1];
    strcpy(cstr, message.c_str());
    XTextItem ti;
    ti.chars = cstr;
    ti.nchars = strlen(cstr);
    ti.delta = 0;
    ti.font = None;
    XSync(display, false);
    XDrawText(display, curr_d, local_gc, position.x - textWidth(message) / 2,
              position.y + textHeight() / 2 - textDescent(), &ti, 1);
    XSync(display, false);
    XFreeGC(display, local_gc);

    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message.c_str());
}

void spriteStatus() {
    cout << "Count: " << spriteSet.size() << endl;
    for (auto iter = spriteSet.begin(); iter != spriteSet.end(); iter++) {
        cout << "["
             << "]-->" << (*iter) << endl;
    }
}

bool checkEvent(XEvent &event) {
    if (XCheckMaskEvent(display,
                        ButtonReleaseMask | ButtonPressMask | KeyPressMask,
                        &event))
        return true;
    return false;
}

bool mouseDragEvent(XEvent &event) { return event.type == MotionNotify; }

bool mouseButtonPressEvent(XEvent &event) { return event.type == ButtonPress; }

bool mouseButtonReleaseEvent(XEvent &event) {
    return event.type == ButtonRelease;
}

bool keyPressEvent(XEvent &event) { return event.type == KeyPress; }

int getClick() {
    XEvent event;

    XSync(display, true); // discard all previous clicks
    while (true) {
        nextEvent(event);
        if (event.type == ButtonPress) {
            return event.xbutton.x * 65536 + event.xbutton.y;
        }
    }
}

} // namespace simplecpp
