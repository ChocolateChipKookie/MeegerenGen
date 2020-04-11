//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_COLOR_MUTATION_H
#define MEEGERENGEN_COLOR_MUTATION_H

#include "../cps.h"
#include "../../../mutation/mutation.h"

namespace meg{
    class color_mutation : public mutation<cps>
    {
        int mutation_chance_;
        bool colored_;

        int separation_factor_;
        int alpha_factor_;

        float sigma_;
        float alpha_sigma_;

        std::pair<float, float> alpha_range_;
        std::pair<float, float> color_range_;

    public:
        color_mutation(
                int mutation_chance,
                bool colored,
                double separation_factor,
                double alpha_factor,
                float sigma,
                float alpha_sigma,
                std::pair<float, float> alpha_range,
                std::pair<float, float> color_range = {0.f, 1.f});

        bool mutate(cps* sol) override;
    };
}


#endif //MEEGERENGEN_COLOR_MUTATION_H
