//
// Created by kookie on 02/04/2020.
//

#include "creator_programs.h"
#include "../util/timer.h"
#include "../graphics/texture.h"
#include "../ga/mutation/mutation.h"
#include "../ga/solution/cps/mutations/add_polygon_mutation.h"
#include "../ga/solution/cps/mutations/curving_vertex_mutation.h"
#include "../ga/solution/cps/mutations/color_mutation.h"
#include "../ga/solution/cps/mutations/point_mutation.h"
#include "../ga/solution/cps/mutations/add_vertex_mutation.h"
#include "../ga/solution/cps/mutations/remove_vertex_mutation.h"
#include "../ga/solution/cps/mutations/remove_polygon_mutation.h"
#include "../ga/solution/cps/mutations/swap_mutation.h"
#include "../ga/mutation/self_adjustable_mutation.h"
#include "../graphics/drawer/opengl_drawer.h"
#include "../ga/evaluator/mipmap_evaluator.h"
#include "../ga/cleaner_saga.h"
#include "../graphics/context.h"
#include "../util/data_loader.h"
#include "../util/drawer_thread.h"
#include "../ga/util/ga_bool_ref_condition.h"
#include "../util/multi_drawer_thread.h"


std::shared_ptr<meg::drawable_genetic_algorithm<meg::cps>> create_cps_ga(std::shared_ptr<meg::texture> tex, std::shared_ptr<meg::opengl_drawer> ogld){
    KKI_PROFILE_FUNCTION;

    //Constants
    unsigned best_preserved{ 2 };
    unsigned not_mutated{ 1 };
    unsigned population_size{ 5 };
    unsigned vertices{ 35 };
    unsigned polygons{ 500 };

    //Initial population
    std::vector<meg::cps*> drawable_solutions;
    drawable_solutions.reserve(population_size);

    for (unsigned i = 0; i < population_size; ++i) drawable_solutions.push_back(new meg::cps{polygons});

    //Mutations
    std::vector<meg::mutation<meg::cps>*> mutations;
    {
        mutations.push_back(new meg::add_polygon_mutation{ 100, polygons, 0.001, 0.05, {0.1f, 0.4f}, true });
        mutations.push_back(new meg::curving_vertex_mutation{ 500, 0.02, 0.25 });
        mutations.push_back(new meg::color_mutation{ 75, true, 0.3, 0.3, 0.05f, 0.02f, {0.1f, 0.6f} });
        mutations.push_back(new meg::point_mutation{ 300, 0.8, 0.05f });
        mutations.push_back(new meg::point_mutation{ 500, 0.8, 0.15f });
        mutations.push_back(new meg::add_vertex_mutation{ 175, vertices, {0.3f, 0.7f}, 0.05f });
        mutations.push_back(new meg::remove_vertex_mutation{ 125 });
        mutations.push_back(new meg::remove_polygon_mutation{ 200 });
        mutations.push_back(new meg::swap_mutation{ 100 });
        mutations.push_back(new meg::remove_vertex_mutation{ 20 });
    }

    //obligatory_composite_mutation<concave_polygon_solution> ocm{ mutations };
    std::shared_ptr<meg::self_adjustable_mutation<meg::cps>> ocm =
            std::make_shared<meg::self_adjustable_mutation<meg::cps>>(mutations, 0.4, 5000, 15);

    //Evaluator
    std::shared_ptr<meg::mipmap_evaluator<meg::cps>> evaluator =
            std::make_shared<meg::mipmap_evaluator<meg::cps>>(tex, ogld, 128, 128, 1);

    return std::make_shared<meg::cleaner_saga>(    ocm,
                                                   evaluator,
                                                   drawable_solutions,
                                                   best_preserved,
                                                   not_mutated,
                                                   population_size,
                                                   100,
                                                   0.0015,
                                                   25000,
                                                   0.666);
}

void creator_main(const std::string& image_path){
    meg::context context(true);
    std::shared_ptr<meg::opengl_drawer> drawer = std::make_shared<meg::opengl_drawer>();

    std::shared_ptr<meg::texture> tex = std::make_shared<meg::texture>(image_path);
    std::shared_ptr<meg::drawable_genetic_algorithm<meg::cps>> gen_alg = create_cps_ga(tex, drawer);

    bool running = true;
    meg::drawer_thread dt{*gen_alg, 400, 600, "current_state", running,std::chrono::milliseconds{30}};
    dt.start();

    meg::ga_bool_ref_condition<meg::cps> ref_condition(running);

    gen_alg->run_condition(ref_condition);

    running = false;
    dt.join();
}

void multiple_creator_main(const std::string& image_path, const std::string& mask_path, unsigned masks){
    meg::context context(true);
    std::shared_ptr<meg::opengl_drawer> drawer = std::make_shared<meg::opengl_drawer>();

    meg::data_loader loader(image_path, mask_path, masks, "intermediate");
    loader.load();
    loader.generate_masks();
    loader.generate_subimages();


    std::vector<meg::multi_drawer_thread::ga_metadata> ga_metadata;

    for (auto& subimage : loader.get_subimages()){
        std::shared_ptr<meg::texture> tex = std::make_shared<meg::texture>(subimage.data, subimage.width, subimage.height, 3);

        auto ga = create_cps_ga(tex, drawer);
        meg::multi_drawer_thread::ga_metadata md{ga, subimage.x, subimage.y, subimage.width, subimage.height};
        ga_metadata.push_back(md);
    }

    bool running = true;
    meg::multi_drawer_thread dt{ga_metadata, 600, 400, "current_state", running,std::chrono::seconds {5}};
    dt.start();

    while(running){
        for(auto& gen_alg : ga_metadata){
            gen_alg.genetic_algorithm->run_iteration();
        }
    }

    running = false;
    dt.join();
}

void multiple_weighted_creator_main(const std::string& image_path, const std::string& mask_path, unsigned masks){
    meg::context context(true);
    std::shared_ptr<meg::opengl_drawer> drawer = std::make_shared<meg::opengl_drawer>();

    meg::data_loader loader(image_path, mask_path, masks, "intermediate");
    //meg::data_loader loader("images/istanbul.jpg", "images/istanbul.png", 9, "intermediate");
    loader.load();
    loader.generate_masks();
    loader.generate_subimages();

    std::vector<meg::multi_drawer_thread::ga_metadata> ga_metadata;

    for (auto& subimage : loader.get_subimages()){
//        meg::context window(400, 400, "Win", true);
//        meg::opengl_drawer drawer;
        std::shared_ptr<meg::texture> tex = std::make_shared<meg::texture>(subimage.data, subimage.width, subimage.height, 3);

//        while(!window.should_close()){
//            window.poll_events();
//            drawer.texture(*tex);
//            window.swap_buffers();
//        }

        auto ga = create_cps_ga(tex, drawer);
        meg::multi_drawer_thread::ga_metadata md{ga, subimage.x, subimage.y, subimage.width, subimage.height};
        ga_metadata.push_back(md);
    }

    bool running = true;
    meg::multi_drawer_thread dt{ga_metadata, 600, 400, "current_state", running,std::chrono::milliseconds{30}};
    dt.start();

    std::vector<size_t> weights(ga_metadata.size(), 0);
    std::vector<size_t> hits(ga_metadata.size(), 100);
    std::vector<double> prev_best(ga_metadata.size(), -1000000000.);
    unsigned update_frequency = 500 * ga_metadata.size();
    unsigned exit_condition = 2 * ga_metadata.size();

    std::ofstream hits_file("weights.txt");

    size_t max_weight{0};
    for(size_t iter = 0; running; ++iter){
        if(iter % update_frequency == 0){

            for(auto hit : hits){
                hits_file << hit << ' ';
            }

            hits_file << std::endl;

            size_t min_entries = std::accumulate<std::vector<size_t>::iterator, size_t>(hits.begin(), hits.end(), 0);
            if (min_entries < exit_condition){
                running = false;
                std::cout << std::endl << "Total hits: " << min_entries << std::endl;
                break;
            }
            min_entries /= hits.size();
            min_entries /= 4;

            for(size_t i = 0; i < weights.size();++i){
                weights[i] = min_entries + hits[i];
            }
            for(size_t i = 1; i < weights.size();++i){
                weights[i] += weights[i-1];
            }
            max_weight = weights.back();
            std::fill(hits.begin(), hits.end(), 0);
        }

        size_t ga_pos = kki::rng::tl.random_index(max_weight);

        for(size_t i = 0; i < ga_metadata.size(); ++i){
            if(ga_pos < weights[i]){
                ga_metadata[i].genetic_algorithm->run_iteration();
                double fitness = ga_metadata[i].genetic_algorithm->get_solution()->fitness;
                if( fitness > prev_best[i]){
                    ++hits[i];
                    prev_best[i] = fitness;
                }
                break;
            }
        }
    }

    running = false;
    dt.join();
}

