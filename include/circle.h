#ifndef _POINT_INCLUDED_
#define _POINT_INCLUDED_

#include <vector>

#include "sprite.h"

namespace simplecpp {
class Circle : public Sprite {
  protected:
    double radius;

  private:
    //  Circle(const Circle&);
    //  Circle operator=(const Circle&);
  public:
    Circle(Composite *owner = nullptr);
    Circle(const double x, const double y, const double radius = 10,
           Composite *owner = nullptr);
    double getRadius();
    void init(const double x, const double y, const double radius = 10,
              bool repaintP = true);
    void reset(const double x, const double y, const double radius = 10,
               bool repaintP = true);
    //  void scale(double s, bool repaintP=true);
    virtual void paint(Pose* p = nullptr);
};
} // namespace simplecpp
#endif
