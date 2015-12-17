#ifndef WINDOW_HPP_INCLUDED
#define WINDOW_HPP_INCLUDED
#include "include/opengl.hpp"
#include "include/vectormath.hpp"
#include <string>

class window
{
public:
    // Constructors, destructor and copy assignment operator
    window();
    window(const window&) = delete;
    window operator=(const window&) = delete;
    ~window();

    // Function declarations
    bool open(bool fullscreen = false);
    void swapbuffers();


    // Inline function definitions
    inline ivec2 size()
    {
        return {m_width, m_height};
    }
    inline void size(size_t width, size_t height)
    {
        m_width = width;
        m_height = height;
    }
    inline void title(const std::string& title)
    {
        m_title = title;
        if (m_open) glfwSetWindowTitle(m_window, title.c_str());
    }
    inline void resize(int width, int height)
    {
        glfwSetWindowSize(m_window, width, height);
    }
    inline bool operator()() const
    {
        return !glfwWindowShouldClose(m_window);
    }
    inline bool keydown(int key) const
    {
        return glfwGetKey(m_window, key);
    }
    inline bool mbdown(int button) const
    {
        return glfwGetMouseButton(m_window, button);
    }
    inline void framebuffersize(int& x, int& y) const
    {
        glfwGetFramebufferSize(m_window, &x, &y);
    }
    inline void cursorpos(double& x, double& y) const
    {
        glfwGetCursorPos(m_window, &x, &y);
    }
    inline void center_cursor() const
    {
        glfwSetCursorPos(m_window, m_width / 2, m_height / 2);
    }
private:
    std::string m_title = "Default window.";
    int m_width = 640;
    int m_height  = 480;
    GLFWwindow* m_window = nullptr;
    bool m_open = false;
};

#endif // WINDOW_HPP_INCLUDED
