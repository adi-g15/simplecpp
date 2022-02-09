#ifndef _LINE_INCLUDED_
#define _LINE_INCLUDED_

#include "sprite.h"
#include "common_def.h"

namespace simplecpp{
class Line : public Sprite{
 protected:
  Position vertex[2];
  double thickness;
 public:
  Line(Composite* owner=nullptr);
  Line(double x1, double y1, double x2, double y2, Composite* owner=nullptr);

  void init(double x1, double y1, double x2, double y2);
  void reset(double x1, double y1, double x2, double y2);
  void paint(Pose* p=nullptr);
  Position getStart(Pose *p = nullptr);
  Position getEnd(Pose *p = nullptr);

  void setThickness(double value=0);
  double getThickness();
};
}
#endif
