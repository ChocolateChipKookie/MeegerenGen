//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_OBLIGATORY_COMPOSITE_MUTATION_H
#define MEEGERENGEN_OBLIGATORY_COMPOSITE_MUTATION_H

#include "mutation.h"
#include <memory>
#include <vector>
#include <iostream>
#include "../../util/random.h"
#include "../../util/timer.h"

namespace meg{

    template <typename sol_t>
    class obligatory_composite_mutation : public mutation<sol_t>
    {
    public:

        // If force_mutation is set to true, it is guaranteed that there will be at least one mutation per mutate call
        explicit obligatory_composite_mutation(const std::vector<std::pair<mutation<sol_t>*, double>>& mutations, bool force_mutation = true);
        explicit obligatory_composite_mutation(bool force_mutation = true);

        // Add mutation with desired weight
        obligatory_composite_mutation& add(double weight, mutation<sol_t>* mutation);

        // Initializes internal variables
        // Uses weights given in the construction process
        void init();

        // Normalizes the weights to a [0, norm] interval
        // Default norm = 1 so the sum of all weights is 1
        // Norm can be set higher for a possibility for several mutations per mutate call
        void normalized_init(double norm = 1.);

        // Mutates given solution
        // Returns if a mutation has occurred
        bool mutate(sol_t* sol) override;

    private:
        struct mutation_{
            mutation<sol_t>* mut_;
            double weight;
            double working_weight;
            size_t working_weight_i;
        };

        bool force_mutation_;

        // Mutations
        std::vector<mutation_> mutations_;
        kki::random& rand = kki::rng::tl;
    };

    template <typename sol_t>
    obligatory_composite_mutation<sol_t>::obligatory_composite_mutation(const std::vector<std::pair<mutation<sol_t>*, double>>& mutations, bool force_mutation)
            :   force_mutation_{force_mutation}
    {
        for(auto& mut : mutations){
            add(mut.first, mut.second);
        }
        init();
    }

    template <typename sol_t>
    obligatory_composite_mutation<sol_t>::obligatory_composite_mutation(bool force_mutation)
            :   force_mutation_{force_mutation}
    {
    }

    template <typename sol_t>
    void obligatory_composite_mutation<sol_t>::init() {
        for (auto& mut : mutations_){
            mut.working_weight_i = mut.weight * static_cast<double>(this->RNG_INT_PRECISION);
        }
    }

    template <typename sol_t>
    void obligatory_composite_mutation<sol_t>::normalized_init(double norm) {
        double total_weights = 0;
        for (auto& mut : mutations_){
            total_weights += mut.weight;
        }

        for (auto& mut : mutations_){
            mut.working_weight = mut.weight / total_weights * norm;
            mut.working_weight_i = mut.working_weight * static_cast<double>(this->RNG_INT_PRECISION);
        }
    }

    template <typename sol_t>
    obligatory_composite_mutation<sol_t>& obligatory_composite_mutation<sol_t>::add(double weight, mutation<sol_t> *mutation) {
        mutations_.push_back({mutation, weight, weight, 0});
        return *this;
    }

    template <typename sol_t>
    bool obligatory_composite_mutation<sol_t>::mutate(sol_t* sol)
    {
        KKI_PROFILE_FUNCTION;
        bool mutated = !force_mutation_;

        do{
            for (auto& mut : mutations_){
                if(rand.random_int(this->RNG_INT_PRECISION) <= mut.working_weight_i){
                    bool res = mut.mut_->mutate(sol);
                    mutated = mutated ||res;
                }
            }
        } while(!mutated);

        return mutated;
    }
}

#endif //MEEGERENGEN_OBLIGATORY_COMPOSITE_MUTATION_H
