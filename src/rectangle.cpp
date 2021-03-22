#include <simplecpp>

namespace simplecpp{
Rectangle::Rectangle(Composite* owner)
  : Polygon(0,0,NULL,0,owner){
  init(0,0,0,0);
}

Rectangle::Rectangle(double cx, double cy, double w, double h, Composite* owner)
  : Polygon(cx, cy, NULL, 0, owner){
  init(cx,cy,w,h);
}

void Rectangle::init(double cx, double cy, double w, double h, Composite* owner){
  reset(cx,cy,w,h, owner);
}

void Rectangle::reset(double cx, double cy, double w, double h, Composite* owner){
  //  pose = Pose(cx,cy);

  if(w == -1) w = 10;
  if(h == -1) h = w;
  width = w;
  height = h;

  double points[4][2];
  double hw = w/2, hh = h/2;
  points[0][0] = - hw;
  points[0][1] = - hh;
  points[1][0] = - hw;
  points[1][1] = + hh;
  points[2][0] = + hw;
  points[2][1] = + hh;
  points[3][0] = + hw;
  points[3][1] = - hh;

  Polygon::reset(cx, cy, points,4, owner);
}

double Rectangle::getWidth(){ return width; }
double Rectangle::getHeight(){ return height; }
}
