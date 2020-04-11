//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_SWAP_MUTATION_H
#define MEEGERENGEN_SWAP_MUTATION_H

#include "../../../mutation/mutation.h"
#include "../cps.h"

namespace meg{
    class swap_mutation : public mutation<cps>
    {
    private:
        int mutation_chance_;
    public:
        explicit swap_mutation(int mutation_chance);

        bool mutate(cps* sol) override;

    };

}


#endif //MEEGERENGEN_SWAP_MUTATION_H
