#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>

inline void GLFWErrorCallback(int err_num, const char *err_desc)
{
    std::cerr << err_desc << std::endl;
}

class OpenGL_Win
{
    GLFWwindow *mWindow;
    std::thread mGameLoop;
    bool m_opengl_initialised = false;

public:
    static bool already_destructed;
    void start_game_loop()
    {
        std::cout << "Start loop init" << std::endl;
        // glClearColor()
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        std::cout << "After clearColor" << std::endl;
        while (!glfwWindowShouldClose(mWindow))
        {
            std::cout << "Inside while" << std::endl;
            // glClear(GL_COLOR_BUFFER_BIT);
        std::cout << "After glClear" << std::endl;
            glfwSwapBuffers(mWindow);
        std::cout << "After glfwSwapBuffers" << std::endl;
            glfwPollEvents();
        std::cout << "After glfwPollEvents" << std::endl;
        }
        std::cout << "Ended game loop" << std::endl;
    }

    void close_window()
    {
        glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
    }

    bool init(const char *name = "Turtle Simulator", int w = 500, int h = 500)
    {
        std::cout << "Start init" << std::endl;
        glfwSetErrorCallback(GLFWErrorCallback);
        if (!glfwInit())
        {
            std::cerr << "Couldn't init glfw" << std::endl;
            return false;
        }

        std::cout << "Start init 2" << std::endl;
        mWindow = glfwCreateWindow(800, 800, "Opengl test", nullptr, nullptr);
        std::cout << "Start init 3" << std::endl;

        if (!mWindow)
        {
            std::cerr << "Couldn't create a window" << std::endl;
            glfwTerminate();
            return false;
        }
        std::cout << "Start init 4" << std::endl;

        m_opengl_initialised = true;

        gladLoadGL();

        std::cout << "Start init 5 -> " << this << std::endl;
        mGameLoop = std::thread(&OpenGL_Win::start_game_loop, this);

        std::cout << "init done" << std::endl;
        return true;
    }

    OpenGL_Win() {}
    ~OpenGL_Win()
    {
        if(OpenGL_Win::already_destructed)  return;

        OpenGL_Win::already_destructed = true;
        if (m_opengl_initialised)
        {
            mGameLoop.join();
            glfwDestroyWindow(mWindow);
            glfwTerminate();
        }
    }
};
