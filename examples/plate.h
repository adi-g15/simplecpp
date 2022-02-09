#include <simplecpp>

void plate() {
    repeat(36) {
        forward(30);
        repeat(13) {
            beginFrame();
            right(20);
            forward(2);
            endFrame();
        }
        forward(30);
        repeat(9) {
            beginFrame();
            right(-30);
            forward(3);
            endFrame();
        }
        //    right(-90); forward(5); right(-90); forward(5); right(-90);
    }
}
