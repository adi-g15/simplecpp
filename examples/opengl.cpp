#include "opengl.hpp"
#include <thread>

int main () {
    // turtleSim();
    OpenGL_Win win;

    auto game_loop = std::thread(&OpenGL_Win::init, &win, "Something", 800, 800);
    std::thread([&win]() {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Changing color" << std::endl;
        win.change_color({ 0.0f, 1.0f, 0.0f, 1.0f });
        }).detach();

    game_loop.join();
    //win.init();
}
