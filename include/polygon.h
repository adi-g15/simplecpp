#ifndef _POLYGON_INCLUDED_
#define _POLYGON_INCLUDED_

#include <iostream>
#include <vector>

#include "sprite.h"

namespace simplecpp {
class Polygon : public Sprite {
  protected:
    vector<Position> vertex;
    void init(const double x, const double y, const double points[][2],
              int count, Composite *owner = nullptr);

  public:
    //  Polygon(const Polygon& other);
    Polygon(Composite *owner = nullptr);
    Polygon(const double x, const double y, const double points[][2], int count,
            Composite *owner = nullptr);
    void reset(const double x, const double y, const double points[][2],
               int count, Composite *owner = nullptr);
    void paint(Pose *p = nullptr);
};
} // namespace simplecpp
#endif
