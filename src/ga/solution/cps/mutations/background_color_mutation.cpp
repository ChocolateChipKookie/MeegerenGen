//
// Created by kookie on 29/03/2020.
//

#include "background_color_mutation.h"
#include "../../../../util/random.h"

meg::background_color_mutation::background_color_mutation
    (   int mutation_chance,
        bool colored,
        double separation_factor,
        float sigma,
        const std::pair<float, float>& color_range)
        :   mutation_chance_(mutation_chance),
            colored_(colored),
            separation_factor_( static_cast<int>(static_cast<double>(RNG_INT_PRECISION) * separation_factor)),
            sigma_(sigma),
            color_range_(color_range)
{
}

bool meg::background_color_mutation::mutate(cps* sol)
{
    bool mutated = false;
    kki::random& rand = kki::rng::tl;

    auto& color = sol->background;
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
        }
    }
    return mutated;
}
