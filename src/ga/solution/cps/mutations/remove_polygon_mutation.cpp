//
// Created by kookie on 29/03/2020.
//

#include "remove_polygon_mutation.h"
#include "../../../../util/random.h"

meg::remove_polygon_mutation::remove_polygon_mutation
    (   int mutation_chance)
        :   mutation_chance_(mutation_chance)
{
}

bool meg::remove_polygon_mutation::mutate(cps* sol)
{
    auto& poly = sol->polygons;
    kki::random& rand = kki::rng::tl;
    if (poly.empty()) return false;
    if (rand.random_bool(mutation_chance_))
    {
        const size_t randPos = rand.random_index(poly.size());
        poly.erase(poly.begin() + randPos);
        return true;
    }
    return false;
}

