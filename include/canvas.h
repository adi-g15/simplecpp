#ifndef _CANVAS_INCLUDED
#define _CANVAS_INCLUDED

#include <GL/freeglut_std.h>
#include <X11/Xlib.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <thread>
#include <type_traits>
#include <typeindex>
#include <vector>

#include <mutex>

#include "circle.h"
#include "common_def.h"
#include "line.h"
#include "polygon.h"
#include "sprite.h"

using std::string;

inline int canvas_width() { return glutGet(GLUT_WINDOW_WIDTH); }
inline int canvas_height() { return glutGet(GLUT_WINDOW_HEIGHT); }

namespace simplecpp {

// @adi Isko abhi ke liye ignore kr
class SpriteManager {
    // Not using std::unique_ptr, it requires C++14, i dont want to change the
    // dependency on compiler
    std::vector<Sprite *>
        sprites; // from the lowest z-index to highest (this is implicit, since
                 // all draw* routines prints at same z-index, so the first ones
                 // are the ones with lowest priority, they can be overriden by
                 // others)
    std::mutex
        _vec_mutex; // this is not "that costly", since sprites_vec_optimiser
                    // doesn't run always, so most times 'locking' will be just
                    // setting a boolean, atleast on Linux since it uses futexes

    // only print when the state is dirty, this is not altered anyway by
    // sprites_vec_optimiser
    int dirty_writes; // this is okay being single-threaded, since only the
                      // application should use this

    // https://stackoverflow.com/questions/23703661/how-to-get-a-pointer-types-pointed-type

  public:
    SpriteManager() : dirty_writes(0) {}

    // Merge same shapes, no need to find 'covered' shapes for now
    void spritesVecOptimiser();
    void pushBordersSprite(Sprite *s);
    void pushFillSprite(Sprite *s);
    void renderFrame();

    ~SpriteManager() {
        for (auto &ptr : this->sprites) {
            delete ptr;
        }
    }
};

double randuv(double u, double v);
//  void srand(unsigned int);
void wait(float duration);
void abort();

// @adig Changed default width and height to dynamically
int initCanvas(const char window_title[] = "Simplecpp Canvas", int w = -1,
               int h = -1, bool is_centered = true);
// creates the graphics window

void closeCanvas();

int textWidth(string text);
int textWidth(char op);
int textHeight();
int textDescent();

void addSprite(Sprite *t); // adds *t into the list of active sprites
                           // does not repaint immediately?
void removeSprite(Sprite *t);

Color COLOR(const char *color_string); // creates a colour.
Color COLOR(unsigned int red, unsigned int green, unsigned int blue);

void imprintLine(short x1, short y1, short x2, short y2,
                 Color line_color = COLOR(0, 0, 0),
                 unsigned int line_width = 2);
// paints the line into the the background

void drawPointNew(Position point, Color point_color,
                  bool called_by_sprite_manager = false);
void drawLineNew(Position start, Position end, Color line_color,
                 unsigned int line_width = 1);
void drawCircleNew(Position centre, int radius, Color fill_color,
                   bool fill = true, unsigned int line_width = 1,
                   bool called_by_sprite_manager = false);
void drawEllipseNew(Position centre, int width, int height, Color fill_color,
                    bool fill = true, unsigned int line_width = 1,
                    bool called_by_sprite_manager = false);
void drawPolygonNew(vector<Position> &points, Color fill_color,
                    bool fill = true, unsigned int line_width = 1,
                    bool called_by_sprite_manager = false);
void drawTextNew(Position position, string message, Color clr,
                 bool called_by_sprite_manager = false);
void drawText(float x, float y, string text, Color clr,
              bool called_by_sprite_manager = false);

uint32_t getClick(); // wait for a click, and return xclickpos*65536+yclickpos.
void c_imprint(Sprite *s); // paints *s into the background
void repaint();    // paint the current screen into new buffer, then transfer.
void beginFrame(); // suspend painting.  set flag appropriately.
void endFrame();   // resume painting.  reset flag.

#ifndef REMOVE_DEPRECATED
/*
  !deprecated - Only left for backward compatibility

  @adi - Think about implementing these in OpenGL, without XEvent
*/
void nextEvent(XEvent &event, Display* display = nullptr);      // wait for an event: mouseclick, keypress.
bool mouseDragEvent(XEvent &event); // true if event is drag
bool keyPressEvent(XEvent &event);
bool mouseButtonPressEvent(XEvent &event);
bool mouseButtonReleaseEvent(XEvent &event);
char charFromEvent(XEvent &event, Display* display = nullptr); // return char whose key was pressed.
bool checkEvent(
    XEvent &event); // true if some event happened since last nextEvent.
void echoKey(XEvent &event, Color clr = COLOR("blue"), Display* display = nullptr); // print value on screen
void spriteStatus();

/*
  !deprecated - These functions with extra arguments just for backward
  compatibility

   @dev-notes - `line_style` can also be implemented in OpenGL easily
*/
inline void drawPoint(XPoint point, Color point_color, int function = GXcopy) { drawPointNew(Position(point.x,point.y), point_color); }
inline void drawLine(XPoint start, XPoint end, Color line_color,
              unsigned int line_width = 1) { drawLineNew(Position(start.x, start.y), Position(end.x, end.y), line_color, line_width); }
inline void drawCircle(XPoint centre, int radius, Color fill_color, bool fill = true,
                unsigned int line_width = 1, int line_style = LineSolid,
                int cap_style = CapButt, int join_style = JoinMiter,
                int function = GXcopy) { drawCircleNew(Position(centre.x, centre.y), radius, fill_color, fill, line_width); }
inline void drawEllipse(XPoint centre, int width, int height, Color fill_color,
                 bool fill = true, unsigned int line_width = 1,
                 int line_style = LineSolid, int cap_style = CapButt,
                 int join_style = JoinMiter, int function = GXcopy) { drawEllipseNew(Position(centre.x, centre.y), width, height, fill_color, fill, line_width); }
inline void drawPolygon(vector<XPoint> &points, int npoints, Color fill_color,
                 bool fill = true, unsigned int line_width = 1,
                 int line_style = LineSolid, int cap_style = CapButt,
                 int join_style = JoinMiter, int fill_rule = WindingRule,
                 int function = GXcopy) { vector<Position> pnts; for (auto &p : points) { pnts.push_back(Position(p.x, p.y)); } drawPolygonNew(pnts, fill_color, fill, line_width); }
inline void drawText(XPoint position, string message, Color clr) { drawTextNew(Position(position.x, position.y), message, clr); }
#endif

} // namespace simplecpp

#endif
