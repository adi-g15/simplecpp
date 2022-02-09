#ifndef _TEXT_INCLUDED_
#define _TEXT_INCLUDED_

#include "canvas.h"
#include "sprite.h"

namespace simplecpp {
class Text : public Sprite {
  protected:
    string message;

  public:
    Text(double cx, double cy, string m, Composite *owner = nullptr);
    Text(double cx, double cy, double n, Composite *owner = nullptr);
    Text(Composite *owner = nullptr);
    //  void init(double cx, double cy, string m);
    void reset(double cx, double cy, string m);
    //  void init(double cx, double cy, double m);
    void reset(double cx, double cy, double m);
    void setMessage(string s);
    void paint(Pose *p = nullptr);
};
} // namespace simplecpp
#endif
