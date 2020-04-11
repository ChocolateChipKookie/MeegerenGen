//
// Created by kookie on 29/03/2020.
//

#include "add_vertex_mutation.h"
#include "../../../../util/random.h"

meg::add_vertex_mutation::add_vertex_mutation
    (   int mutation_chance, unsigned max_vertices,
        std::pair<float, float> interpolation_range,
        float sigma,
        std::pair<float, float> point_range_horizontal,
        std::pair<float, float> point_range_vertical)
        :   mutation_chance_(mutation_chance),
            max_vertices_(max_vertices),
            interpolation_range_(std::move(interpolation_range)),
            sigma_(sigma),
            point_range_horizontal_(std::move(point_range_horizontal)),
            point_range_vertical_(std::move(point_range_vertical))
{
}

bool meg::add_vertex_mutation::mutate(cps* sol)
{
    bool changed{ false };
    kki::random& rand = kki::rng::tl;
    for(auto& polygon : sol->polygons)
    {
        auto& points = polygon.vertices;
        if (points.size() >= max_vertices_ || points.empty()) continue;
        if (rand.random_bool(mutation_chance_))
        {
            //The position of the new vertex is going to be between these two vertices
            const size_t vertex1 = rand.random_index(points.size());
            const size_t vertex2 = (vertex1 + 1) % points.size();

            //The position of the new point is interpolated between the two old points
            const float position = rand.random_real<float>(interpolation_range_.first, interpolation_range_.second);
            std::array<float, 2> newPos{
                    points[vertex1][0] + (points[vertex2][0] - points[vertex1][0]) * position,
                    points[vertex1][1] + (points[vertex2][1] - points[vertex1][1]) * position
            };

            //Move the new point a bit
            newPos[0] += rand.random_real<float>(-sigma_, sigma_);
            newPos[1] += rand.random_real<float>(-sigma_, sigma_);

            // Used to get x and y into the acceptable range
            newPos[0] = std::max(std::min(newPos[0], point_range_horizontal_.second), point_range_horizontal_.first);
            newPos[1] = std::max(std::min(newPos[1], point_range_vertical_.second), point_range_vertical_.first);

            points.insert(points.begin() + vertex2, newPos);
            changed = true;
        }
    }

    return changed;
}
