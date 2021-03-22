#include <simplecpp>
//#include <cstdlib>

Turtle *_PsystemTurtle_;

void check(const char* msg){
  if(!_PsystemTurtle_){
    cout << "You need to call turtleSim before calling "<<msg<<".\n";
    exit(1);
  }
}


namespace simplecpp{

  void turtleSim(const char* name, int w, int h){
    initCanvas(name,w,h);
    _PsystemTurtle_=new Turtle;
  }

  void closeTurtleSim(){
    closeCanvas();
  }

  void resetTurtle(){
    _PsystemTurtle_->Sprite::reset(canvas_width()/2,canvas_height()/2);
  }

  void hide(bool state){
    if(state) _PsystemTurtle_->hide(); 
    else _PsystemTurtle_->show();
  }
  void penDown(bool state){check("penDown"); _PsystemTurtle_->penDown(state);}
  void penUp(bool state){check("penUp"); _PsystemTurtle_->penDown(!state);}
  void left(double dist){check("left"); _PsystemTurtle_->left(dist);}
  void left(int dist){check("left"); left(float(dist));}
  void right(double dist){check("right"); _PsystemTurtle_->right(dist);}
  void right(int dist){check("right"); right(float(dist));}
  void forward(double dist){check("forward"); _PsystemTurtle_->forward(dist);}
  void forward(int dist){check("forward"); forward(float(dist));}

  double sine(double x){return sin(PI*x/180);}
  double cosine(double x){return cos(PI*x/180);}
  double tangent(double x){return tan(PI*x/180);}
  double arcsine(double x){return asin(x)*180/PI;}
  double arccosine(double x){return acos(x)*180/PI;}
  double arctangent(double x){return atan(x)*180/PI;}
  double arctangent2(double y, double x){return atan2(y,x)*180/PI;}


}
