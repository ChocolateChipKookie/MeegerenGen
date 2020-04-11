//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_DRAWER_THREAD_H
#define MEEGERENGEN_DRAWER_THREAD_H

#include <iostream>
#include <utility>
#include "thread.h"
#include "../ga/mbga.h"
#include "../ga/solution/cps/cps.h"
#include "../graphics/context.h"
#include "../graphics/drawer/opengl_drawer.h"
#include "../graphics/drawer/svg_drawer.h"

namespace meg{



    template<typename _Rep, typename _Period>
    class drawer_thread : public kki::thread {
    public:

        drawer_thread
            (   drawable_genetic_algorithm<cps>& genetic_algorithm,
                int width,
                int height,
                std::string  win_name,
                bool& running,
                std::chrono::duration<_Rep, _Period> sleep_time = std::chrono::milliseconds{30}
            )
                :   genetic_algorithm_{genetic_algorithm},
                    width_{width},
                    height_{height},
                    win_name_{std::move(win_name)},
                    running_{running},
                    sleep_time_{sleep_time}
        {
        }

        void run() override{
            double best = 0.;
            double best_tmp;

            context window_{width_, height_, win_name_, true};
            opengl_drawer drawer_;

            window_.bind();
            while(running_)
            {
                window_.poll_events();

                if (window_.should_close())
                {
                    running_ = false;
                    break;
                }

                {
                    std::unique_lock<std::mutex> ul{ genetic_algorithm_.get_solution_mutex() };
//                    drawer_.set_viewport(window_.get_width(), window_.get_height());
                    drawer_.clear({1, 1, 0, 1});
                    genetic_algorithm_.get_solution()->draw(drawer_);
                    best_tmp = genetic_algorithm_.get_solution()->fitness;
                }

                if(best != best_tmp)
                {
                    std::cout << std::string(30, 8) << genetic_algorithm_.get_iteration() << ". " << best_tmp << std::flush;
                    best = best_tmp;
                }

                window_.swap_buffers();
                std::this_thread::sleep_for(sleep_time_);
            }
            running_ = false;

            svg_drawer svgd(1200, 800);

            {
                std::unique_lock<std::mutex> ul{ genetic_algorithm_.get_solution_mutex() };
//                drawer_.set_viewport(window_.get_width(), window_.get_height());
                genetic_algorithm_.get_solution()->draw(svgd);
            }
            svgd.save("output.svg");
        }

        void stop(){
            running_ = false;
        }

    private:

        drawable_genetic_algorithm<cps>& genetic_algorithm_;

        int width_, height_;
        std::string win_name_;
        bool& running_;
        std::chrono::duration<_Rep, _Period> sleep_time_;
    };
}




#endif //MEEGERENGEN_DRAWER_THREAD_H
