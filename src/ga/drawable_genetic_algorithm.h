//
// Created by kookie on 30/03/2020.
//

#ifndef MEEGERENGEN_DRAWABLE_GENETIC_ALGORITHM_H
#define MEEGERENGEN_DRAWABLE_GENETIC_ALGORITHM_H

#include <mutex>
#include "genetic_algorithm.h"

namespace meg{

    template <typename sol_t>
    class drawable_genetic_algorithm : public genetic_algorithm<sol_t>{
    public:
        virtual std::mutex& get_solution_mutex() final {return solution_mutex_;}
    private:
        std::mutex solution_mutex_;
    };
}


#endif //MEEGERENGEN_DRAWABLE_GENETIC_ALGORITHM_H
