#ifndef _SPRITEINTERFACE_INCLUDED_
#define _SPRITEINTERFACE_INCLUDED_

#include <common_def.h>
#include <pose.h>

namespace simplecpp{

class Composite;
class Sprite; 

class SpriteInterface{
 public:
  //  Sprite& operator=(const Sprite&){cout << "Copying.\n"; return *this;}
  void reset(double dx, double dy, Composite* owner = NULL);

  virtual void forward(double distance)=0;
  virtual void penDown(bool=true)=0;
  virtual void penUp(bool=true)=0;
  virtual void left(double Dangle)=0;
  virtual void right(double Dangle)=0;


  virtual void paint(Pose *p=NULL)=0;
  virtual void show()=0;
  virtual void hide()=0;
  virtual void rotate(double angle)=0;
  virtual void face(Sprite &s)=0;
  virtual void face(double x, double y, bool repaintP=true)=0;
  virtual void setScale(double factor, bool repaintP=true)=0;
  virtual void scale(double factor, bool repaintP=true)=0;
  virtual void move(double xshift, double yshift, bool repaintP=true)=0;
  virtual void moveTo(double x, double y, bool repaintP=true)=0;
  virtual void print(string msg="")=0;
  void setColor(Color c, bool repaintP=true);
  void setFill(bool v=true, bool repaintP=true);
  void imprint(bool repaintP=true);
  virtual Position getOrigin() const=0;
  virtual void setZIndex(float new_z_index)=0;
  virtual float getZIndex() const=0;
  virtual Position getAtan2() const=0;
  virtual double getOrientation() const=0;
};
}
#endif
