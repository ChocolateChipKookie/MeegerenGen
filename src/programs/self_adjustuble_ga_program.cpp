//
// Created by kookie on 30/03/2020.
//

#include "self_adjustuble_ga_program.h"
#include <vector>

#include "../graphics/context.h"
#include "../graphics/drawer/opengl_drawer.h"

#include "../ga/solution/cps/cps.h"
#include "../ga/solution/cps/mutations/remove_polygon_mutation.h"
#include "../ga/solution/cps/mutations/add_polygon_mutation.h"
#include "../ga/solution/cps/mutations/curving_vertex_mutation.h"
#include "../ga/solution/cps/mutations/color_mutation.h"
#include "../ga/solution/cps/mutations/point_mutation.h"
#include "../ga/solution/cps/mutations/add_vertex_mutation.h"
#include "../ga/solution/cps/mutations/remove_vertex_mutation.h"
#include "../ga/solution/cps/mutations/swap_mutation.h"
#include "../ga/mutation/obligatory_composite_mutation.h"
#include "../ga/evaluator/mipmap_evaluator.h"
#include "../ga/mbga.h"
#include "../util/drawer_thread.h"
#include "../ga/mutation/self_adjustable_mutation.h"
#include "../ga/self_adjustable_ga.h"
#include "../ga/util/ga_time_condition.h"
#include "../util/timer.h"

void sa_image_main()
{
    KKI_PROFILE_FUNCTION;

    //OpenGL context
    meg::context context{ true };

    //Constants
    unsigned best_preserved{ 2 };
    unsigned not_mutated{ 1 };
    unsigned population_size{ 5 };
    unsigned vertices{ 35 };
    unsigned polygons{ 500 };

    std::string image_src{ "/home/kookie/documents/Projects/MeegerenGen/images/krakow.jpg"};

    //Initial population
    std::vector<meg::cps*> drawable_solutions;
    drawable_solutions.reserve(population_size);

    for (unsigned i = 0; i < population_size; ++i) drawable_solutions.push_back(new meg::cps{polygons});


    meg::remove_polygon_mutation* remove_polygon_mutation_ = new meg::remove_polygon_mutation{ 200 };

    //Mutations
    std::vector<meg::mutation<meg::cps>*> mutations;
    {
        //mutations.push_back(new add_polygon_mutation{ 2, polygons, 0.05, 0.25, {0.1f, 0.9f}, true });
        mutations.push_back(new meg::add_polygon_mutation{ 100, polygons, 0.001, 0.05, {0.1f, 0.4f}, true });
        mutations.push_back(new meg::curving_vertex_mutation{ 500, 0.02, 0.25 });
        //mutations.push_back(new background_mutation{ 250, true, 0.4f, 0.05f, {0.f, 1.f} });
        mutations.push_back(new meg::color_mutation{ 75, true, 0.3, 0.3, 0.05f, 0.02f, {0.1f, 0.6f} });
        mutations.push_back(new meg::point_mutation{ 300, 0.8, 0.05f });
        mutations.push_back(new meg::point_mutation{ 500, 0.8, 0.15f });
        mutations.push_back(new meg::add_vertex_mutation{ 175, vertices, {0.3f, 0.7f}, 0.05f });
        mutations.push_back(new meg::remove_vertex_mutation{ 125 });
        mutations.push_back(remove_polygon_mutation_);
        mutations.push_back(new meg::swap_mutation{ 100 });
        mutations.push_back(new meg::remove_vertex_mutation{ 20 });
    }

    //obligatory_composite_mutation<concave_polygon_solution> ocm{ mutations };
    std::shared_ptr<meg::self_adjustable_mutation<meg::cps>> ocm =
            std::make_shared<meg::self_adjustable_mutation<meg::cps>>(mutations, 0.4, 5000, 15);

    //Evaluator
    std::shared_ptr<meg::texture> tex = std::make_shared<meg::texture>(image_src);
    std::shared_ptr<meg::opengl_drawer> ogld = std::make_shared<meg::opengl_drawer>();
    std::shared_ptr<meg::mipmap_evaluator<meg::cps>> evaluator =
            std::make_shared<meg::mipmap_evaluator<meg::cps>>(tex, ogld, 128, 128, 1);

    //drawable_ga<concave_polygon_solution> dga{ ocm, evaluator, drawable_solutions, best_preserved, not_mutated, population_size };
    meg::self_adjustable_ga<meg::cps> saga{ocm, evaluator, drawable_solutions, best_preserved, not_mutated, population_size};

    bool running = true;
    meg::drawer_thread dt{saga, 600, 400, "current_state", running,std::chrono::milliseconds{30}};
    dt.start();

    meg::ga_time_condition<meg::cps, std::chrono::minutes> timeCondition(std::chrono::minutes (3));

    saga.run_condition(timeCondition);

    running = false;
    dt.join();

}
