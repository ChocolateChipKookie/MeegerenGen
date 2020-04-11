//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_SVG_DRAWER_H
#define MEEGERENGEN_SVG_DRAWER_H

#include "drawer.h"
#include <sstream>

namespace meg{
    class svg_drawer : public drawer{
    public:
        svg_drawer(double width, double height);;

        ~svg_drawer() override = default;

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

        void create_group(const std::string& group_name);
        void close_group();
        void enable_clear(bool enable);
        void save(const std::string& file_name);

    private:
        std::stringstream builder_stream_;

        static std::string color_to_percentages(const color& color);

        size_t tabs_{1};

        bool clear_enabled_{true};
        double width_, height_;
        double viewport_width_, viewport_height_;
        double offset_x_{0}, offset_y_{0};
        double stroke_{1.f};
    };
}



#endif //MEEGERENGEN_SVG_DRAWER_H
