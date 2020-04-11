//
// Created by kookie on 30/03/2020.
//

#ifndef MEEGERENGEN_SELF_ADJUSTABLE_GA_H
#define MEEGERENGEN_SELF_ADJUSTABLE_GA_H

#include <utility>

#include "genetic_algorithm.h"
#include "mutation/self_adjustable_mutation.h"
#include "evaluator/mipmap_evaluator.h"
#include "drawable_genetic_algorithm.h"


namespace meg{


    template <typename sol_t>
    class self_adjustable_ga : public drawable_genetic_algorithm<sol_t>{
    public:

        self_adjustable_ga
            (   std::shared_ptr<self_adjustable_mutation<sol_t>> mutation,
                std::shared_ptr<evaluator<sol_t>> evaluator,
                std::vector<sol_t*> drawable_solutions,
                unsigned best_preserved,
                unsigned not_mutated,
                unsigned population_size)
                :   mutation_(std::move(mutation)),
                    evaluator_(std::move(evaluator)),
                    population_(std::move(drawable_solutions)),
                    best_preserved_(best_preserved),
                    not_mutated_(not_mutated),
                    population_size_(population_size)
        {
            evaluator_->evaluate_generation(population_);

            {
                std::unique_lock<std::mutex> get_solution_lock(this->get_solution_mutex());
                best_solution_ = (*std::max_element(
                                population_.begin(),
                                population_.end(),
                                [](solution* s1, solution* s2){return s1->fitness < s2->fitness;})
                                 )->clone();
            }
        }

        void reevaluate_population()
        {
            for(auto& s : population_)
            {
                s->evaluated = false;
                evaluator_->evaluate(s);
            }

            {
                std::unique_lock<std::mutex> get_solution_lock(this->get_solution_mutex());
                best_solution_ = (*std::max_element(population_.begin(), population_.end(), [](solution* s1, solution* s2)
                {
                    return s1->fitness < s2->fitness;
                }))->clone();
            }
        }

        sol_t* get_solution() override
        {
            return best_solution_;
        }

        void run_iteration() override;

    private:
        std::shared_ptr<self_adjustable_mutation<sol_t>> mutation_;
        std::shared_ptr<evaluator<sol_t>> evaluator_;
        std::vector<sol_t*> population_;

        sol_t* best_solution_{};
        unsigned best_preserved_;
        unsigned not_mutated_;
        unsigned population_size_;
    };

    template<typename sol_t>
    void self_adjustable_ga<sol_t>::run_iteration() {
        for (unsigned k = 0; k < best_preserved_; ++k)
        {
            delete population_[population_size_ - 1 - k];
            population_[population_size_ - 1 - k] = population_[k]->clone();
        }

        for (unsigned k = not_mutated_; k < population_size_; ++k)
        {
            double previous_fitness = population_[k]->fitness;
            mutation_->mutate(population_[k]);
            population_[k]->evaluated = false;
            evaluator_->evaluate(population_[k]);
            mutation_->update(previous_fitness < population_[k]->fitness);
        }

        std::sort(
                population_.begin(),
                population_.end(),
                [](sol_t* s1, sol_t* s2) {return s1->fitness > s2->fitness; });

        if (best_solution_->fitness < population_[0]->fitness)
        {
            std::unique_lock<std::mutex> lock(this->get_solution_mutex());
            delete best_solution_;
            best_solution_ = population_[0]->clone();
        }
        ++this->iteration_;
    }
}


#endif //MEEGERENGEN_SELF_ADJUSTABLE_GA_H
