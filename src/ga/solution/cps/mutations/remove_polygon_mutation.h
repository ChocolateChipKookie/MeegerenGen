//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_REMOVE_POLYGON_MUTATION_H
#define MEEGERENGEN_REMOVE_POLYGON_MUTATION_H

#include "../../../mutation/mutation.h"
#include "../cps.h"

namespace meg{
    class remove_polygon_mutation :  public mutation<cps>
    {
        int mutation_chance_;
    public:
        void set_chance(int chance){ mutation_chance_ = chance; }
        remove_polygon_mutation(int mutation_chance);

        bool mutate(cps* sol) override;
    };

}

#endif //MEEGERENGEN_REMOVE_POLYGON_MUTATION_H
