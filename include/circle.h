#ifndef _POINT_INCLUDED_
#define _POINT_INCLUDED_

#include <sprite.h>
#include <vector>

namespace simplecpp{
class Circle : public Sprite{
 protected:
  double radius;
 private:
  //  Circle(const Circle&);
  //  Circle operator=(const Circle&);
 public:
  Circle(Composite* owner = NULL);
  Circle(const double x, const double y, const double radius=10,
	  Composite* owner=NULL);
  double getRadius();
  void init(const double x, const double y, const double radius=10, 
	    bool repaintP=true);
  void reset(const double x, const double y, const double radius=10, 
	    bool repaintP=true);
  //  void scale(double s, bool repaintP=true);
  virtual void paint(Pose *p=NULL);
};
}
#endif
