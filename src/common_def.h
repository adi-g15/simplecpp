#ifndef _COMMON_DEF_INCLUDED_
#define _COMMON_DEF_INCLUDED_

#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

#ifndef PI
#define PI 3.14159265358979323846
#endif

namespace simplecpp{
struct Position{
  double x, y;
 public:
  Position(){
    x = y = 0.0;
  }
  Position(double x1, double y1){
    x = x1;
    y = y1;
  }
  Position operator+(Position p){
    return Position(x+p.x, y+p.y);
  }

  double getX(){
    return x;
  }
  
  double getY(){
    return y;
  }
  double GetXDistance(){
    return x;
  }
  double GetYDistance(){
    return y;
  }
  void print(char *msg = NULL){cout << msg <<": "<< x <<", "<< y<<endl;}
};

struct RectBox{
  Position top_left;
  Position bottom_right;
  RectBox(){}
  RectBox(Position tl, Position br){
    top_left = tl;
    bottom_right = br;
  }
  bool intersects(RectBox b2){
    return (!((top_left.getX() > b2.bottom_right.getX()) ||
	      (top_left.getY() > b2.bottom_right.getY()) ||
	      (bottom_right.getX() < b2.top_left.getX()) ||
	      (bottom_right.getY() < b2.top_left.getY())));
  }
  RectBox boxUnion(RectBox s){
    RectBox r;
    r.top_left.x = min(top_left.x, s.top_left.x);
    r.top_left.y = min(top_left.y, s.top_left.y);
    r.bottom_right.x = max(bottom_right.x, s.bottom_right.x);
    r.bottom_right.y = max(bottom_right.y, s.bottom_right.y);
    return r;
  }
};

typedef unsigned long Color;
}
#endif
