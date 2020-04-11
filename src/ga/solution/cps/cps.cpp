//
// Created by kookie on 28/03/2020.
//

#include "cps.h"

size_t meg::cps::vertices(){
    size_t total = 0;
    for (auto& polygon : polygons)
    {
        total += polygon.vertices.size();
    }
    return total;
}

void meg::cps::draw(meg::drawer& drawer)
{
    background[3] = 1.f;
    drawer.clear(background);
    for (auto& polygon : polygons)
    {
        drawer.concave_poly(polygon.vertices, polygon.col);
    }
}

meg::cps* meg::cps::clone()
{
    return new cps(*this);
}

float meg::cps::polygon::area() {

    // Using the shoelace formula, calculates the area of the polygon
    float agg = 0.f;
    for(unsigned i = 0; i < vertices.size() - 1; ++i){
        agg += vertices[i][0] * vertices[i + 1][1];
        agg -= vertices[i][1] * vertices[i + 1][0];
    }

    agg += vertices.back()[0] * vertices[0][1];
    agg -= vertices.back()[1] * vertices[0][0];

    return std::abs(agg/2.f);
}
