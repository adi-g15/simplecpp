#include <simplecpp>

namespace simplecpp{
void Line::init(double x1, double y1, double x2, double y2){
  reset(x1,y1,x2,y2);
}

void Line::reset(double x1, double y1, double x2, double y2){
  pose = Pose((x1+x2)/2,(y1+y2)/2);
	      
  double dx = (x1-x2)/2;
  double dy = (y1-y2)/2;
  vertex[0] = Position(dx,dy);
  vertex[1] = Position(-dx,-dy);
  thickness = 2;
  show();
}

Line::Line(Composite* owner) : Sprite(0,0,owner) {
  init(0,0,0,0);
}

Line::Line(double x1, double y1, double x2, double y2, Composite* owner)
  : Sprite((x1+x2)/2, (y1+y2)/2, owner){
  init(x1,y1,x2,y2);
}

void Line::paint(Pose *p){
  if(visible){
    XPoint xpt[2];
    if(p){
      Pose r = Pose(*p,pose);
      r.compute_res_vertex(xpt, vertex, 2);
    }
    else pose.compute_res_vertex(xpt, vertex, 2);

    drawLine(xpt[0], xpt[1], color, (int) thickness);    
  } 
}

void Line::setThickness(double value){
  if(value > 0){
    thickness = value;
    paint();
  }
}

double Line::getThickness(){
  return thickness;
}

Position Line::getStart(Pose *p){
  return p ? Pose(*p,pose).prodPosition(vertex[0]) : 
    pose.prodPosition(vertex[0]);
}

Position Line::getEnd(Pose *p){
  return p ? Pose(*p,pose).prodPosition(vertex[1]) : 
    pose.prodPosition(vertex[1]);
}
}
