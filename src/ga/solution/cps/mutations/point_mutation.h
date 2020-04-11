//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_POINT_MUTATION_H
#define MEEGERENGEN_POINT_MUTATION_H

#include "../../../mutation/mutation.h"
#include "../cps.h"

namespace meg{
    class point_mutation : public mutation<cps>
    {
        int mutation_chance_;
        int separation_factor_;
        float mutation_intensity_;

        std::pair<float, float> point_range_horizontal_;
        std::pair<float, float> point_range_vertical_;

    public:
        point_mutation(int mutation_chance, double separation_factor, float mutation_intensity, std::pair<float, float> point_range_horizontal = { -1.f, 1.f }, std::pair<float, float> point_range_vertical = { -1.f, 1.f });

        bool mutate(cps* sol) override;
    };

}

#endif //MEEGERENGEN_POINT_MUTATION_H
