#include <queue>
#include <simplecpp>

struct steppable {
    virtual bool step() = 0;
    virtual ~steppable(){};
};

struct controller {
    static double time;
    static queue<steppable *> stbls;
    static void process(int n = 1);
};
double controller::time = 0;
queue<steppable *> controller::stbls;

void controller::process(int n) {
    repeat(n) {
        beginFrame();
        //    wait(0.01);
        size_t ws = stbls.size();
        repeat(ws) {
            steppable *w = stbls.front();
            stbls.pop();
            bool alive = w->step();
            if (alive)
                stbls.push(w);
            else
                delete (w);
        }
        endFrame();
        time++;
    }
}

//#include "steppable.cpp"

const double screenw = 1500, screenh = 800;

struct myt : public Turtle, public steppable {
    double length, scalef;
    int levels;
    myt(int lev, double l, double s, double x, double y) {
        penUp();
        left(90);
        setColor(COLOR(randuv(0, 255), randuv(0, 255), randuv(0, 255)));
        moveTo(x, y);
        levels = lev;
        length = l;
        scalef = s;
        setScale(scalef);
    }
    virtual bool step() {
        if (levels > 0) {
            forward(length * scalef);
            myt *L = new myt(*this);
            L->levels--;
            L->left(30);
            L->setColor(COLOR(randuv(0, 255), randuv(0, 255), randuv(0, 255)));
            right(30);
            controller::stbls.push(L);
        } else if (levels == 0) {
            double ortn = getOrientation();
            this->rotate(PI / 2 - ortn);
        } else {
            scale(0.95);
            setColor(COLOR(randuv(0, 255), randuv(0, 255), randuv(0, 255)));
            forward(10);
        }
        levels--;
        if (getX() > screenw || getX() < 0 || getY() > screenh || getY() < 0) {
            hide();
            return false;
        }
        return true;
    }
};

class Wheel : public Composite, public steppable {
    double r, vx, vy, omega;
    Circle *rim;
    Line *spoke[10];

  public:
    ~Wheel() {
        delete rim;
        for (int i = 0; i < 10; i++)
            delete spoke[i];
    }
    Wheel(double x, double y, double rr, double vxx, double vyy,
          Composite *owner = NULL)
        : Composite(x, y, owner) {
        omega = 2 * PI / 72;
        r = rr;
        vx = vxx;
        vy = vyy;
        rim = new Circle(0, 0, r, this);
        rim->setFill();

        rim->setColor(COLOR(randuv(20, 255), randuv(30, 255), randuv(0, 255)));
        for (int i = 0; i < 10; i++) {
            spoke[i] =
                new Line(0, 0, r * cos(i * PI / 5), r * sin(i * PI / 5), this);
        }
    }
    virtual bool step() {
        move(vx, vy);
        if (r > 10) {
            scale(0.95);
            r = r * 0.95;
            rotate(omega);
            double t = controller::time;
            for (int phi = 0; phi < 360; phi += 18) {
                double phirad = omega * t + phi * PI / 180;
                Wheel *w =
                    new Wheel(getX() + r * 0.95 * cos(phirad),
                              getY() + r * 0.95 * sin(phirad), 3 * randuv(1, 3),
                              vx - 5 * r * omega * sin(phirad),
                              vy + 5 * r * omega * cos(phirad));
                w->penDown();
                controller::stbls.push(w);
            }
        }
        if (getX() > screenw || getX() < 0 || getY() > screenh || getY() < 0) {
            hide();
            return false;
        }
        return true;
    }
};

void anar() {
    Rectangle r(screenw / 2, screenh / 2, screenw, screenh);
    r.setFill();
    r.setColor(COLOR("black"));

    myt *m = new myt(10, 45, 1.1, 500, screenh - 200);
    controller::stbls.push(m);

    repeat(80) {
        wait(0.08);
        controller::process();
    }
}

void chakra() {
    Rectangle r(screenw / 2, screenh / 2, screenw, screenh);
    r.setFill();
    r.setColor(COLOR("black"));

    Wheel *w = new Wheel(500, screenh / 2, 75, 5, 3);
    controller::stbls.push(w);
    repeat(100) { controller::process(); }
}
/*

    if(checkEvent(event) && mouseButtonPressEvent(event)){
      lastx = event.xbutton.x; lasty = event.xbutton.y;
      beginFrame();
      if(event.xbutton.button == 1){
      }
      else{
      }

      endFrame();
    }
  }
}
*/
