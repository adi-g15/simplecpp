#include <simplecpp>

void bounce0() {
    double x1 = 100, x2 = 700, y1 = 100, y2 = 700;
    double x = 630, y = 200, vx = 3, vy = 5, radius = 30;

    Rectangle r((x1 + x2) / 2, (y1 + y2) / 2, x2 - x1 + 2 * radius,
                y2 - y1 + 2 * radius);

    Circle ball(x, y, radius);
    ball.penDown(true);
    ball.setFill();
    ball.setColor(COLOR("blue"));

    for (int i = 0; i < 20; i++) {
        double vwalltime, hwalltime, time;
        if (vx > 0)
            vwalltime = (x2 - x) / vx;
        else
            vwalltime = (x1 - x) / vx;

        if (vy > 0)
            hwalltime = (y2 - y) / vy;
        else
            hwalltime = (y1 - y) / vy;

        time = min(hwalltime, vwalltime);

        int t = time;
        for (int j = 0; j < t; j++) {
            beginFrame();
            ball.move(vx, vy);
            endFrame();
        }
        ball.move(vx * (time - t), vy * (time - t));
        x += vx * time;
        y += vy * time;

        if (x == x1 || x == x2)
            vx = -vx;
        if (y == y1 || y == y2)
            vy = -vy;
    }
}
