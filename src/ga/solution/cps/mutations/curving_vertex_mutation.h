//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_CURVING_VERTEX_MUTATION_H
#define MEEGERENGEN_CURVING_VERTEX_MUTATION_H

#include "../cps.h"
#include "../../../mutation/mutation.h"

namespace meg{
    class curving_vertex_mutation : public mutation<cps>
    {
        int mutation_chance_;
        float delete_condition_;
        float intensity_;

    public:
        curving_vertex_mutation(int mutation_chance, float delete_condition, float intensity);

        bool mutate(cps* sol) override;
    };

}


#endif //MEEGERENGEN_CURVING_VERTEX_MUTATION_H
