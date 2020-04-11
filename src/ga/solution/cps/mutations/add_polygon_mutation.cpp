//
// Created by kookie on 29/03/2020.
//

#include "add_polygon_mutation.h"
#include "../../../../util/random.h"

meg::add_polygon_mutation::add_polygon_mutation
    (   int mutation_chance,
        unsigned max_polygons,
        double min_triangle_size,
        float sigma,
        std::pair<float, float> alpha_range,
        bool colored_,
        std::pair<float, float> point_range_horizontal,
        std::pair<float, float> point_range_vertical)
        :   mutation_chance_(mutation_chance),
            max_polygons_(max_polygons),
            min_triangle_size_(min_triangle_size),
            sigma_(sigma),
            alpha_range_(std::move(alpha_range)),
            colored_(colored_),
            point_range_horizontal_(std::move(point_range_horizontal)),
            point_range_vertical_(std::move(point_range_vertical))
{
}

bool meg::add_polygon_mutation::mutate(cps* sol)
{
    kki::random& rand = kki::rng::tl;

    if (sol->polygons.size() >= max_polygons_) return false;
    if (rand.random_bool(mutation_chance_))
    {
        meg::cps::polygon poly;

        const float new_grayscale = rand.random_real<float>();
        poly.col = {
                colored_ ? rand.random_real<float>() : new_grayscale,
                colored_ ? rand.random_real<float>() : new_grayscale,
                colored_ ? rand.random_real<float>() : new_grayscale,
                rand.random_real<float>(alpha_range_.first, alpha_range_.second)
        };

        while (true)
        {
            poly.vertices.clear();
            const std::pair<float, float> center = {
                    rand.random_real<float>(point_range_horizontal_.first, point_range_horizontal_.second),
                    rand.random_real<float>(point_range_vertical_.first, point_range_vertical_.second)
            };

            for (unsigned i = 0; i < 3; ++i)
            {
                float x = center.first + rand.random_real<float>(-sigma_, sigma_);
                float y = center.second + rand.random_real<float>(-sigma_, sigma_);

                // Used to get x and y into the acceptable range
                x = std::max(std::min(x, point_range_horizontal_.second), point_range_horizontal_.first);
                y = std::max(std::min(y, point_range_vertical_.second), point_range_vertical_.first);

                poly.vertices.push_back({x, y});
            }
            // If the area of the new triangle is greater than the minimum area, continue
            if (poly.area() > min_triangle_size_)
                break;
        }

        sol->polygons.push_back(poly);
        return true;
    }

    return false;
}
