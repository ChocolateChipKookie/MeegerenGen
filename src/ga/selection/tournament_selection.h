//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_TOURNAMENT_SELECTION_H
#define MEEGERENGEN_TOURNAMENT_SELECTION_H

#include "selection.h"
#include "../../util/random.h"

namespace meg{
    template <typename sol_t>
    class tournament_selection : public selection<sol_t> {
    public:
        explicit tournament_selection(size_t k) : k_(k) {};

        sol_t* select(std::vector<sol_t*>& solutions) override;
        std::vector<sol_t*> select_n(std::vector<sol_t*>& solutions, unsigned count) override;
        void set_k(size_t k) {k_ = k;}
        size_t get_k() const {return k_;}

    private:
        size_t k_;
        kki::random& rand = kki::rng::tl;
    };

    template <typename sol_t>
    sol_t* tournament_selection<sol_t>::select(std::vector<sol_t*> &solutions) {
        std::vector<sol_t*> tmp_selected;

        for (unsigned k = 0; k < k_; ++k)
        {
            sol_t* tmp_ptr;
            do
            {
                tmp_ptr = rand.random_element(solutions);
            } while (std::find(tmp_selected.begin(), tmp_selected.end(), tmp_ptr) != tmp_selected.end());

            tmp_selected.push_back(tmp_ptr);
        }

        return *std::max_element(tmp_selected.begin(), tmp_selected.end(), [](solution* s1, solution* s2) {return s1->fitness < s2->fitness; });    }

    template <typename sol_t>
    std::vector<sol_t*> tournament_selection<sol_t>::select_n(std::vector<sol_t*> &solutions, unsigned count) {
        assert(count < solutions.size());
        std::vector<sol_t*> result;

        for (unsigned counter = 0; counter < count; ++counter)
        {
            std::vector<sol_t*> tmp_results;

            for (unsigned k = 0; k < k_; ++k)
            {
                sol_t* tmp_ptr;
                do
                {
                    tmp_ptr = rand.random_element(solutions);
                } while (
                        std::find(tmp_results.begin(), tmp_results.end(), tmp_ptr) != tmp_results.end() ||
                        std::find(result.begin(), result.end(), tmp_ptr) != result.end()
                        );
                tmp_results.push_back(tmp_ptr);
            }

            sol_t* s = *std::max_element(tmp_results.begin(), tmp_results.end(), [](solution* s1, solution* s2) {return s1->fitness < s2->fitness; });
            result.push_back(s);
        }

        return result;
    }
}

#endif //MEEGERENGEN_TOURNAMENT_SELECTION_H
