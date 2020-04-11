//
// Created by kookie on 31/03/2020.
//

#ifndef MEEGERENGEN_CLEANER_SAGA_H
#define MEEGERENGEN_CLEANER_SAGA_H

#include "solution/cps/cps.h"
#include "drawable_genetic_algorithm.h"
#include "mutation/self_adjustable_mutation.h"
#include "evaluator/evaluator.h"


namespace meg{

    class cleaner_saga : public drawable_genetic_algorithm<cps>{
    public:

        cleaner_saga
            (   std::shared_ptr<self_adjustable_mutation<cps>> mutation,
                std::shared_ptr<evaluator<cps>> evaluator,
                std::vector<cps*> drawable_solutions,
                unsigned best_preserved,
                unsigned not_mutated,
                unsigned population_size,
                unsigned clean_iterations,
                float clean_size,
                unsigned clean_decay_iterations,
                float clean_decay
                );

        void reevaluate_population();

        cps* get_solution() override
        {
            return best_solution_;
        }

        void run_iteration() override;



        void clean_population();

    private:

        static float triangle_area(const meg::vec2& v1, const meg::vec2& v2, const meg::vec2& v3);

        std::shared_ptr<self_adjustable_mutation<cps>> mutation_;
        std::shared_ptr<evaluator<cps>> evaluator_;
        std::vector<cps*> population_;

        cps* best_solution_{};
        unsigned best_preserved_;
        unsigned not_mutated_;
        unsigned population_size_;
        unsigned clean_iterations_;
        float clean_size_;
        unsigned clean_decay_iterations_;
        float clean_decay_;
    };
}

#endif //MEEGERENGEN_CLEANER_SAGA_H
