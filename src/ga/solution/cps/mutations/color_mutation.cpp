//
// Created by kookie on 29/03/2020.
//

#include "color_mutation.h"
#include "../../../../util/random.h"

meg::color_mutation::color_mutation
    (   int mutation_chance,
        bool colored,
        double separation_factor,
        double alpha_factor,
        float sigma,
        float alpha_sigma,
        std::pair<float, float> alpha_range,
        std::pair<float, float> color_range)
        :   mutation_chance_(mutation_chance),
            colored_(colored),
            separation_factor_(static_cast<int>(separation_factor * RNG_INT_PRECISION)),
            alpha_factor_(static_cast<int>(alpha_factor * RNG_INT_PRECISION)),
            sigma_(sigma),
            alpha_sigma_(alpha_sigma),
            alpha_range_(std::move(alpha_range)),
            color_range_(std::move(color_range))
{
}

bool meg::color_mutation::mutate(cps* sol)
{
    bool mutated = false;

    kki::random& rand = kki::rng::tl;

    for(auto& poly : sol->polygons)
    {
        auto& color = poly.col;
        if (colored_)
        {
            if (rand.random_bool(mutation_chance_))
            {
                for (unsigned i = 0; i < 3; ++i)
                {
                    if (rand.random_int(RNG_INT_PRECISION) < separation_factor_)
                    {
                        color[i] += rand.random_real<float>(-sigma_, sigma_);
                        color[i] = std::max(std::min(color[i], color_range_.second), color_range_.first);
                        mutated = true;
                    }
                }
                if (rand.random_int(RNG_INT_PRECISION) < alpha_factor_)
                {
                    color[3] += rand.random_real<float>(-alpha_sigma_, alpha_sigma_);
                    color[3] = std::max(std::min(color[3], alpha_range_.second), alpha_range_.first);
                    mutated = true;
                }
            }
        }
        else
        {
            if (rand.random_bool(mutation_chance_))
            {
                if (rand.random_int(RNG_INT_PRECISION) < separation_factor_)
                {
                    float grayscale = color[0] + rand.random_real<float>(-sigma_, sigma_);
                    grayscale = std::max(std::min(grayscale, color_range_.second), color_range_.first);
                    for (unsigned i = 0; i < 3; ++i)
                    {
                        color[i] = grayscale;
                    }
                }
                if (rand.random_int(RNG_INT_PRECISION) < alpha_factor_)
                {
                    color[3] += rand.random_real<float>(-alpha_sigma_, alpha_sigma_);
                    if (color[3] < alpha_range_.first) color[3] = alpha_range_.first;
                    else  if (color[3] > alpha_range_.second) color[3] = alpha_range_.second;
                    mutated = true;
                }
            }
        }
    }

    return mutated;
}
