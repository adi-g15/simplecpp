#include <simplecpp>
#include <sstream>

namespace simplecpp{
Text::Text(double cx, double cy, string m, Composite* owner)
  : Sprite(cx, cy, owner){
  reset(cx, cy, m);
}

Text::Text(double cx, double cy, double m, Composite* owner)
  : Sprite(cx, cy, owner){
  reset(cx, cy, m);
}

Text::Text(Composite* owner)
  : Sprite(owner){
  reset(0,0,"");
}

void Text::init(double cx, double cy, string m){
  reset(cx,cy,m);
}
void Text::init(double cx, double cy, double m){
  reset(cx,cy,m);
}

void Text::reset(double cx, double cy, string m){
  pose = Pose(cx,cy);
  message = m;
  show();
}

void Text::reset(double cx, double cy, double m){
  ostringstream message;
  message << m;
  reset(cx, cy, message.str());
}

void Text::paint(Pose *p){
  if(visible){
    XPoint xp = (p ? Pose(*p,pose) : pose).prod(Position(0,0));
    drawText(xp, message, color);
  }
}

void Text::setMessage(string s){
  message = s;
  repaint();
}


}
