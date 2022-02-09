#include <simplecpp>

double pts_body[4][2] = {{-50, -50}, {-50, 50}, {50, 50}, {50, -50}};

void trans(Polygon &s, float angle, int step) {
    //  s.forward(10);
    s.rotate(step * PI / 180);
    angle = min(angle, 90 - angle);

    s.setScale(cos(angle * PI / 180), false);
}

void rotSquares() {
    const int canvasw = 800, canvash = 800;
    initCanvas("Rotating rectangles", canvasw, canvash);

    beginFrame();
    const int m = 5, n = 5, sqW = 100;
    Rectangle r[m][n];

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            r[i][j].reset(canvasw / 2 - m / 2 * sqW + i * sqW,
                          canvash / 2 - n / 2 * sqW + j * sqW, sqW, sqW);

            r[i][j].setColor((i + j) % 2 == 0 ? COLOR("blue") : COLOR("red"));
            r[i][j].setFill();
        }
    endFrame();

    int step = 1, angle = 0;

    for (int i = 0; i < 360; i++) {
        angle = (angle + step) % 90;
        beginFrame();
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                trans(r[i][j], angle, step);
        endFrame();
    }
}
