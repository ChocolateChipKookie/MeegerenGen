//
// Created by kookie on 27/03/2020.
//

#ifndef MEEGERENGEN_DRAWER_H
#define MEEGERENGEN_DRAWER_H

#include <vector>
#include <array>
#include <cassert>
#include "../../util/types.h"

namespace meg{

    class drawer
    {
    public:
        virtual ~drawer() = default;

        virtual void convex_poly(const std::vector<vertex>& vertices, const color& color) = 0;
        virtual void concave_poly(const std::vector<vertex>& vertices, const color& color) = 0;
        virtual void triangle(const std::array<vertex, 3>& vertices, const color& color) = 0;
        //virtual void ellipse(const vec2& centerPos, float a, float b, const col& col) = 0;
        virtual void rectangle(const vec2& pos, const vec2& dimensions, const color& color) = 0;
        virtual void set_stroke(float width) = 0;
        virtual void line(const vec2& p1, const vec2& p2, const color& color) = 0;

        virtual void clear(const color& color) = 0;
        virtual void set_viewport(unsigned x, unsigned y, unsigned width, unsigned height) = 0;
        virtual void set_viewport(unsigned width, unsigned height) = 0;
    };
}


#endif //MEEGERENGEN_DRAWER_H
