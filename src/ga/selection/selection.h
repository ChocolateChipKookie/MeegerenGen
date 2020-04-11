//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_SELECTION_H
#define MEEGERENGEN_SELECTION_H

#include <type_traits>
#include <vector>
#include "../solution/solution.h"

namespace meg{

    template<typename sol_t>
    class selection
    {
        static_assert(std::is_base_of<solution, sol_t>::value, "sol_t must derive from solution");

    public:
        virtual ~selection() = default;

        // Returns random selected unit from population
        // Does not return a clone
        virtual sol_t* select(std::vector<sol_t*>& solutions) = 0;
        virtual std::vector<sol_t*> select_n(std::vector<sol_t*>& solutions, unsigned count)
        {
            std::vector<sol_t*> res;
            res.reserve(count);
            for(unsigned i = 0; i < count; ++i)
            {
                res.push_back(select(solutions));
            }
            return res;
        }
    };
}

#endif //MEEGERENGEN_SELECTION_H
