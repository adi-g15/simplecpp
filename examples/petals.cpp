#include <simplecpp>

void petals() {

    const double sw = 1000, sh = 800;
    const double petals = 16, theta = PI / petals, angle = 2 * (theta + PI / 2);
    const int n = 35, whorls = 10;

    double stepL = 1;

    beginFrame(); // calculate ratio of petal sizes in successive whorls
    Turtle t;
    t.hide();
    t.penUp();

    double x0 = t.getX(), y0 = t.getY();

    repeat(n / 2 + 1) { // Draw half a petal
        t.forward(stepL);
        t.rotate(-angle / (n));
    }
    t.rotate(angle / n / 2 + PI / 2);

    double x1 = t.getX(), y1 = t.getY();
    double dx = (y0 - y1) / tan(theta);
    double r0 = x0 - (x1 - dx), r1 = (y0 - y1) / sin(theta);
    double pRatio = r1 / r0; // petal size ratio

    t.moveTo(sw / 2 + r0, sh / 2); // move turtle back to origin etc.
    t.rotate(theta);
    t.penDown();
    endFrame();

    repeat(whorls) {
        repeat(petals) {
            repeat(n) {
                beginFrame();
                t.forward(stepL);
                t.rotate(-angle / (n));
                endFrame();
            }
            t.rotate(PI);
        }
        repeat(n / 2 + 1) {
            t.forward(stepL);
            t.rotate(-angle / n);
        }
        t.rotate(angle / n / 2 + PI / 2);
        stepL *= pRatio;
    }
    t.hide();
}

int main() {
  petals();

  getClick();
}
