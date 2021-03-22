#include <simplecpp>

namespace simplecpp{

float Sprite::max_z_index;

Sprite::Sprite(Composite* owner) {
  init(owner);
}

Sprite::Sprite(double dx, double dy, Composite* owner) 
  : pose(dx,dy) {
  init(owner);
}
void Sprite::reset(double dx, double dy, Composite* owner){
  pose = Pose(dx,dy);
  repaint();
}
Sprite& Sprite::operator=(const Sprite& other){
  hide();
  z_index = other.z_index; // should z_index change?  also on copy const.
  pose = other.pose;
  visible = other.visible;
  color = other.color;
  fill = other.fill;
  penIsDown = other.penIsDown;
  return *this;
}


Sprite::Sprite(const Sprite& other):
  z_index(other.z_index), pose(other.pose), visible(other.visible),
  color(other.color), fill(other.fill), penIsDown(other.penIsDown)
{
  //if(owner != NULL) owner->addPart(this);
  //else               // will not work with Composite.
  addSprite(this);     // Register to canvas
}  

void Sprite::init(Composite* owner){
  visible = true;
  color = COLOR("black");
  fill = false;
  penIsDown = false;
  z_index = ++ max_z_index;
  if(owner != NULL) owner->addPart(this);
  else addSprite(this);     // Register to canvas
}  

Sprite::~Sprite(){
  removeSprite(this);
}

void Sprite::show(){
  visible = true;
  repaint();
}

void Sprite::hide(){
  visible = false;
  repaint();
}

void Sprite::rotate(double angle){
  pose = Pose(pose, Pose(angle));  // rotate in its frame of reference
  repaint();  
}

void Sprite::setScale(double factor, bool repaintP){
  pose.setScale(factor);
  if(repaintP) repaint();
}

void Sprite::scale(double factor, bool repaintP){
  pose = Pose(pose,Pose(factor,true));
  if(repaintP) repaint();
}

void Sprite::moveTo(double x, double y, bool repaintP){
  Position oldorigin = pose.getOrigin();
  move(x-oldorigin.getX(), y-oldorigin.getY(), repaintP);
}

void Sprite::move(double xshift, double yshift, bool repaintP){
  Position oldorigin = pose.getOrigin();
  pose = Pose(Pose(xshift,yshift),pose);  // move in parent's frame

  if(penIsDown){
    Line l(oldorigin.x, oldorigin.y, 
	 oldorigin.x+xshift, oldorigin.y+yshift);
    //l.setColor(COLOR("green"));
    l.imprint(false);
  }
  if(repaintP) repaint();
}

void Sprite::setFill(bool v, bool repaintP){
  fill = v;
  if(repaintP) repaint();
}

Sprite & Sprite::setColor(Color c, bool repaintP){
  color = c;
  if(repaintP) repaint();
  return *this;
}


void Sprite::imprint(bool repaintP){
  c_imprint(this);
  if(repaintP) repaint();
}  

void Sprite::forward(double distance){
  Position image  = pose.unitX();
  double xshift = image.getX()*distance;
  double yshift = image.getY()*distance;
  move(xshift,yshift);
}

void Sprite::penDown(bool down){
  penIsDown = down;
}

void Sprite::penUp(bool up){
  penIsDown = !up;
}

void Sprite::face(double x, double y, bool repaintP){
  Position image  = pose.unitX();
  double angle = atan2(image.getY(), image.getX());
  Position origin = getOrigin();
  double target = atan2(y-origin.getY(), x-origin.getX());
  rotate((target - angle));
}
void Sprite::face(Sprite &s){
  Position org = s.getOrigin();
  double x = org.getX();
  double y = org.getY();
  face(x,y);
}

void Sprite::setZIndex(float new_z_index){
  beginFrame();
  removeSprite(this);
  z_index = new_z_index;
  addSprite(this);
  endFrame();
}

float Sprite::getZIndex() const{
  return z_index;
}

Position Sprite::getAtan2() const{
  return pose.getAtan2();
}
}
