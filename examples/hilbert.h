#include <simplecpp>

void hilbert(float dist, int n, int parity)
// moves forward drawing a curve to left if parity = 1, right if -1.
{
    left(90 * parity);
    if (n > 0)
        hilbert(dist, n - 1, -parity);
    forward(dist);
    right(90 * parity);
    if (n > 0)
        hilbert(dist, n - 1, parity);
    forward(dist);
    if (n > 0)
        hilbert(dist, n - 1, parity);
    right(90 * parity);
    forward(dist);
    if (n > 0)
        hilbert(dist, n - 1, -parity);
    left(90 * parity);
}

void hilbert0() {

    penDown(false);
    forward(-400);
    penDown(true);

    hilbert(20, 0, 1);
    penDown(false);
    forward(100);
    penDown(true);
    hilbert(20, 1, 1);
    penDown(false);
    forward(100);
    penDown(true);
    hilbert(20, 2, 1);
    penDown(false);
    forward(100);
    penDown(true);
    hilbert(20, 3, 1);

    wait(1);

    return;
}
