#ifndef _TEXT_INCLUDED_
#define _TEXT_INCLUDED_

#include <sprite.h>
#include <canvas.h>

namespace simplecpp{
class Text : public Sprite{
 protected:
  string message;
 public:
  Text(double cx, double cy, string m, Composite* owner=NULL);
  Text(double cx, double cy, double n, Composite* owner=NULL);
  Text(Composite* owner=NULL);
  //  void init(double cx, double cy, string m);
  void reset(double cx, double cy, string m);
  //  void init(double cx, double cy, double m);
  void reset(double cx, double cy, double m);
  void setMessage(string s);
  void paint(Pose* p=NULL);
};
}
#endif
