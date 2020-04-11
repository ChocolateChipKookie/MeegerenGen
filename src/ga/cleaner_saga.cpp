//
// Created by kookie on 31/03/2020.
//

#include "cleaner_saga.h"

void meg::cleaner_saga::run_iteration() {

    if (iteration_ % clean_iterations_ == 0){
        clean_population();
        reevaluate_population();

        if (iteration_ % clean_decay_iterations_ == 0){
            clean_size_ *= clean_decay_;
        }
    }

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
            [](cps* s1, cps* s2) {return s1->fitness > s2->fitness; });

    if (best_solution_->fitness < population_[0]->fitness)
    {
        std::unique_lock<std::mutex> lock(this->get_solution_mutex());
        delete best_solution_;
        best_solution_ = population_[0]->clone();
    }
    ++this->iteration_;
}

float meg::cleaner_saga::triangle_area(const meg::vec2 &v1, const meg::vec2 &v2, const meg::vec2 &v3) {
    float double_area =
            v1[0] * (v2[1] - v3[1]) +
            v3[0] * (v1[1] - v2[1]) +
            v2[0] * (v3[1] - v1[1]);
    return std::abs(double_area) / 2.f;
}

meg::cleaner_saga::cleaner_saga
    (   std::shared_ptr<self_adjustable_mutation<cps>> mutation,
        std::shared_ptr<evaluator <cps>> evaluator,
        std::vector<cps *> drawable_solutions,
        unsigned best_preserved,
        unsigned not_mutated,
        unsigned population_size,
        unsigned clean_iterations,
        float clean_size,
        unsigned clean_decay_iterations,
        float clean_decay)
        :   mutation_(std::move(mutation)),
            evaluator_(std::move(evaluator)),
            population_(std::move(drawable_solutions)),
            best_preserved_(best_preserved),
            not_mutated_(not_mutated),
            population_size_(population_size),
            clean_iterations_(clean_iterations),
            clean_size_(clean_size),
            clean_decay_iterations_(clean_decay_iterations),
            clean_decay_(clean_decay)
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

void meg::cleaner_saga::reevaluate_population() {
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

void meg::cleaner_saga::clean_population() {

    for(auto& solution : population_){

        std::vector<size_t> to_remove;

        for(size_t p = 0; p < solution->polygons.size(); ++p) {
            auto& polygon = solution->polygons[p];
            bool changed = true;

            while(changed){
                changed = false;

                auto& vertices = polygon.vertices;

                if(vertices.size() > 3){
                    for (size_t i = 0; i < vertices.size(); ++i){
                        vec2& left = vertices[(i + vertices.size() - 1) % vertices.size()];
                        vec2& center = vertices[i];
                        vec2& right = vertices[(i + 1) % vertices.size()];

                        float area = triangle_area(left, center, right);

                        if(area < clean_size_){
                            vertices.erase(vertices.begin() + i);
                            changed = true;
                            break;
                        }
                    }
                }
                else if (vertices.size() == 3){
                    if (polygon.area() < clean_size_){
                        to_remove.push_back(p);
                    }
                }
                else{
                    to_remove.push_back(p);
                }
            }
        }

        std::sort(to_remove.begin(), to_remove.end(), [](size_t i1, size_t i2){return i1 > i2;});

        for(size_t index : to_remove){
            solution->polygons.erase(solution->polygons.begin() + index);
        }

    }
}
