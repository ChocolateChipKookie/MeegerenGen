//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_ADD_VERTEX_MUTATION_H
#define MEEGERENGEN_ADD_VERTEX_MUTATION_H

#include "../cps.h"
#include "../../../mutation/mutation.h"

namespace meg{
    class add_vertex_mutation : public mutation<cps>
    {
        int mutation_chance_;
        unsigned max_vertices_;
        std::pair<float, float> interpolation_range_;
        float sigma_;

        std::pair<float, float> point_range_horizontal_;
        std::pair<float, float> point_range_vertical_;

    public:
        add_vertex_mutation(
                int mutation_chance,
                unsigned max_vertices,
                std::pair<float, float> interpolation_range,
                float sigma,
                std::pair<float, float> point_range_horizontal_ = {-1.f, 1.f},
                std::pair<float, float> point_range_vertical_ = {-1.f, 1.f});

        bool mutate(cps* sol) override;
    };
}


#endif //MEEGERENGEN_ADD_VERTEX_MUTATION_H
