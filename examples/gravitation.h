#include "V3.h"
#include "star.h"
#include <assert.h>
#include <fstream>
#include <simplecpp>

void read_star_data(Star stars[], int n, ifstream &infile) {
    double mass, vx, vy, vz, x, y, z, rad;
    string clr;
    for (int i = 0; i < n; i++) {
        infile >> mass >> x >> y >> z >> vx >> vy >> vz >> clr >> rad;
        stars[i].init(mass, V3(x, y, z), V3(vx, vy, vz), clr, rad);
    }
    assert(infile); // quick check that input was valid
}

void calculate_net_force(int n, Star stars[], V3 forces[]) {
    for (int i = 0; i < n; i++)
        forces[i] = V3(0, 0, 0);

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            V3 f = stars[i].forceOf(stars[j]);
            forces[i] = forces[i] + f;
            forces[j] = forces[j] - f;
        }
    }
}

void gravitation0(const char *fname) {
    ifstream infile(fname);
    double T, delta;
    infile >> T >> delta;
    int n;
    infile >> n;
    Star stars[n];
    read_star_data(stars, n, infile);

    V3 forces[n];

    calculate_net_force(n, stars, forces);
    for (int i = 0; i < n; i++)
        stars[i].vStep(delta / 2, forces[i]);

    for (float t = 0; t < T; t += delta) {
        beginFrame();
        for (int i = 0; i < n; i++)
            stars[i].rStep(delta);
        calculate_net_force(n, stars, forces);
        for (int i = 0; i < n; i++)
            stars[i].vStep(delta, forces[i]);
        wait(0.05);
        endFrame();
    }
    wait(1);
}
