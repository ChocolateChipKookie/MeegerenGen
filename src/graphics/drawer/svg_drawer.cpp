//
// Created by kookie on 28/03/2020.
//

#include <fstream>
#include "svg_drawer.h"

meg::svg_drawer::svg_drawer(double width, double height) : width_(width), height_(height), viewport_width_(width), viewport_height_(height){
    builder_stream_ << R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)" << std::endl;
    builder_stream_ << R"(<svg xmlns:svg="http://www.w3.org/2000/svg" xmlns="http://www.w3.org/2000/svg" width=")" << width_ << R"(" height=")" << height_ << R"(">)" << std::endl;
}



void meg::svg_drawer::convex_poly(const std::vector<vertex> &vertices, const meg::color &color) {
    builder_stream_ << std::string(tabs_, '\t') << "<polygon points=\"";

    for (unsigned i = 0; i < vertices.size(); ++i)
    {
        auto x = static_cast<unsigned>(((vertices[i][0] + 1.f) / 2.) * viewport_width_ + offset_x_);
        auto y = static_cast<unsigned>(((2. - (vertices[i][1] + 1.f)) / 2.) * viewport_height_ + offset_y_);
        builder_stream_ << x << "," << y << (i != (vertices.size() - 1) ? " " : "");
    }
    builder_stream_ << R"(" fill=")" << color_to_percentages(color) << R"( fill-opacity=")" + std::to_string(color[3]) + '"' << "/>" << std::endl;
}

void meg::svg_drawer::concave_poly(const std::vector<vertex> &vertices, const meg::color &color) {
    builder_stream_ << std::string(tabs_, '\t') << "<polygon points=\"";

    for (unsigned i = 0; i < vertices.size(); ++i)
    {
        unsigned x = static_cast<unsigned>(((vertices[i][0] + 1.f) / 2.) * viewport_width_ + offset_x_);
        unsigned y = static_cast<unsigned>(((2. - (vertices[i][1] + 1.f)) / 2.) * viewport_height_ + offset_y_);
        builder_stream_ << x << "," << y << (i != (vertices.size() - 1) ? " " : "");
    }
    builder_stream_ << R"(" fill=")" << color_to_percentages(color) << R"(" fill-rule="evenodd")" << R"( fill-opacity=")" + std::to_string(color[3]) + '"' << "/>" << std::endl;
}

void meg::svg_drawer::triangle(const std::array<vertex, 3> &vertices, const meg::color &color) {
    builder_stream_ << std::string(tabs_, '\t') << "<polygon points=\"";

    for (unsigned i = 0; i < 3; ++i)
    {
        auto x = static_cast<unsigned>(((vertices[i][0] + 1.f) / 2.) * viewport_width_ + offset_x_);
        auto y = static_cast<unsigned>(((2. - (vertices[i][1] + 1.f)) / 2.) * viewport_height_ + offset_y_);
        builder_stream_ << x << "," << y << (i != (vertices.size() - 1) ? " " : "");
    }
    builder_stream_ << R"(" fill=")" << color_to_percentages(color)<< R"( fill-opacity=")" + std::to_string(color[3]) + '"' << "/>" << std::endl;
}

void meg::svg_drawer::rectangle(const meg::vec2 &pos, const meg::vec2 &dimensions, const meg::color &color) {
//    <rect x="1" y="1" width="300" height="100" style="fill:rgb(0,0,255);stroke-width:3;stroke:rgb(0,0,0)" />
    auto x = static_cast<unsigned>(((pos[0] + 1.f) / 2.) * viewport_width_ + offset_x_);
    auto y = static_cast<unsigned>(((2. - (pos[1] + 1.f)) / 2.) * viewport_height_ + offset_y_);

    auto width = static_cast<unsigned>((dimensions[0] * viewport_width_)/ 2.f);
    auto height = static_cast<unsigned>((dimensions[1] * viewport_width_)/ 2.f);


    builder_stream_ << std::string(tabs_, '\t')
                    << "<rect x=\""
                    << x
                    << "\" y=\""
                    << y
                    << "\" width=\""
                    << width
                    << "\" height=\""
                    << height
                    << "\" fill=\""
                    << color_to_percentages(color)
                    << "\" fill-opacity=\""
                    << color[3]
                    << "\"/>"
                    << std::endl;
}

void meg::svg_drawer::set_stroke(float width) {
    stroke_ = width;
}

void meg::svg_drawer::line(const meg::vec2 &p1, const meg::vec2 &p2, const meg::color &color) {
    //<line x1="0" y1="0" x2="200" y2="200" style="stroke:rgb(255,0,0);stroke-width:2" />

    auto x1 = static_cast<unsigned>(((p1[0] + 1.f) / 2.) * viewport_width_ + offset_x_);
    auto y1 = static_cast<unsigned>(((2. - (p1[1] + 1.f)) / 2.) * viewport_height_ + offset_y_);

    auto x2 = static_cast<unsigned>(((p2[0] + 1.f) / 2.) * viewport_width_ + offset_x_);
    auto y2 = static_cast<unsigned>(((2. - (p2[1] + 1.f)) / 2.) * viewport_height_ + offset_y_);


    builder_stream_ << std::string(tabs_, '\t')
                    << "<line x1=\""
                    << x1
                    << "\" y1=\""
                    << y1
                    << "\" x2=\""
                    << x2
                    << "\" y2=\""
                    << y2
                    << "\" stroke=\""
                    << color_to_percentages(color)
                    << "\" opacity=\""
                    << color[3]
                    << "\" stroke-width=\""
                    << stroke_
                    << "\"/>"
                    << std::endl;

}

void meg::svg_drawer::clear(const meg::color &color) {
    if(clear_enabled_){
        builder_stream_ << std::string(tabs_, '\t')
                        << "<rect width = \""
                        << width_
                        << "\" height=\""
                        << height_
                        << "\" fill=\""
                        << color_to_percentages(color)
                        << "\"/>"
                        << std::endl;
    }
}

void meg::svg_drawer::set_viewport(unsigned x, unsigned y, unsigned width, unsigned height) {
    offset_x_ = x;
    offset_y_ = height_ - y - height;
    viewport_width_ = width;
    viewport_height_ = height;
}

void meg::svg_drawer::set_viewport(unsigned width, unsigned height) {
    set_viewport(0, 0, width, height);
}

void meg::svg_drawer::save(const std::string &file_name) {
    std::ofstream file_stream(file_name);
    file_stream << builder_stream_.rdbuf();
    file_stream << "</svg>";
    file_stream.close();
}

void meg::svg_drawer::create_group(const std::string &group_name) {
    //<g id="pozadina">
    builder_stream_ << std::string(tabs_, '\t')
                    << "<g id=\""
                    << group_name
                    << "\">"
                    << std::endl;
    ++tabs_;
}

void meg::svg_drawer::close_group() {
    assert(tabs_ > 1);
    --tabs_;
    builder_stream_ << std::string(tabs_, '\t')
                    << "</g>"
                    << std::endl;
}


std::string meg::svg_drawer::color_to_percentages(const meg::color &color) {
    std::stringstream stream;
    stream << "rgb(" <<
           color[0] * 100.f << "%, " <<
           color[1] * 100.f << "%, " <<
           color[2] * 100.f << "%)";
    return std::string(stream.str());
}

void meg::svg_drawer::enable_clear(bool enable) {
    clear_enabled_ = enable;
}

