//
// Created by kookie on 29/03/2020.
//

#include "curving_vertex_mutation.h"
#include "../../../../util/random.h"

meg::curving_vertex_mutation::curving_vertex_mutation
    (   int mutation_chance,
        float delete_condition,
        float intensity)
        :   mutation_chance_(mutation_chance),
            delete_condition_(delete_condition),
            intensity_(intensity)
{
}

bool meg::curving_vertex_mutation::mutate(cps* sol)
{
    bool changed = false;
    kki::random& rand = kki::rng::tl;

    for (auto& polygon : sol->polygons)
    {
        for (unsigned i = 0; i < polygon.vertices.size(); ++i)
        {
            if (rand.random_bool(mutation_chance_))
            {
                size_t s = polygon.vertices.size();
                auto& point_center = polygon.vertices[i];
                auto& point_left = polygon.vertices[(i + s - 1 ) % s];
                auto& point_right = polygon.vertices[(i + 1) % s];

                //Vector from point_center to the mid point between point_left and point_right
                const float vec_x = ((point_left[0] + point_right[0]) / 2.f) - point_center[0];
                const float vec_y = ((point_left[1] + point_right[1]) / 2.f) - point_center[1];

                const float intensity = rand.random_real<float>(0.f, intensity_);
                point_center[0] = point_center[0] + vec_x * intensity;
                point_center[1] = point_center[1] + vec_y * intensity;

                const float area = std::abs(
                        point_center[0] * (point_left[1] - point_right[1]) +
                        point_right[0] * (point_center[1] - point_left[1]) +
                        point_left[0] * (point_right[1] - point_center[1])) / 2.f;

                if(area < delete_condition_)
                {
                    polygon.vertices.erase(polygon.vertices.begin() + i);
                }

                changed = true;
            }
        }
    }
    return changed;
}
