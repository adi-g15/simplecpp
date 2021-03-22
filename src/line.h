#ifndef _LINE_INCLUDED_
#define _LINE_INCLUDED_

#include <sprite.h>
#include <common_def.h>

namespace simplecpp{
class Line : public Sprite{
 protected:
  Position vertex[2];
  double thickness;
 public:
  Line(Composite* owner=NULL);
  Line(double x1, double y1, double x2, double y2, Composite* owner=NULL);

  void init(double x1, double y1, double x2, double y2);
  void reset(double x1, double y1, double x2, double y2);
  void paint(Pose* p=NULL);
  Position getStart(Pose *p = NULL);
  Position getEnd(Pose *p = NULL);

  void setThickness(double value=0);
  double getThickness();
};
}
#endif
