#ifndef _POSE_INCLUDED
#define _POSE_INCLUDED
#include <common_def.h>

namespace simplecpp{
class Pose{
  double a11,a12,a13,a21,a22,a23;
 public:
  Pose(){
    a11=a22=1;
    a21=a12=a13=a23=0;
  }
  Pose(double dx,double dy){
    a11=a22=1;
    a21=a12=0;
    a13=dx; a23=dy;
  }
  Pose(double theta){
    a11=a22=cos(theta);
    a21=sin(theta);
    a12=-a21;
    a13=a23=0;
  }
  Pose(double scale, bool s){
    a11=a22=scale;
    a21=a12=a13=a23=0;
  }
  Pose(const Pose &p, const Pose &q){
    a11 = p.a11*q.a11+p.a12*q.a21;
    a12 = p.a11*q.a12+p.a12*q.a22;
    a13 = p.a11*q.a13+p.a12*q.a23+p.a13;
    a21 = p.a21*q.a11+p.a22*q.a21;
    a22 = p.a21*q.a12+p.a22*q.a22;
    a23 = p.a21*q.a13+p.a22*q.a23+p.a23;
  }
  XPoint prod(Position p) const{
    XPoint xp;
    xp.x = a11*p.x+a12*p.y+a13 + 0.5;  // round, not truncate.
    xp.y = a21*p.x+a22*p.y+a23 + 0.5;
    return xp;
  }

  Position prodPosition(Position p) const{
    return Position(a11*p.x+a12*p.y+a13, a21*p.x+a22*p.y+a23);
  }

  void compute_res_vertex(XPoint* xpt, vector<Position> vertex, int count) const{
    for(int i=0; i<count; i++)
      xpt[i] = prod(vertex[i]);
  }

  void compute_res_vertex(XPoint* xpt, Position *vertex, int count) const{
    for(int i=0; i<count; i++)
      xpt[i] = prod(vertex[i]);
  }

  Position unitX() const{
    double s = getScale();
    return Position(a11/s,a21/s);}

  Position getOrigin() const{
    return Position(a13,a23);
  }

  Position getAtan2() const{
    return Position(a11,a21);
  }

  double getOrientation() const{
    return atan2(a21,a11);
  }

  double X() const{ return a13; }
  double Y() const{ return a23; }

  double getScale() const {return sqrt(pow(a11,2)+pow(a12,2));}

  void setScale(double s){
    double olds = getScale();
    double r = s/olds;
    a11 *= r;
    a12 *= r;
    //a13 *= r;
    a21 *= r;
    a22 *= r;
    //a23 *= r;
  }

  void print() const{
    cout << a11 <<","<<a12<<","<<a13<<"  "<<a21<<","<<a22<<","<<a23<<endl;
  }
};
}
#endif
