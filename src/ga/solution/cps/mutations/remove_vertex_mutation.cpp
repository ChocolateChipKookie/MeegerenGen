//
// Created by kookie on 29/03/2020.
//

#include "remove_vertex_mutation.h"
#include "../../../../util/random.h"

meg::remove_vertex_mutation::remove_vertex_mutation
    (   int mutation_chance)
        : mutation_chance_(mutation_chance)
{
}

bool meg::remove_vertex_mutation::mutate(cps* sol)
{
    bool changed = false;
    kki::random& rand = kki::rng::tl;

    for (auto& polygon : sol->polygons)
    {
        auto points = polygon.vertices;
        if (points.size() <= 3) continue;
        if (rand.random_bool(mutation_chance_))
        {
            const size_t randPos = rand.random_index(points.size());
            points.erase(points.begin() + randPos);
            changed = true;
        }
    }

    return changed;
}
