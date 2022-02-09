#ifndef _RECTANGLE_INCLUDED_
#define _RECTANGLE_INCLUDED_

#include "canvas.h"
#include "polygon.h"

namespace simplecpp {
class Rectangle : public Polygon {
  protected:
    double width, height;

  public:
    Rectangle(double cx, double cy, double w, double h = -1,
              Composite *owner = nullptr);
    Rectangle(Composite *owner = nullptr);
    void init(double cx, double cy, double w, double h = -1,
              Composite *owner = nullptr);
    void reset(double cx, double cy, double w, double h = -1,
               Composite *owner = nullptr);
    double getWidth();
    double getHeight();
};
} // namespace simplecpp
#endif
