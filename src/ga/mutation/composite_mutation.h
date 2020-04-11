//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_COMPOSITE_MUTATION_H
#define MEEGERENGEN_COMPOSITE_MUTATION_H
#include "mutation.h"
#include <memory>
#include <vector>
#include "../../util/random.h"

namespace meg{

    template <typename sol_t>
    class composite_mutation : public mutation<sol_t>
    {
    public:
        explicit composite_mutation(const std::vector<std::pair<mutation<sol_t>*, double>>& mutations);
        composite_mutation() = default;

        // Add mutation with desired weight
        composite_mutation& add(mutation<sol_t>* mutation, double weight);
        // Initializes internal variables for the algorithm
        void init();
        // Mutates given solution
        // Returns if a mutation has occurred
        bool mutate(sol_t* sol) override;

    private:
        struct mutation_{
            mutation<sol_t>* mut_;
            double weight;
            double normalized_weight_pos;
            size_t normalized_weight_pos_i;
        };

        // Mutations
        std::vector<mutation_> mutations_;
        kki::random& rand = kki::rng::tl;
    };

    template <typename sol_t>
    composite_mutation<sol_t>::composite_mutation(const std::vector<std::pair<mutation<sol_t>*, double>>& mutations){
        for(auto& mut : mutations){
            add(mut.first, mut.second);
        }
        init();
    }

    template <typename sol_t>
    void composite_mutation<sol_t>::init() {
        double total_weights = 0;
        for (auto& mut : mutations_){
            total_weights += mut.weight;
        }

        double weight_pos = 0.;
        for (auto& mut : mutations_){
            mut.normalized_weight_pos = weight_pos + mut.weight/total_weights;
            weight_pos = mut.normalized_weight_pos;
            mut.normalized_weight_pos_i = mut.normalized_weight_pos * static_cast<double>(this->RNG_INT_PRECISION);
        }
    }

    template <typename sol_t>
    composite_mutation<sol_t>& composite_mutation<sol_t>::add(mutation<sol_t> *mutation, double weight) {
        mutations_.push_back({mutation, weight, 0., 0});
        return *this;
    }



    template <typename sol_t>
    bool composite_mutation<sol_t>::mutate(sol_t* sol)
    {
        size_t pos = rand.random_int(this->RNG_INT_PRECISION);

        for (auto& mut : mutations_){
            if(pos <= mut.normalized_weight_pos_i )
                return mut.mut_->mutate(sol);
        }

        return rand.random_element(mutations_).mut_->mutate(sol);
    }
}

#endif //MEEGERENGEN_COMPOSITE_MUTATION_H
