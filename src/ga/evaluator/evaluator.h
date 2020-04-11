//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_EVALUATOR_H
#define MEEGERENGEN_EVALUATOR_H

#include <vector>
#include "../solution/solution.h"

namespace meg{

    template<typename sol_t>
    class evaluator
    {
        static_assert(std::is_base_of<solution, sol_t>::value, "sol_t must derive from solution");

    public:
        virtual ~evaluator() = default;

        virtual void evaluate(sol_t* sol) = 0;
        virtual void evaluate_generation(std::vector<sol_t*>& generation)
        {
            for (auto& sol : generation)
            {
                evaluate(sol);
            }
        }
    };
}

#endif //MEEGERENGEN_EVALUATOR_H
