//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_RANDOM_SELECTION_H
#define MEEGERENGEN_RANDOM_SELECTION_H

#include "selection.h"
#include "../../util/random.h"

namespace meg {
    template<typename sol_t>
    class random_selection : public selection<sol_t> {
    public:
        random_selection() = default;

        sol_t *select(std::vector<sol_t *> &solutions) override;

    private:
        kki::random &rand = kki::rng::tl;
    };

    template<typename sol_t>
    sol_t *random_selection<sol_t>::select(std::vector<sol_t *> &solutions) {
        return rand.random_element(solutions);
    }
}

#endif //MEEGERENGEN_RANDOM_SELECTION_H
