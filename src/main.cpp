#include <string>
#include <cstring>
#include <unistd.h>
#include "include/window.hpp"
#include "include/shader.hpp"
#include "include/mesh.hpp"
#include <iostream>
#include "include/rigid_body.hpp"

int main(int argc, char* argv[])
{
    bool primus_relaunch = false;
    for (int i = 0; i < argc; ++i)
    {
        if (!strcmp(argv[i], "--primus"))
        {
            primus_relaunch = true;
        }
    }

    if (primus_relaunch)
    {
        std::string launch_cmd = "";
        launch_cmd += "primusrun ";
        for (int i = 0; i < argc; ++i)
        {
            if (strcmp(argv[i], "--primus"))
            {
                launch_cmd += argv[i];
                launch_cmd += ' ';
            }
        }
        return system(launch_cmd.c_str());
    }
    window wnd;
    wnd.size(1280, 720);
    wnd.title("Ominous owl");
    wnd.open();
    shader shd;
    shd.attachf(GL_VERTEX_SHADER, "data/shaders/default.vert.glsl");
    shd.attachf(GL_FRAGMENT_SHADER, "data/shaders/default.frag.glsl");
    shd.link();
    shd.use();
    glUniform3f(shd.uniform_location("ldir0"), 0.606642824f, 0.198277918f, -0.769850863f);
    glUniform1f(shd.uniform_location("lint0"), 1.0f);
    glUniform3f(shd.uniform_location("ldir1"), -0.530619152f, 0.277329051f, 0.800956874f);
    glUniform1f(shd.uniform_location("lint1"), 0.75f);
    glUniform3f(shd.uniform_location("ldir2"), -0.80483495f, -0.409118391f, -0.429956795f);
    glUniform1f(shd.uniform_location("lint2"), 0.5f);
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glPointSize(5.0f);
    std::vector<rigid_body> bodies;
    bodies.reserve(8);
    ///bodies.push_back({{1, 2, 1}, {0, 4, 0}, 5.0f});/*
    float scale = 50.0f;
    bodies.push_back({{0.1, 0.3, 0.2}, {0, 4, 0}, 50e-3f*scale*scale*scale});
    bodies.push_back({{25, 1, 8}, {0, 0, 0}, IMMOBILE});
    /*bodies.push_back({{5, 5, 0.2}, {5, 5, -5}, IMMOBILE});
    bodies.push_back({{5, 5, 0.2}, {5, 5, 5}, IMMOBILE});
    bodies.push_back({{0.2, 5, 5}, {0, 5, 0}, IMMOBILE});
    bodies.push_back({{0.2, 5, 5}, {10, 5, 0}, IMMOBILE});*/
    /*for (int i = 0; i < 80; ++i)
    {
        bodies.push_back({{0.006f*scale/2.f, 0.042f*scale/2.f, 0.023f*scale/2.f}, {5, 3+i, 0}, 4.25e-3f*scale*scale*scale});
    }*/
    vec3 brick = vec3(0.006f*scale/2.f, 0.042f*scale/2.f, 0.023f*scale/2.f);
    vec3 fbrick = vec3(brick.y, brick.x, brick.z);
    float mass = 4.25e-3f*scale*scale*scale;
    vec3 bo = 2.0f*brick;

    bodies.push_back({brick, {bo.y, bo.y, 0}, mass});
    bodies.push_back({brick, {2*bo.y-bo.x, bo.y, 0}, mass});
    bodies.push_back({fbrick, {1.5*bo.y-0.5*bo.x, 2*bo.y+2*fbrick.y, 0}, mass});

    bodies.push_back({brick, {bo.y+1.5*bo.y, bo.y, 0}, mass});
    bodies.push_back({brick, {2*bo.y-bo.x+1.5*bo.y, bo.y, 0}, mass});
    bodies.push_back({fbrick, {1.5*bo.y-0.5*bo.x+1.5*bo.y, 2*bo.y+2*fbrick.y, 0}, mass});

    bodies.push_back({brick, {bo.y+0.75*bo.y, bo.y+1.5*(bo.y+bo.x), 0}, mass});
    bodies.push_back({brick, {2*bo.y-bo.x+0.75*bo.y, bo.y+1.5*(bo.y+bo.x), 0}, mass});
    bodies.push_back({fbrick, {1.5*bo.y-0.5*bo.x+0.75*bo.y, 2*bo.y+2*fbrick.y+1.5*(bo.y+bo.x), 0}, mass});

/*
    bodies.push_back({{0.01f*scale/2.f, 0.05f*scale/2.f, 0.016f*scale/2.f}, {1 + 2 * scale * 0.015, 3, -0.4}, 4.1666667e-3f*scale*scale*scale});
    bodies.push_back({{0.01f*scale/2.f, 0.05f*scale/2.f, 0.016f*scale/2.f}, {1 + 5 * scale * 0.015, 3, -0.4}, 4.1666667e-3f*scale*scale*scale});
    bodies.push_back({{0.01f*scale/2.f, 0.05f*scale/2.f, 0.016f*scale/2.f}, {1 + 7 * scale * 0.015, 3, -0.4}, 4.1666667e-3f*scale*scale*scale});
    bodies.push_back({{0.01f*scale/2.f, 0.05f*scale/2.f, 0.016f*scale/2.f}, {1 + 10 * scale * 0.015, 3, -0.4}, 4.1666667e-3f*scale*scale*scale});
    bodies.push_back({{0.05f*scale/2.f, 0.01f*scale/2.f, 0.016f*scale/2.f}, {1 + 8.5 * scale * 0.015, 6, -0.4}, 4.1666667e-3f*scale*scale*scale});
    bodies.push_back({{0.05f*scale/2.f, 0.01f*scale/2.f, 0.016f*scale/2.f}, {1 + 3.5 * scale * 0.015, 6, -0.4}, 4.1666667e-3f*scale*scale*scale});
    //bodies.push_back({{0.01f*scale/2.f, 0.05f*scale/2.f, 0.016f*scale/2.f}, {1 + 3.5 * scale * 0.015, 8, -0.4}, 4.1666667e-3f*scale*scale*scale});
    //bodies.push_back({{0.01f*scale/2.f, 0.05f*scale/2.f, 0.016f*scale/2.f}, {1 + 8.5 * scale * 0.015, 8, -0.4}, 4.1666667e-3f*scale*scale*scale});
    bodies.push_back({{0.05f*scale/2.f, 0.01f*scale/2.f, 0.016f*scale/2.f}, {1 + 6 * scale * 0.015, 9, -0.4}, 4.1666667e-3f*scale*scale*scale});
*/


    //bodies.push_back({{0.2, 1.3, 0.8}, {0.2, 20, -0.4}, 1.0f});
    // bodies.push_back({{1, 1.3, 1.2}, {0.2, 15, -0.4}, 5.0f});//*/
    vec3 epos{5, 5, 10};
    double ptime = glfwGetTime();
    double ntime = glfwGetTime();
    float dt = 0.0f;
    dvec2 mousepos;
    wnd.center_cursor();
    wnd.cursorpos(mousepos.x, mousepos.y);
    vec2 rotation = {M_PI, 0.0f};
    bool tabdown = false;
    size_t tab_index = 0;
    //glUniform4f(shd.uniform_location("object_colour"),
    //            0.95f, 1.0f, 0.8f, 1.0f);
    glUniform4f(shd.uniform_location("object_colour"),
                1.0f, 1.0f, 1.0f, 1.0f);
    bool simulate = true;

    bool ssf = false;
    int thisisacounter = 0;
    while (wnd() && !wnd.keydown(GLFW_KEY_ESCAPE))
    {
        /*if (thisisacounter < 600)
        {
            if (thisisacounter%3 == 0)
            {
                bodies.push_back({{0.042f*scale/2.f, 0.006f*scale/2.f, 0.023f*scale/2.f}, {5, 8, 0}, 4.25e-3f*scale*scale*scale});
            }
            ++thisisacounter;
        }*/
        dvec2 ocp = mousepos;
        wnd.cursorpos(mousepos.x, mousepos.y);
        ocp -= mousepos;
        rotation.x += 0.0025f * ocp.x;
        rotation.y += 0.0025f * ocp.y;
        if (rotation.y > M_PI_2-0.00001f) rotation.y = M_PI_2-0.00001f;
        if (rotation.y < -M_PI_2+0.00001f) rotation.y = -M_PI_2+0.00001f;

        wnd.center_cursor();
        wnd.cursorpos(mousepos.x, mousepos.y);
        vec3 direction
        {
            cos(rotation.y) * sin(rotation.x),
            sin(rotation.y),
            cos(rotation.y) * cos(rotation.x)
        };

        vec3 right
        {
            sin(rotation.x - M_PI_2),
            0.0f,
            cos(rotation.x - M_PI_2)
        };
        float speed = 5.0f;
        if (wnd.keydown(GLFW_KEY_SPACE))
        {
            speed *= 0.025f;
        }
        if (wnd.keydown(GLFW_KEY_I))
        {
            epos += vec3(sin(rotation[0]), 0.0, cos(rotation[0]))
                         * dt * speed;
        }
        if (wnd.keydown(GLFW_KEY_K))
        {
            epos -= vec3(sin(rotation[0]), 0.0, cos(rotation[0]))
                         * dt * speed;
        }
        if (wnd.keydown(GLFW_KEY_L))
        {
            epos += right * dt * speed;
        }
        if (wnd.keydown(GLFW_KEY_J))
        {
            epos -= right * dt * speed;
        }
        if (wnd.keydown(GLFW_KEY_U))
        {
            epos.y += speed * dt;
        }
        if (wnd.keydown(GLFW_KEY_O))
        {
            epos.y -= speed * dt;
        }

        float aspect = wnd.size().x / (float) wnd.size().y;
        mat4 wv = glm::lookAt(epos, epos + direction, {0, 1, 0});
        wv = glm::perspective(1.0f, aspect, 0.1f, 100.0f) * wv;
        glUniformMatrix4fv(shd.uniform_location("model_camera"), 1, GL_FALSE, &wv[0][0]);
        glClearColor(0xA7 / 255.f, 0xC4 / 255.f, 0xDA / 255.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (!tabdown && wnd.keydown(GLFW_KEY_TAB))
        {
            tabdown = true;
            ++tab_index;
            if (tab_index >= bodies.size())
            {
                tab_index = 0;
            }
        }
        else if (!wnd.keydown(GLFW_KEY_TAB)) tabdown = false;
        if (wnd.keydown(GLFW_KEY_PAUSE) || wnd.keydown(GLFW_KEY_ENTER))
        {
            std::cout << "Warning, stopping simulation!" << std::endl;
            simulate = false;
        }
        if (wnd.keydown(GLFW_KEY_BACKSPACE))
        {
            std::cout << "Warning, resuming simulation!" << std::endl;
            simulate = true;
        }
        if (simulate || (!ssf && (ssf = wnd.keydown(GLFW_KEY_0))))
        {
            dengo:
            try
            {
                bodies[tab_index].input(wnd);
                int iterations = 1;
                for (size_t r = 0; r < iterations; ++r)
                {
                    for (size_t i = 0; i < bodies.size(); ++i)
                    {
                        for (size_t j = i; j < bodies.size(); ++j)
                        {
                            if (i == j) continue;
                            bodies[i].intersect(bodies[j]);
                        }
                    }
                }
            }
            catch(...)
            {
                std::cerr << "An exception occured while simulating, "
                              "simulation halted to allow for visual "
                              "inspection (Simulation can be continued "
                              "by pressing <BACKSPACE> but it might be very "
                              "unstable.)" << std::endl;
                simulate = false;
            }
            int cntr = 0;
                for (auto& body : bodies)
                {
                    ///std::cout << "Updating body #" << cntr++ << std::endl;
                    //body.update(0.002f);
                    body.update(0.0005f);
                }
            if (glfwGetTime() - ntime < 0.017f) goto dengo;
        }
        if (ssf) ssf = wnd.keydown(GLFW_KEY_0);
        for (auto& body : bodies)
        {
            body.render(shd.uniform_location("model_world"),
                        shd.uniform_location("model_camera"),
                        wv);
        }

        wnd.swapbuffers();
        ptime = ntime;
        ntime = glfwGetTime();
        dt = double(ntime - ptime);
    }

    return 0;
}
