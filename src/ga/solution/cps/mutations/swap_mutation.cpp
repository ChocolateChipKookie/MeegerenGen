//
// Created by kookie on 29/03/2020.
//

#include "swap_mutation.h"
#include "../../../../util/random.h"
#include <algorithm>

meg::swap_mutation::swap_mutation
    (   int mutation_chance)
        :   mutation_chance_(mutation_chance)
{
}

bool meg::swap_mutation::mutate(cps* sol)
{
    auto& poly = sol->polygons;
    kki::random& rand = kki::rng::tl;

    if (sol->polygons.empty() || sol->polygons.size() == 1) return false;
    if (rand.random_bool(mutation_chance_))
    {
        unsigned first = rand.random_index(poly.size());
        unsigned second = rand.random_index(poly.size());

        //In case the polygons are the same, swap two neighbouring polygons
        if (first == second)
            second = (second + 1) % poly.size();

        std::swap(poly[first], poly[second]);
        return true;
    }
    return false;
}
