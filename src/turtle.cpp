#include <simplecpp>

namespace simplecpp{
Turtle::Turtle(Composite* owner){
  double pts_body[4][2] = {{0, 15}, {20, 0}, {0, -15}, {0,15}};
  setColor(COLOR("red"), false);
  setFill();
  penDown();
  reset(canvas_width()/2, canvas_height()/2,pts_body, 4, owner);
}
}
