//
// Created by kookie on 29/03/2020.
//

#include "image_ga.h"

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
#include "../ga/selection/tournament_selection.h"
#include "../util/drawer_thread.h"
#include "../ga/util/ga_time_condition.h"


void image_ga_main(){
    KKI_PROFILE_FUNCTION;
    //OpenGL context
    meg::context context{ true };
    std::shared_ptr<meg::opengl_drawer> ogld = std::make_shared<meg::opengl_drawer>();

    //Constants
    unsigned best_preserved{ 2 };
    unsigned not_mutated{ 1 };
    unsigned population_size{ 5 };
    unsigned vertices{ 35 };
    unsigned polygons{ 50 };

    std::string image_src{"/home/kookie/documents/Projects/MeegerenGen/images/krakow.jpg" };

    //Initial population
    std::vector<meg::cps*> drawable_solutions;
    drawable_solutions.reserve(population_size);

    for (unsigned i = 0; i < population_size; ++i) {
        drawable_solutions.push_back(new meg::cps{ polygons });
    }

    //Mutations
    meg::obligatory_composite_mutation<meg::cps> ocm;
    ocm
    .add(1.f, new meg::add_polygon_mutation{ 75, polygons, 0.005, 0.5, {0.1f, 0.4f}, true })
    .add(1.f, new meg::curving_vertex_mutation{ 500, 0.02, 0.25 })
    .add(1.f, new meg::color_mutation{ 75, true, 0.3, 0.3, 0.05f, 0.02f, {0.1f, 0.6f} })
    .add(1.f, new meg::point_mutation{ 300, 0.8, 0.05f })
    .add(1.f, new meg::point_mutation{ 500, 0.8, 0.15f })
    .add(1.f, new meg::add_vertex_mutation{ 175, vertices, {0.3f, 0.7f}, 0.05f })
    .add(1.f, new meg::remove_vertex_mutation{ 125 })
    .add(1.f, new meg::remove_polygon_mutation{ 200 })
    .add(1.f, new meg::swap_mutation{ 100 })
    .add(1.f, new meg::remove_vertex_mutation{ 20 })
    .normalized_init(2.);

    meg::tournament_selection<meg::cps> selection{2};

    std::shared_ptr<meg::texture> tex = std::make_shared<meg::texture>(image_src);
    meg::mipmap_evaluator<meg::cps> evaluator{ tex, ogld, 128, 128, 1};

    meg::mbga<meg::cps> mbga{ drawable_solutions, population_size, evaluator, ocm, selection};

    bool running = true;
    meg::drawer_thread dt{mbga, 600, 400, "current_state", running,std::chrono::milliseconds{30}};
    dt.start();

    meg::ga_time_condition<meg::cps, std::chrono::minutes> timeCondition(std::chrono::minutes(3));

    mbga.run_condition(timeCondition);

    running = false;
    dt.join();
}





