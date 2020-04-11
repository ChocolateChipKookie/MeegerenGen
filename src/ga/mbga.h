//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_MBGA_H
#define MEEGERENGEN_MBGA_H

#include "solution/solution.h"
#include "evaluator/evaluator.h"
#include "mutation/mutation.h"
#include "selection/selection.h"
#include "drawable_genetic_algorithm.h"
#include "../util/timer.h"
#include <mutex>

namespace meg{
    // Mutation based genetic algorithm
    template <typename sol_t>
    class mbga : public drawable_genetic_algorithm<sol_t>{
        static_assert(std::is_base_of<solution, sol_t>::value, "sol_t must be a have solution as base");
    public:
        mbga
            (   std::vector<sol_t*> starting_population,
                unsigned population_size,
                evaluator<sol_t>& eval,
                mutation<sol_t>& mut,
                selection<sol_t>& sel)
                :   population_{starting_population},
                    population_size_{population_size},
                    eval_{eval},
                    mut_{mut},
                    sel_{sel}
        {
            KKI_PROFILE_FUNCTION;
            eval_.evaluate_generation(starting_population);
            best_solution_ = (*std::max_element(
                    population_.begin(),
                    population_.end(),
                    [](sol_t* s1, sol_t* s2) {return s1->fitness < s2->fitness; }))->clone();
        }

        sol_t* get_solution()
        {
            return best_solution_;
        }

        void run_iteration() override
        {
            KKI_PROFILE_FUNCTION;
            std::vector<sol_t*> new_generation(population_size_, nullptr);

            for (unsigned i = 0; i < population_size_; ++i)
            {
                sol_t* selected = sel_.select(population_);
                new_generation[i] = selected->clone();

                bool mutated = mut_.mutate(new_generation[i]);
                new_generation[i]->evaluated = !mutated;
            }

            for (auto& unit : population_)
            {
                delete unit;
                unit = nullptr;
            }

            population_.swap(new_generation);

            eval_.evaluate_generation(population_);

            {
                std::unique_lock<std::mutex> lock(this->get_solution_mutex());
                sol_t* curr_best = *std::max_element(
                        population_.begin(),
                        population_.end(),
                        [](sol_t* s1, sol_t* s2) {return s1->fitness < s2->fitness; });

                if (curr_best->fitness > best_solution_->fitness){
                    delete best_solution_;
                    best_solution_ = curr_best->clone();
                }
            }


            new_generation.clear();
            ++(this->iteration_);
        }

    private:
        std::vector<sol_t*> population_;
        unsigned population_size_;
        evaluator<sol_t>& eval_;
        mutation<sol_t>& mut_;
        selection<sol_t>& sel_;

        sol_t* best_solution_{nullptr};
    };
}


#endif //MEEGERENGEN_MBGA_H
