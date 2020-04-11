//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_BACKGROUND_COLOR_MUTATION_H
#define MEEGERENGEN_BACKGROUND_COLOR_MUTATION_H

#include "../cps.h"
#include "../../../mutation/mutation.h"


namespace meg{
    class background_color_mutation : public mutation<cps>
    {
        int mutation_chance_;
        bool colored_;

        int separation_factor_;
        float sigma_;

        std::pair<float, float> color_range_;

    public:

        background_color_mutation(
                int mutation_chance,
                bool colored,
                double separation_factor,
                float sigma,
                const std::pair<float, float>& color_range);

        bool mutate(cps* sol) override;
    };
}

#endif //MEEGERENGEN_BACKGROUND_COLOR_MUTATION_H
