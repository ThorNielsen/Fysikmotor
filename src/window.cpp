#include "window.hpp"
#include <iostream>

namespace wnd_impl
{
    int fbwidth = 0;
    int fbheight = 0;
    void resize_framebuffer(GLFWwindow* wptr, int w, int h)
    {
        glViewport(0, 0, fbwidth = w, fbheight = h);
    }
}

window::window()
{
    if (!glfwInit())
    {
        glfwTerminate();
    }

    glfwWindowHint(GL_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

window::~window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void window::swapbuffers()
{
    if (wnd_impl::fbwidth) m_width = wnd_impl::fbwidth;
    if (wnd_impl::fbheight) m_height = wnd_impl::fbheight;
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

bool window::open(bool fullscreen)
{
    if (fullscreen)
    {
        m_window = glfwCreateWindow(m_width, m_height,
                                    m_title.c_str(),
                                    glfwGetPrimaryMonitor(),
                                    0);
    }
    else
    {
        m_window = glfwCreateWindow(m_width, m_height,
                                    m_title.c_str(), 0, 0);
    }
    if (!m_window)
    {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, wnd_impl::resize_framebuffer);

    glewExperimental = true;
    GLenum glew_error_code = glewInit();
    if (glew_error_code != GLEW_OK)
    {
        std::cerr <<  "Could not initialise GLEW:\n"
        << (char*)glewGetErrorString(glew_error_code)
        << '\n';
    }
    if (!GLEW_VERSION_3_3)
    {
        std::cerr<<"No support for OpenGL 3.3.\n"
                    "If you have switchable graphics then try to run\n"
                    "this program with your dedicated graphics card.\n";
    }
    else
    {
        std::cout << "Support for OpenGL 3.3!\n";
    }
    m_open = true;
    return true;
}
