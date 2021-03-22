#include <simplecpp>

namespace simplecpp{
Circle::Circle(Composite* owner)
  : Sprite(0,0, owner){
  radius = 0;
  show();
}

Circle::Circle(const double x, const double y, const double r,
		 Composite *owner)
  : Sprite(x,y,owner){
  radius = r;
  show();
}

void Circle::init(const double x, const double y, const double r, bool repaintP){
  reset(x,y,r,repaintP);
}

void Circle::reset(const double x, const double y, const double r, bool repaintP){
  pose = Pose(x,y);
  radius = r;
  if (repaintP) show();
}

/*
void Circle::scale(double s, bool repaintP){
  radius *= s;
  Sprite::scale(s,repaintP);
}
*/

double Circle::getRadius(){return radius;}

void Circle::paint(Pose *p){
  if(visible){
    XPoint res_center;
    double s;
    if(p){
      Pose r = Pose(*p,pose);
      res_center = r.prod(Position(0,0));
      s = r.getScale();
    }
    else{
      res_center = pose.prod(Position(0,0));
      s = pose.getScale();
    }
    drawEllipse(res_center, int(radius*2 * s), int(radius*2 * s),
		color, fill);  // need x and y diameter 
  }
}

}
