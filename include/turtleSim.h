#ifndef _TURTLESIM_INCLUDED
#define _TURTLESIM_INCLUDED

#include <string>

#ifndef PI
#define PI 3.14159265358979323846
#endif

namespace simplecpp{
  // Created 2 overloads for convinience, replacing const char* with const string& to prevent 'bad' bugs, for eg. turtleSim(NULL, 100); matching the wrong one
  void turtleSim(const std::string& name="Turtle Simulator", int w=-1, int h=-1, bool is_centered = true);
  void turtleSim(int w=-1, int h=-1);
  void closeTurtleSim();
  void resetTurtle();

  double sine(double x);
  double cosine(double x);
  double tangent(double x);
  double arcsine(double x);
  double arccosine(double x);
  double arctangent(double x);
  double arctangent2(double y, double x);


  void left(double dist);
  void right(double dist);
  void forward(double dist);
  void left(int dist);
  void right(int dist);
  void forward(int dist);
  void penDown(bool state=true);
  void penUp(bool state=true);
  void hide(bool stat=true);
}
#endif
