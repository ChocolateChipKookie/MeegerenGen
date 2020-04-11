//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_MUTATION_H
#define MEEGERENGEN_MUTATION_H
#include <type_traits>
#include "../solution/solution.h"

namespace meg{

    template<typename sol_t>
    class mutation
    {
        static_assert(std::is_base_of<solution, sol_t>::value, "sol_t must be a have solution as base");
    public:
        // As the rng is up to 10 times faster for integer types than floating point numbers
        // All the probabilties givein in a [0, 1] range are scaled to a [0, RNG_INT_PRECISION] range
        const static size_t RNG_INT_PRECISION {100000000};

        virtual ~mutation() = default;
        virtual bool mutate(sol_t* sol) = 0;
    };

}

#endif //MEEGERENGEN_MUTATION_H
