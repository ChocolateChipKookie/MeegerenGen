//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_OPENGL_DRAWER_H
#define MEEGERENGEN_OPENGL_DRAWER_H

#include "drawer.h"
#include "../texture.h"
#include "../shader.h"

namespace meg{
    class opengl_drawer : public drawer{
    public:
        opengl_drawer();
        ~opengl_drawer() override;

        void convex_poly(const std::vector<vertex>& vertices, const color& color) override;
        void concave_poly(const std::vector<vertex>& vertices, const color& color) override;
        void triangle(const std::array<vertex, 3>& vertices, const color& color) override;
        //void ellipse(const vec2& centerPos, float a, float b, const col& col) override;
        void rectangle(const vec2& pos, const vec2& dimensions, const color& color) override;
        void set_stroke(float width) override;
        void line(const vec2& p1, const vec2& p2, const color& color) override;

        void clear(const color& color) override;
        void set_viewport(unsigned x, unsigned y, unsigned width, unsigned height) override;
        void set_viewport(unsigned width, unsigned height) override;

        // Class specific methods
        virtual void subtract(meg::texture& t1, meg::texture& t2);
        virtual void texture(meg::texture& texture);

        void enable_clear(bool enable);
    private:
        //Flag for clearing screen
        //Used for genetic algorithm when several solutions are drawn at once
        bool clear_enabled_{true};

        //Convex poly
        //Uses triangle fan and stencil buffer
        unsigned int convexPolyVertices{ 4 };
        unsigned int convexPolyVBO{ 0 }, convexPolyVAO{ 0 };
        //Concave poly
        //Uses triangle fan and normal triangle shader
        unsigned int concavePolyVertices{ 4 };
        unsigned int concavePolyVBO{ 0 }, concavePolyVAO{ 0 };
        //Triangle
        shader triangleShader;
        unsigned int triangleVBO{ 0 }, triangleVAO{ 0 };
        //Circle
        unsigned circlePrecision{ 60 };
        std::vector<std::pair<float, float>> circleDirectionPairs;
        //Rectangle
        //Uses triangle fan with 4 points
        unsigned int rectangleVBO{ 0 }, rectangleVAO{ 0 };
        float rectangle_vertices[8]{};
        //Subtract
        shader subtractShader;
        unsigned int subtractVBO{ 0 }, subtractVAO{ 0 }, subtractEBO{ 0 };
        //Texture
        shader textureShader;
        unsigned int textureVBO{ 0 }, textureVAO{ 0 }, textureEBO{ 0 };

    };
}

#endif //MEEGERENGEN_OPENGL_DRAWER_H
