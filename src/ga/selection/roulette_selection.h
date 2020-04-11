//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_ROULETTE_SELECTION_H
#define MEEGERENGEN_ROULETTE_SELECTION_H


#include "selection.h"
#include "../../util/random.h"
#include "../solution/cps/cps.h"

namespace meg {
    template <typename sol_t>
    class roulette_selection : public selection<sol_t> {
    public:
        roulette_selection() = default;

        sol_t *select(std::vector<sol_t*> &solutions) override;

    private:
        kki::random& rand = kki::rng::tl;
    };

    template <typename sol_t>
    sol_t *roulette_selection<sol_t>::select(std::vector<sol_t*> &solutions) {
        //Query needed values
        double total = 0.;
        double min = solutions[0]->fitness;

        /*
        for(auto& sol : solutions){
            min = std::min(min, sol->fitness);
        }

        for(auto& sol : solutions){
            total += sol->fitness - min;
        }
        */
        for(auto& sol : solutions){
            min = std::min(min, sol->fitness);
            total += sol->fitness - min;
        }

        total -= min * solutions.size();

        // Pos = random in range [0, total]
        // Where total is the sum of truncated fitnesses
        // This way, the worst solution will never be selected
        // If the average fitness is high but the differences between solutions are small,
        // by subtracting the worst fitness out of all the solutions, the difference between solutions gets enhanced
        double pos = rand.random_real(total);
        for(auto& sol : solutions){
            pos = pos - (sol->fitness - min);
            if(pos <= 0){
                return sol;
            }
        }

        // In the rare case, no solution gets selected, a random solution gets returned
        return rand.random_element(solutions);
    }
}


#endif //MEEGERENGEN_ROULETTE_SELECTION_H
