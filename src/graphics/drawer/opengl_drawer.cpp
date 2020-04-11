//
// Created by kookie on 28/03/2020.
//

#include "opengl_drawer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#define CONTEXT_ASSERT (assert(glfwGetCurrentContext() != NULL))

meg::opengl_drawer::opengl_drawer():
        triangleShader("shaders/triangle_shader2D.sha"),
        subtractShader("shaders/subtract_shader.sha"),
        textureShader("shaders/draw_texture.sha")
{
    CONTEXT_ASSERT;

    //Convex poly
    {
        glGenVertexArrays(1, &convexPolyVAO);
        glGenBuffers(1, &convexPolyVBO);
        glBindVertexArray(convexPolyVAO);

        glBindBuffer(GL_ARRAY_BUFFER, convexPolyVBO);
        glBufferData(GL_ARRAY_BUFFER, concavePolyVertices * 2 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    //Concave poly
    {
        glGenVertexArrays(1, &concavePolyVAO);
        glGenBuffers(1, &concavePolyVBO);
        glBindVertexArray(concavePolyVAO);

        glBindBuffer(GL_ARRAY_BUFFER, concavePolyVBO);
        glBufferData(GL_ARRAY_BUFFER, concavePolyVertices * 2 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    //Triangle
    {
        glGenVertexArrays(1, &triangleVAO);
        glGenBuffers(1, &triangleVBO);
        glBindVertexArray(triangleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
        glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    //Rectangle
    {
        glGenVertexArrays(1, &rectangleVAO);
        glGenBuffers(1, &rectangleVBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(rectangleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
        glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }

    //Subtract
    {
        static float subtractVertices[] = {
                1.f,  1.f,	1.0f, 1.0f, // top right
                1.f, -1.f,	1.0f, 0.0f, // bottom right
                -1.f, -1.f,	0.0f, 0.0f, // bottom left
                -1.f,  1.f,	0.0f, 1.0f  // top left
        };

        static unsigned int subtractIndices[] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
        };
        subtractShader.use();
        subtractShader.set_int("texture1", 0);
        subtractShader.set_int("texture2", 1);

        glGenVertexArrays(1, &subtractVAO);
        glGenBuffers(1, &subtractVBO);
        glGenBuffers(1, &subtractEBO);

        glBindVertexArray(subtractVAO);

        glBindBuffer(GL_ARRAY_BUFFER, subtractVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(subtractVertices), subtractVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subtractEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(subtractIndices), subtractIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    //Texture
    {
        static float textureVertices[] = {
                1.f,  1.f, 1.0f, 1.0f, // top right
                1.f, -1.f, 1.0f, 0.0f, // bottom right
                -1.f, -1.f, 0.0f, 0.0f, // bottom left
                -1.f,  1.f, 0.0f, 1.0f  // top left
        };
        static unsigned int textureIndices[] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
        };

        glGenVertexArrays(1, &textureVAO);
        glGenBuffers(1, &textureVBO);
        glGenBuffers(1, &textureEBO);

        glBindVertexArray(textureVAO);

        glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertices), textureVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(textureIndices), textureIndices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // col attribute
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(2);

        textureShader.set_int("texture1", 0);
    }
}

meg::opengl_drawer::~opengl_drawer()
{
    //Convex poly
    //Uses triangle fan and stencil buffer
    glDeleteBuffers(1, &convexPolyVBO);
    glDeleteVertexArrays(1, &convexPolyVAO);

    //Concave poly
    //Uses triangle fan and normal triangle shader
    glDeleteBuffers(1, &concavePolyVBO);
    glDeleteVertexArrays(1, &concavePolyVAO);

    //Triangle
    glDeleteBuffers(1, &triangleVBO);
    glDeleteVertexArrays(1, &triangleVAO);

    //Circle
    //Rectangle
    //Uses triangle fan with 4 points
    glDeleteBuffers(1, &rectangleVBO);
    glDeleteVertexArrays(1, &rectangleVAO);

    //Subtract
    glDeleteBuffers(1, &subtractVBO);
    glDeleteBuffers(1, &subtractEBO);
    glDeleteVertexArrays(1, &subtractVAO);

    //Texture
    glDeleteBuffers(1, &textureVBO);
    glDeleteBuffers(1, &textureEBO);
    glDeleteVertexArrays(1, &textureVAO);
}

void meg::opengl_drawer::convex_poly(const std::vector<vertex>& vertices, const color& color)
{
    CONTEXT_ASSERT;

    glBindBuffer(GL_ARRAY_BUFFER, convexPolyVBO);

    if (vertices.size() > convexPolyVertices)
    {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * 2 * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        convexPolyVertices = static_cast<unsigned>(vertices.size());
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * 2 * sizeof(float), vertices.data());
    }

    triangleShader.use();
    triangleShader.set4f("triangleColor", color[0], color[1], color[2], color[3]);

    glBindVertexArray(convexPolyVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(vertices.size()));
}

void meg::opengl_drawer::concave_poly(const std::vector<vertex>& vertices, const color& color)
{
    CONTEXT_ASSERT;

    glBindBuffer(GL_ARRAY_BUFFER, concavePolyVBO);
    if (vertices.size() > concavePolyVertices)
    {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * 2 * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        concavePolyVertices = static_cast<unsigned>(vertices.size());
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * 2 * sizeof(float), vertices.data());
    }

    if (vertices.empty())return;
    float minX{ vertices[0][0] }, maxX{ vertices[0][0] }, minY{ vertices[0][1] }, maxY{ vertices[0][1] };

    for (unsigned i = 1; i < vertices.size(); ++i)
    {
        if (vertices[i][0] < minX) minX = vertices[i][0];
        else if (vertices[i][0] > maxX)maxX = vertices[i][0];
        if (vertices[i][1] < minY) minY = vertices[i][1];
        else if (vertices[i][1] > maxY)maxY = vertices[i][1];
    }

    triangleShader.use();
    glBindVertexArray(concavePolyVAO);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_INVERT, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)

    // draw stencil pattern
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF

    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(vertices.size()));

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0x00);
    // draw where stencil's value is 0
    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

    rectangle({ minX, minY }, { maxX - minX, maxY - minY }, color);

    glDisable(GL_STENCIL_TEST);
}

void meg::opengl_drawer::triangle(const std::array<vertex, 3>& vertices, const color& color)
{
    CONTEXT_ASSERT;

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3*2*sizeof(float), vertices.data());

    triangleShader.use();
    triangleShader.set4f("triangleColor", color[0], color[1], color[2], color[3]);

    glBindVertexArray(triangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void meg::opengl_drawer::rectangle(const vec2& pos, const vec2& dimensions, const color& color)
{
    CONTEXT_ASSERT;

    rectangle_vertices[0] = pos[0];
    rectangle_vertices[1] = pos[1];

    rectangle_vertices[2] = pos[0] + dimensions[0];
    rectangle_vertices[3] = pos[1];

    rectangle_vertices[4] = pos[0] + dimensions[0];
    rectangle_vertices[5] = pos[1] + dimensions[1];

    rectangle_vertices[6] = pos[0];
    rectangle_vertices[7] = pos[1] + dimensions[1];

    glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(rectangle_vertices), rectangle_vertices);

    triangleShader.use();
    triangleShader.set4f("triangleColor", color[0], color[1], color[2], color[3]);

    glBindVertexArray(rectangleVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void meg::opengl_drawer::set_stroke(float width)
{
    glLineWidth(width);
}

void meg::opengl_drawer::line(const vec2& p1, const vec2& p2, const color& color)
{
    CONTEXT_ASSERT;
    float data[4]{ p1[0], p1[1], p2[0], p2[1] };

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);

    triangleShader.use();
    triangleShader.set4f("triangleColor", color[0], color[1], color[2], color[3]);

    glBindVertexArray(triangleVAO);
    glDrawArrays(GL_LINES, 0, 2);
}

void meg::opengl_drawer::subtract(meg::texture& t1, meg::texture& t2)
{
    CONTEXT_ASSERT;
    subtractShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t1.get_id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, t2.get_id());

    glBindVertexArray(subtractVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void meg::opengl_drawer::texture(meg::texture& texture)
{
    CONTEXT_ASSERT;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.get_id());

    textureShader.use();

    glBindVertexArray(textureVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void meg::opengl_drawer::set_viewport(unsigned x, unsigned y, unsigned width, unsigned height)
{
    CONTEXT_ASSERT;
    glViewport(x, y, width, height);
}

void meg::opengl_drawer::set_viewport(unsigned width, unsigned height)
{
    CONTEXT_ASSERT;
    glViewport(0, 0, width, height);
}

void meg::opengl_drawer::clear(const color& color)
{
    CONTEXT_ASSERT;

    if(clear_enabled_){
        glClearColor(color[0], color[1], color[2], color[3]);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

/*
void meg::opengl_drawer::set_circle_precision(unsigned triangles)
{
    CONTEXT_ASSERT

    circlePrecision = triangles;
    circleDirectionPairs.clear();
    circleDirectionPairs.reserve(triangles);
    float step = 6.28318530718f / static_cast<float>(triangles);
    float currentAngle = 0.f;
    for (unsigned i = 0; i < triangles; ++i)
    {
        circleDirectionPairs.emplace_back(std::sin(currentAngle), std::cos(currentAngle));
        currentAngle += step;
    }
}
*/

void meg::opengl_drawer::enable_clear(bool enable) {
    clear_enabled_ = enable;
}