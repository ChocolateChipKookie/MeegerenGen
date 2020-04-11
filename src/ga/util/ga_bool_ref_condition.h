//
// Created by kookie on 02/04/2020.
//

#ifndef MEEGERENGEN_GA_BOOL_REF_CONDITION_H
#define MEEGERENGEN_GA_BOOL_REF_CONDITION_H

#include "../genetic_algorithm.h"

namespace meg{

    template <typename sol_t>
    class ga_bool_ref_condition : public genetic_algorithm<sol_t>::genetic_algorithm_condition{
    public:

        explicit ga_bool_ref_condition(bool& running) : running_(running){}

        ~ga_bool_ref_condition() override = default;

        void init(genetic_algorithm<sol_t>* gen_alg) override {}
        void update(genetic_algorithm<sol_t>* gen_alg) override {};
        explicit operator bool() override {
            return running_;
        }

    private:
        bool& running_;
    };
}



#endif //MEEGERENGEN_GA_BOOL_REF_CONDITION_H
