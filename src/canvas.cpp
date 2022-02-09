#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <set>
#include <simplecpp>
#include <thread>

#include <GL/freeglut.h>
#include <GL/gl.h>

#include "canvas.h"
#include "rectangle.h"
#include "sprite.h"

using namespace std;

Display *display = nullptr; // A connection to X server

// Basic information
int screen_number;
int screen_width;
int screen_height;

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
        std::chrono::milliseconds(static_cast<long>(1000 * duration)));
}

int initCanvas(const char window_title[], int width, int height) {

    int argc = 0;
    char **argv = {};
    glutInit(&argc, argv);

    // @remove
    display = XOpenDisplay(nullptr); // Connect X server by opening a display

    // @remove
    screen_number = DefaultScreen(display);

    // @remove
    if (width == -1) {
        screen_width = (DisplayWidth(display, screen_number) - 100);
        screen_height = (DisplayHeight(display, screen_number) - 100);
    } else {
        screen_width = width;
        screen_height = height;
    }

    glutInitWindowSize(screen_width, screen_height);

    if (glutCreateWindow(window_title) == 0) {
        cerr << "Unable to create window\n";
        return 1;
    }

    { // Initialise
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, 800, 0, 600);
    }
    // @remove
    canvas_window = XCreateSimpleWindow(
        display, RootWindow(display, screen_number), 0,
        0, // left top corner.  doesnt work.
        screen_width, screen_height, 1, BlackPixel(display, screen_number),
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

    // @replace
    xfs = XLoadQueryFont(display, "-*-helvetica-bold-r-normal-*-24-*");
    if (!xfs) {
        xfs = XLoadQueryFont(display, "fixed");
    }
    if (!xfs) {
        cout << "Cannot load font.  You will not be able to use Text.\n";
    }

    // @remove
    cmap = DefaultColormap(display, screen_number);
    // @replace - Flush all events to X server and wait
    XSync(display, False);

    // @remove
    screenBG =
        XCreatePixmap(display, canvas_window, screen_width, screen_height,
                      XDefaultDepth(display, screen_number));

    // @remove
    wait(0.1);
    XSync(display, False);
    screenTmp =
        XCreatePixmap(display, canvas_window, screen_width, screen_height,
                      XDefaultDepth(display, screen_number));
    curr_d = canvas_window;

    // @adi This should be repeated on each render
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // @remove
    Rectangle r(screen_width / 2.0, screen_height / 2.0, screen_width,
                screen_height);
    r.setColor(COLOR("white"));
    r.setFill();
    r.imprint();

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

    gc_vals.foreground = line_color;
    gc_vals.line_width = line_width;

    XChangeGC(display, gc, GCForeground | GCLineWidth, &gc_vals);

    // XSync(display, false);

    // Draw line
    XDrawLine(display, curr_d, gc, start.x, start.y, end.x, end.y);

    //  XSync(display, false);
}

void imprintLine(short x1, short y1, short x2, short y2, Color line_color,
                 unsigned int line_width) {

    Drawable temp;
    temp = curr_d;
    curr_d = screenBG;

    gc_vals.foreground = line_color;
    gc_vals.line_width = line_width;

    XChangeGC(display, gc, GCForeground | GCLineWidth, &gc_vals);

    XDrawLine(display, curr_d, gc, x1, y1, x2, y2);

    curr_d = temp;
}

Color COLOR(const char *color_string) {
    if (!display) {
        cout << "You must first call initCanvas before "
             << "using any graphics features.\n";
        exit(1);
    }
    XColor screen_color, true_color;
    XAllocNamedColor(display, cmap, color_string, &screen_color, &true_color);

    return screen_color.pixel;
}

Color COLOR(unsigned int red, unsigned int green, unsigned int blue) {
    Color clr = red << 16 | green << 8 | blue;
    return clr;
}

void drawPoint(XPoint point, Color point_color, int function) {

    gc_vals.foreground = point_color;

    XChangeGC(display, gc, GCForeground, &gc_vals);

    XSync(display, false);

    XDrawPoint(display, curr_d, gc, point.x, point.y);

    XSync(display, false);
}

void drawCircle(XPoint centre, int radius, Color fill_color, bool fill,
                unsigned int line_width, int line_style, int cap_style,
                int join_style, int function) {
    if (fill) {
        line_width = radius;
        radius /= 2;
    }

    XGCValues local_gc_vals;
    local_gc_vals.function = function;
    local_gc_vals.foreground = fill_color;
    local_gc_vals.line_width = line_width;
    local_gc_vals.line_style = line_style;
    local_gc_vals.cap_style = cap_style;
    local_gc_vals.join_style = join_style;

    // Create gc for current drawable
    GC local_gc = XCreateGC(display, curr_d,
                            GCFunction | GCForeground | GCLineWidth |
                                GCLineStyle | GCCapStyle | GCJoinStyle,
                            &local_gc_vals);

    XSync(display, false);

    // Draw full arc
    XDrawArc(display, curr_d, local_gc, centre.x - radius, centre.y - radius,
             radius * 2, radius * 2, 0, 23040);

    XSync(display, false);

    // Free temporary GC
    XFreeGC(display, local_gc);
}

void drawEllipse(XPoint centre, int width, int height, Color fill_color,
                 bool fill, unsigned int line_width, int line_style,
                 int cap_style, int join_style, int function) {

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
}

void drawPolygon(vector<XPoint> &points, int npoints, Color fill_color,
                 bool fill, unsigned int line_width, int line_style,
                 int cap_style, int join_style, int fill_rule, int function) {

    // @remove
    gc_vals.foreground = fill_color;
    gc_vals.line_width = line_width;
    gc_vals.fill_rule = fill_rule;
    XChangeGC(display, gc, GCForeground | GCLineWidth | GCFillRule, &gc_vals);

    if (fill) {
        glBegin(GL_POLYGON);
        for (const auto &pnt : points) {
            glVertex2d(pnt.x, pnt.y);
        }
        glEnd();
        // @remove
        XFillPolygon(display, curr_d, gc, points.data(), npoints, Complex,
                     CoordModeOrigin);
    } else {
        glBegin(GL_LINE_LOOP);
        for (const auto &pnt : points) {
            glVertex2d(pnt.x, pnt.y);
        }
        glEnd();
        // @remove
        vector<XPoint> pts(points);
        pts.push_back(points[0]); // Add first point again at end
        XDrawLines(display, curr_d, gc, pts.data(), npoints + 1,
                   CoordModeOrigin);
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
    if (!display) {
        cout << "Repaint: You must first call initCanvas before "
             << "using any graphics features.\n";
        exit(1);
    }

    if (globalRepaintFlag) {
        int tl_x = 0, tl_y = 0, width = screen_width, height = screen_height;
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

// void drawText(Position position, string text, Color clr){
//   XPoint p;
//   p.x = position.getX(); p.y = position.getY();
//   drawText(p, text.c_str(), clr);
// }

void drawText(float x, float y, string text, Color clr) {
    XPoint p;
    p.x = x;
    p.y = y;
    drawText(p, text.c_str(), clr);
}

void drawText(XPoint position, string message, Color clr) {

    XGCValues local_gc_vals;
    local_gc_vals.foreground = clr;
    local_gc_vals.font = xfs->fid;

    // Create gc for current drawable
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

    // Free temporary GC
    XFreeGC(display, local_gc);
}

// void drawText(XPoint position, const char* text, Color clr){
//   GC local_gc;
//   XGCValues local_gc_vals;

//   // Fill the structure
//   local_gc_vals.foreground = clr;
//   local_gc_vals.font = xfs->fid;

//   // Create gc for current drawable
//   local_gc = XCreateGC(display, curr_d, GCForeground
// 			 |GCFont
// 			 , &local_gc_vals);

//   XTextItem ti;

//   ti.chars = (char*)text;
//   ti.nchars = strlen(text);
//   ti.delta = 0;
//   ti.font = None;

//   XSync(display, false);

//   XDrawText(display, curr_d, local_gc,
// 	      position.x-XTextWidth(xfs,text,strlen(text))/2,
// 	      position.y+textHeight()/2 - textDescent(), &ti, 1);

//   XSync(display, false);

//   // Free temporary GC
//   XFreeGC(display, local_gc);
// }

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
