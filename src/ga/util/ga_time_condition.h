//
// Created by kookie on 31/03/2020.
//

#ifndef MEEGERENGEN_GA_TIME_CONDITION_H
#define MEEGERENGEN_GA_TIME_CONDITION_H

#include <chrono>
#include "../genetic_algorithm.h"
#include "../solution/cps/cps.h"
#include <ratio>

namespace meg{

    template <typename sol_t, typename time_t, typename clock_t = std::chrono::high_resolution_clock>
    class ga_time_condition : public genetic_algorithm<sol_t>::genetic_algorithm_condition{
    public:

        explicit ga_time_condition(const time_t& framerate) : time_(framerate){}

        ~ga_time_condition() override = default;
        void init(genetic_algorithm<sol_t>* gen_alg) override {
            end_timepoint_ = clock_t::now() + time_;
        }

        void update(genetic_algorithm<sol_t>* gen_alg) override {};
        explicit operator bool() override {
            return clock_t::now() <= end_timepoint_;
        }

    private:
        std::chrono::time_point<clock_t> end_timepoint_{};
        time_t time_;
    };
}




#endif //MEEGERENGEN_GA_TIME_CONDITION_H
