#include <simplecpp>
#include "bounce.h"
#include "plate.h"
#include "hilbert.h"
#include "bintree.h"
#include "car.h"
#include "plane.h"
#include "gravitation.h"
#include "realtree.h"

struct erase {
    static void spiral(Color clr = COLOR("white")) {
        Rectangle r(canvas_width() / 2, canvas_height() / 2, 0, 0);
        r.setFill();
        r.setColor(clr);
        for (int i = 1; i <= 72; i++) {
            beginFrame();
            r.reset(canvas_width() / 2.0, canvas_height() / 2.0,
                    canvas_width() / 72.0 * i, canvas_height() / 72.0 * i);
            r.right(i * 5);
            wait(0.05);
            endFrame();
        }
        r.setColor(COLOR("white"));
        r.imprint();
    }
    static void bars(Color clr = COLOR("white")) {
        Rectangle r[10];
        for (int i = 0; i < 10; i++) {
            r[i].setFill();
            r[i].setColor(clr);
        }
        for (int t = 0; t <= 10; t++) {
            beginFrame();
            for (int i = 0; i < 10; i++)
                r[i].reset(canvas_width() / 2.0,
                           canvas_height() / 10.0 * (i + 0.5), canvas_width(),
                           canvas_height() / 10.0 / 10 * t);
            wait(0.1);
            endFrame();
        }
        for (int i = 0; i < 10; i++) {
            r[i].setColor(COLOR("white"));
            r[i].imprint();
        }
    }
    static void squeeze(Color clr = COLOR("white")) {
        Rectangle r[2];

        for (int i = 0; i < 2; i++) {
            r[i].setFill();
            r[i].setColor(clr);
        }

        for (int i = 0; i < 46; i++) {
            beginFrame();
            r[0].reset(canvas_width() * cosine(i) + canvas_height() * sine(i),
                       canvas_width() * sine(i) - canvas_height() * cosine(i),
                       3 * canvas_width(), 2 * canvas_height());
            r[0].right(i);

            r[1].reset(canvas_width() - canvas_height() * sine(i),
                       canvas_height() * (1 + cosine(i)), 3 * canvas_width(),
                       2 * canvas_height());
            r[1].right(i);
            endFrame();
            wait(0.03);
        }
        beginFrame();
        for (int i = 0; i < 2; i++) {
            r[i].setColor(COLOR("white"));
            r[i].imprint();
        }
        endFrame();
    }

    static void droplets(Color clr) {
        const int dropD = 100;
        int m = canvas_width() / dropD, n = canvas_height() / dropD;
        Circle c[m][n];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                c[i][j].setFill();
                c[i][j].setColor(clr);
            }

        for (int t = 0; t < dropD / sqrt(2.0) + 2; t++) {
            beginFrame();
            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                    c[i][j].reset((i + 0.5) * dropD, (j + 0.5) * dropD, t);
            wait(0.02);
            endFrame();
        }
        beginFrame();
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                c[i][j].setColor(COLOR("white"));
                c[i][j].imprint();
            }
        endFrame();
    }
};

int main(int argc, char **argv) {

    turtleSim("Raagmaalaa", 1000, 800); // big canvas

    while (true) {

        resetTurtle();
        penDown();
        wait(0.3);

        plate();

        erase::bars(COLOR("yellow"));

        resetTurtle();
        hilbert0();
        wait(0.3);
        hide();

        erase::spiral(COLOR("pink"));

        realtree();

        wait(0.3);
        erase::squeeze(COLOR("green"));

        tree(6, canvas_width() / 2.0, 25, canvas_height() - 100,
             canvas_width());

        wait(0.3);
        // penUp(); forward(5000);
        erase::droplets(COLOR("blue"));

        car0();

        erase::bars(COLOR("gray"));

        bounce0();

        erase::spiral(COLOR("red"));

        airport0("arrivals.txt");

        erase::squeeze(COLOR("purple"));

        gravitation0("sEm.txt");

        break;
    }
}
