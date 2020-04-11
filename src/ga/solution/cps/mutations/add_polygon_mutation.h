//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_ADD_POLYGON_MUTATION_H
#define MEEGERENGEN_ADD_POLYGON_MUTATION_H

#include "../../../mutation/mutation.h"
#include "../cps.h"

namespace meg{
    class add_polygon_mutation : public mutation<cps>
    {
    private:
        int mutation_chance_;
        unsigned max_polygons_;
        double min_triangle_size_;
        float sigma_;
        std::pair<float, float> alpha_range_;
        bool colored_;

        std::pair<float, float> point_range_horizontal_;
        std::pair<float, float> point_range_vertical_;

    public:
        add_polygon_mutation(
                int mutation_chance,
                unsigned max_polygons,
                double min_triangle_size,
                float sigma,
                std::pair<float, float> alpha_range_,
                bool colored_,
                std::pair<float, float> point_range_horizontal_ = {-1.f, 1.f},
                std::pair<float, float> point_range_vertical_ = {-1.f, 1.f});

        bool mutate(cps* sol) override;
    };

}


#endif //MEEGERENGEN_ADD_POLYGON_MUTATION_H
