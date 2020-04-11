//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_GENETIC_ALGORITHM_H
#define MEEGERENGEN_GENETIC_ALGORITHM_H


namespace meg{

    template <typename sol_t>
    class genetic_algorithm
    {
    protected:
        unsigned iteration_ = 0;

    public:

        class genetic_algorithm_condition
        {
        public:
            virtual ~genetic_algorithm_condition() = default;
            virtual void init(genetic_algorithm* gen_alg) {}
            virtual void update(genetic_algorithm* gen_alg) = 0;
            virtual operator bool() = 0;
        };

        virtual ~genetic_algorithm() = default;

        //Returns best solution
        virtual sol_t* get_solution() = 0;
        virtual unsigned get_iteration() { return iteration_; }
        virtual void set_iteration(unsigned i) { iteration_ = i; }

        //Runs one iteration of the genetic algorithm
        virtual void run_iteration() = 0;

        //Runs n iterations_ of the genetic algorithm
        virtual void run_iterations(unsigned iterations)
        {
            for (unsigned i = 0; i < iterations; ++i)
            {
                run_iteration();
            }
        }

        //Runs for loop until condition is satisfied
        virtual void run_condition(genetic_algorithm_condition& con)
        {
            for (con.init(this); con; con.update(this))
            {
                run_iteration();
            }
        }
    };

    template<typename sol_t>
    using condition = typename genetic_algorithm<sol_t>::genetic_algorithm_condition;
}



#endif //MEEGERENGEN_GENETIC_ALGORITHM_H
