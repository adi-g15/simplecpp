#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <array>

inline void GLFWErrorCallback(int err_num, const char *err_desc)
{
    std::cerr << err_desc << std::endl;
}
class OpenGL_Win
{
    GLFWwindow *mWindow;
    std::thread mGameLoop;
    std::array<GLfloat, 4> colors{ 1.0f, 0.0f, 0.0f, 1.0f };
    bool m_opengl_initialised = false;

public:
    static bool already_destructed;
    void change_color(const std::array<GLfloat, 4>& arr) {
        colors = arr;
    }
    void start_game_loop()
    {
        std::cout << "Start loop init" << std::endl;
        std::cout << "After clearColor" << std::endl;
        while (!glfwWindowShouldClose(mWindow))
        {
        	glClearColor(colors[0], colors[1], colors[2], colors[3]);
	        glClear(GL_COLOR_BUFFER_BIT);
            // std::cout << "Inside while" << std::endl;
            // glClear(GL_COLOR_BUFFER_BIT);
            // std::cout << "After glClear: " << mWindow << std::endl;
            glfwSwapBuffers(mWindow);
            // std::cout << "After glfwSwapBuffers: " << mWindow << std::endl;
            glfwPollEvents();
            // std::cout << "After glfwPollEvents" << std::endl;
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
        glfwMakeContextCurrent(mWindow);
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
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        std::cout << "Start init 5 -> " << this << std::endl;
        while (!glfwWindowShouldClose(mWindow))
        {
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            // std::cout << "Inside while" << std::endl;
            // glClear(GL_COLOR_BUFFER_BIT);
            // std::cout << "After glClear: " << mWindow << std::endl;
            glfwSwapBuffers(mWindow);
            // std::cout << "After glfwSwapBuffers: " << mWindow << std::endl;
            glfwPollEvents();
            // std::cout << "After glfwPollEvents" << std::endl;
        }
        //mGameLoop = std::thread(&OpenGL_Win::start_game_loop, this);

        std::cout << "init done" << std::endl;
        return true;
    }

    OpenGL_Win() {}
    ~OpenGL_Win()
    {
        if (OpenGL_Win::already_destructed)
            return;

        OpenGL_Win::already_destructed = true;
        if (m_opengl_initialised)
        {
            mGameLoop.join();
            glfwDestroyWindow(mWindow);
            glfwTerminate();
        }
    }
};
