#ifndef _POLYGON_INCLUDED_
#define _POLYGON_INCLUDED_

#include <sprite.h>
#include <vector>
#include <iostream>

namespace simplecpp{
class Polygon : public Sprite{
 protected:
  vector<Position> vertex;
  void init(const double x, const double y, const double points[][2], 
	     int count, Composite* owner=NULL);
 public:
  //  Polygon(const Polygon& other);
  Polygon(Composite* owner = NULL);
  Polygon(const double x, const double y, const double points[][2], int count,
	  Composite* owner=NULL);
  void reset(const double x, const double y, const double points[][2], 
	     int count, Composite* owner=NULL);
  void paint(Pose *p=NULL);
};
}
#endif
