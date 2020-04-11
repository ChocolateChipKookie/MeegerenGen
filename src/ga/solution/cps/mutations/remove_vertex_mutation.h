//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_REMOVE_VERTEX_MUTATION_H
#define MEEGERENGEN_REMOVE_VERTEX_MUTATION_H

#include "../../../mutation/mutation.h"
#include "../cps.h"

namespace meg{
    class remove_vertex_mutation : public mutation<cps>
    {
    private:
        int mutation_chance_;
    public:
        remove_vertex_mutation(int mutation_chance);

        bool mutate(cps* sol) override;

        void set_chance(int chance){mutation_chance_ = chance;}
    };


}

#endif //MEEGERENGEN_REMOVE_VERTEX_MUTATION_H
