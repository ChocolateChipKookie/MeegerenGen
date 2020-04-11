//
// Created by kookie on 30/03/2020.
//

#ifndef MEEGERENGEN_SELF_ADJUSTABLE_MUTATION_H
#define MEEGERENGEN_SELF_ADJUSTABLE_MUTATION_H

#include <vector>
#include "mutation.h"
#include "../solution/solution.h"
#include "../../util/random.h"
#include "../../util/timer.h"

namespace meg{

    template <typename sol_t>
    class self_adjustable_mutation : public mutation<sol_t>{
    public:
        self_adjustable_mutation(const std::vector<mutation<sol_t>*>& mutations, double min_probability, size_t iterations, size_t tries);

        bool mutate(sol_t* sol) override;
        void update(bool good);

    private:
        const size_t treshold_ = 50;

        size_t counter_ = 0;
        size_t iterations_ = 0;
        size_t min_probability_;
        size_t tries_;

        std::vector<mutation<sol_t>*> mutations_;
        std::vector<size_t> probabilities_;

        size_t total_probability_{};
        std::vector<size_t> uses_;
        std::vector<size_t> hits_;
        std::vector<bool> used_;
    };

    template <typename sol_t>
    self_adjustable_mutation<sol_t>::self_adjustable_mutation
        (   const std::vector<mutation<sol_t>*>& mutations,
            double min_probability,
            size_t iterations,
            size_t tries)
            :   iterations_{iterations},
                min_probability_{static_cast<size_t>(this->RNG_INT_PRECISION * min_probability)},
                tries_{tries},
                mutations_{mutations},
                probabilities_(mutations.size(), min_probability_),
                total_probability_(this->min_probability_ * mutations_.size()),
                uses_(mutations.size()),
                hits_(mutations.size()),
                used_(mutations.size())
    {
    }

    template <typename sol_t>
    bool self_adjustable_mutation<sol_t>::mutate(sol_t* sol)
    {
        KKI_PROFILE_FUNCTION;
        bool mutated = false;

        std::fill(used_.begin(), used_.end(), false);
        kki::random& rand = kki::rng::tl;

        while (!mutated)
        {
            KKI_PROFILE_SCOPE("MUTATION");

            for(unsigned i = 0; i < tries_; ++i)
            {
                size_t position = rand.random_index(total_probability_);

                size_t p = 0;
                while(true)
                {
                    if(position <= probabilities_[p])
                        break;

                    position -= probabilities_[p];
                    p++;
                }

                if (mutations_[p]->mutate(sol))
                {
                    //mutated = mutated || mutation->mutate(sol);
                    mutated = true;
                    used_[p] = true;
                    uses_[p]++;
                }
            }
        }
        ++counter_;
        // Update probabilities
        if (counter_ == iterations_)
        {
            KKI_PROFILE_SCOPE("RESET");

            counter_ = 0;
            bool ready = false;

            for(auto use : uses_)
            {
                if(use > treshold_)
                {
                    ready = true;
                    break;
                }
            }
            if(ready)
            {
                std::fill(probabilities_.begin(), probabilities_.end(), this->min_probability_);

                std::vector<double> percentages(mutations_.size());
                double max{ 0 };
                double min{ 1 };
                for (unsigned i = 0; i < mutations_.size(); ++i)
                {
                    percentages[i] = static_cast<double>(hits_[i]) / static_cast<double>(uses_[i]);
                    if (percentages[i] > max) max = percentages[i];
                    else if (percentages[i] < min) min = percentages[i];
                }

                for (auto& p : percentages)
                {
                    p -= min;
                    p /= max - min;
                }

                total_probability_ = 0;
                for (unsigned i = 0; i < mutations_.size(); ++i)
                {
                    probabilities_[i] += static_cast<unsigned>(percentages[i] * this->RNG_INT_PRECISION);
                    total_probability_ += probabilities_[i];
                }
                total_probability_--;
                std::fill(uses_.begin(), uses_.end(), 0);
                std::fill(hits_.begin(), hits_.end(), 0);
            }
        }
        return mutated;
    }

    template <typename sol_t>
    void self_adjustable_mutation<sol_t>::update(bool good)
    {
        if(good)
        {
            for (size_t i = 0; i < mutations_.size(); ++i)
            {
                if(used_[i])
                {
                    hits_[i]++;
                }
            }
        }
    }

}





#endif //MEEGERENGEN_SELF_ADJUSTABLE_MUTATION_H
