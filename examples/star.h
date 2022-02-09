#include <simplecpp>
#include "V3.h"

class Star {
  private:
    Circle image;
    double mass;
    V3 r, v; // position, velocity
  public:
    Star(){};
    void init(double m, V3 position, V3 velocity, string clr, double rad);
    void vStep(double dT, V3 f);
    void rStep(double dT);
    void print();
    V3 forceOf(Star &s);
};

void Star::init(double m, V3 r1, V3 v1, string clr, double rad) {
    mass = m;
    r = r1;
    v = v1;
    image.reset(r.getx(), r.gety(), rad);
    image.setColor(COLOR(clr.c_str()));
    image.setColor({1,0,0} /*red*/);
    image.setFill(true);
    image.penDown();
}

void Star::vStep(double dT, V3 f) { v = v + f * (dT / mass); }

void Star::rStep(double dT) {
    V3 d = v * dT;
    image.move(d.getx(), d.gety());
    r = r + d;
}

void Star::print() {
    cout << mass << " " << image.getOrigin().getX() << " "
         << image.getOrigin().getY() << " ";
    cout << v << endl;
}

V3 Star::forceOf(Star &s) {
    V3 R = s.r - r;
    return R * (mass * s.mass / pow(R.length(), 3));
}
