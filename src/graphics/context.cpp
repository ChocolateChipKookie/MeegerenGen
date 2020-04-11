//
// Created by kookie on 27/03/2020.
//

#include "context.h"
#include "util/graphics_error.h"

//
//  Constructors, destructors and default callback functions
//

meg::context::context(bool alpha, int width, int height, const std::string &name) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, "", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        throw graphics_error{ "Failed to create GLFW window" };
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw graphics_error{ "Failed to create GLFW window" };
    }

    if (alpha)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    this->context_ = window;
}

void meg::context::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void meg::context::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

meg::context::context(int width, int height, const std::string &name, bool alpha) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        throw graphics_error{ "Failed to create GLFW window" };
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw graphics_error{ "Failed to create GLFW window" };
    }

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHTING);

    if (alpha)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    this->context_ = window;
}

meg::context::~context()
{
    glfwDestroyWindow(context_);
}

//
//  Handling functions
//

bool meg::context::should_close() const
{
    return glfwWindowShouldClose(context_);
}

void meg::context::set_should_close(bool value) const
{
    glfwSetWindowShouldClose(context_, value);
}

void meg::context::swap_buffers() const
{
    glfwSwapBuffers(context_);
}

void meg::context::poll_events()
{
    glfwPollEvents();
}

void meg::context::wait_events(double seconds)
{
    if(seconds < 0.){
        glfwWaitEvents();
    }
    else{
        glfwWaitEventsTimeout(seconds);
    }
}

void meg::context::set_viewport() const
{
    int width, height;
    glfwGetWindowSize(context_, &width, &height);
    glViewport(0, 0, width, height);
}

void meg::context::resize(int width, int height) const
{
    glfwSetWindowSize(context_, width, height);
}

unsigned meg::context::get_width() const
{
    int width, height;
    glfwGetWindowSize(context_, &width, &height);
    return width;
}

unsigned meg::context::get_height() const
{
    int width, height;
    glfwGetWindowSize(context_, &width, &height);
    return height;
}

void meg::context::set_process_input(void(*function)(GLFWwindow*, int, int, int, int)) const
{
    glfwSetKeyCallback(context_, function);
}

void meg::context::set_resize_callback(void(*function)(GLFWwindow*, int, int)) const {
    glfwSetFramebufferSizeCallback(context_, function);
}

void meg::context::bind()
{
    glfwMakeContextCurrent(context_);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



