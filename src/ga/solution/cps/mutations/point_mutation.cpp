//
// Created by kookie on 29/03/2020.
//

#include "point_mutation.h"
#include "../../../../util/random.h"

meg::point_mutation::point_mutation
    (   int mutation_chance,
        double separation_factor,
        float mutation_intensity,
        std::pair<float, float> point_range_horizontal,
        std::pair<float, float> point_range_vertical)
        :   mutation_chance_(mutation_chance - 1),
            separation_factor_(static_cast<int>(separation_factor * RNG_INT_PRECISION)),
            mutation_intensity_(mutation_intensity),
            point_range_horizontal_(std::move(point_range_horizontal)),
            point_range_vertical_(std::move(point_range_vertical))
{
}

bool meg::point_mutation::mutate(cps* sol)
{
    bool changed = false;
    kki::random& rand = kki::rng::tl;

    for(auto& polygon : sol->polygons)
    {
        for(auto & point : polygon.vertices)
        {
            if (rand.random_bool(mutation_chance_))
            {
                if (rand.random_int(RNG_INT_PRECISION) < separation_factor_)
                    point[0] += rand.random_real<float>(-mutation_intensity_, mutation_intensity_);
                if (rand.random_int(RNG_INT_PRECISION) < separation_factor_)
                    point[1] += rand.random_real<float>(-mutation_intensity_, mutation_intensity_);

                point[0] = std::max(std::min(point[0], point_range_horizontal_.second), point_range_horizontal_.first);
                point[1] = std::max(std::min(point[1], point_range_vertical_.second), point_range_vertical_.first);

                changed = true;
            }
        }
    }
    return changed;
}
