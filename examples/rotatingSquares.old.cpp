#include <simplecpp>

double pts_body[4][2] = {{-50, -50}, {-50, 50}, {50, 50}, {50, -50}};

void trans(Polygon &s, float angle, int step) {
    //  s.forward(10);
    s.rotate(step * PI / 180);
    angle = min(angle, 90 - angle);

    s.setScale(cos(angle * PI / 180), false);
}

void rotSquares() {

    const int x1 = 550, x2 = 450, y1 = 450, y2 = 350;

    Polygon sq1(x1, y1, pts_body, 4);
    sq1.setColor(COLOR("red"));
    sq1.setFill(true);
    Polygon sq2(x2, y1, pts_body, 4);
    sq2.setColor(COLOR("blue"));
    sq2.setFill(true);
    Polygon sq3(x1, y2, pts_body, 4);
    sq3.setColor(COLOR("blue"));
    sq3.setFill(true);
    Polygon sq4(x2, y2, pts_body, 4);
    sq4.setColor(COLOR("red"));
    sq4.setFill(true);

    int step = 1;
    int angle = 0;

    for (int i = 0; i < 360; i++) {
        angle = (angle + step) % 90;
        beginFrame();
        trans(sq1, angle, step);
        trans(sq2, angle, step);
        trans(sq3, angle, step);
        trans(sq4, angle, step);
        endFrame();
    }
}

int main() {
  rotSquares();

  getClick();
}
