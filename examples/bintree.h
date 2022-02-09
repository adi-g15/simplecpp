#include <simplecpp>

void tree(int n, double rx, double ry, double height, double width) {
    if (n > 1) {
        Line left(rx, ry, rx - width / 4, ry + height / 2);
        Line right(rx, ry, rx + width / 4, ry + height / 2);

        right.imprint();
        left.imprint();
        wait(0.1);
        tree(n - 1, rx - width / 4, ry + height / 2, height / 2, width / 2);
        wait(0.1);
        tree(n - 1, rx + width / 4, ry + height / 2, height / 2, width / 2);
    }
    Text(rx + 10, ry, n).imprint();
}
