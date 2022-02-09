#include <simplecpp>

const double RADIUS = 50;
class Wheel : public Composite {
    Circle *rim;
    Line *spoke[10];

  public:
    Wheel(double x, double y, Composite *owner = NULL)
        : Composite(x, y, owner) {
        rim = new Circle(0, 0, RADIUS, this);
        for (int i = 0; i < 10; i++) {
            spoke[i] = new Line(0, 0, RADIUS * cos(i * PI / 5),
                                RADIUS * sin(i * PI / 5), this);
        }
    }
};

class Car : public Composite {
    Polygon *body;
    Wheel *w1;
    Wheel *w2;

  public:
    Car(double x, double y, Color c, Composite *owner = NULL)
        : Composite(x, y, owner) {
        double bodyV[9][2] = {{-150, 0},   {-150, -100}, {-100, -100},
                              {-75, -200}, {50, -200},   {100, -100},
                              {150, -100}, {150, 0},     {-150, 0}};
        body = new Polygon(0, 0, bodyV, 9, this);
        body->setColor(c);
        body->setFill();
        w1 = new Wheel(-90, 0, this);
        w2 = new Wheel(90, 0, this);
    }
    void forward(double dx) {
        Composite::forward(dx);                 // superclass forward function
        w1->rotate(dx / (RADIUS * getScale())); // angle = dx/current-RADIUS
        w2->rotate(dx / (RADIUS * getScale()));
    }
};

void car0() {

    beginFrame();
    Car c(100, 150, COLOR("blue")), d(900, 700, COLOR("red"));
    d.scale(0.5);
    c.scale(0.7);
    endFrame();

    for (int i = 0; i < 80; i++) {
        beginFrame();
        c.forward(5);
        d.forward(-5);
        endFrame();
        wait(0.05);
    }

    for (int i = 0; i < 360; i++) {
        beginFrame();
        c.forward(4);
        d.forward(-4.8);
        c.left(-1);
        d.left(-1);
        endFrame();
        wait(0.05);
    }

    for (int i = 0; i < 80; i++) {
        beginFrame();
        c.forward(5);
        d.forward(-5);
        endFrame();
        wait(0.05);
    }
}
