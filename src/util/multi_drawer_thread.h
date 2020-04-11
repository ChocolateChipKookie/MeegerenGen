//
// Created by kookie on 01/04/2020.
//

#ifndef MEEGERENGEN_MULTI_DRAWER_THREAD_H
#define MEEGERENGEN_MULTI_DRAWER_THREAD_H


#include <vector>
#include "thread.h"
#include "../ga/drawable_genetic_algorithm.h"
#include "../ga/solution/cps/cps.h"
#include "../graphics/drawer/svg_drawer.h"
#include "../graphics/context.h"
#include "../graphics/drawer/opengl_drawer.h"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace meg{

    class multi_drawer_thread;
    multi_drawer_thread* mdt_global_pointer;

    class multi_drawer_thread : public kki::thread {
    public:

        struct ga_metadata{
            std::shared_ptr<drawable_genetic_algorithm<cps>> genetic_algorithm;
            int orig_x, orig_y, orig_width, orig_height;
            int x{}, y{}, width{}, height{};
        };

        multi_drawer_thread
            (   std::vector<ga_metadata>& genetic_algorithms,
                int width,
                int height,
                std::string  win_name,
                bool& running,
                std::chrono::milliseconds sleep_time = std::chrono::milliseconds{30})
                :   genetic_algorithms_(genetic_algorithms),
                    width_(width),
                    height_(height),
                    win_name_(std::move(win_name)),
                    running_(running),
                    sleep_time_(sleep_time)
        {
            mdt_global_pointer = this;

            for(auto& ga_meta : mdt_global_pointer->genetic_algorithms_){
                ga_width = std::max(ga_width, ga_meta.orig_x + ga_meta.orig_width);
                ga_height = std::max(ga_height, ga_meta.orig_y + ga_meta.orig_height);
            }

            double x_ratio = static_cast<double>(width)/static_cast<double>(ga_width);
            double y_ratio = static_cast<double>(height)/static_cast<double>(ga_height);
            mdt_global_pointer->width_ = width;
            mdt_global_pointer->height_ = height;

            for(auto& ga_meta : genetic_algorithms_){
                ga_meta.x       = static_cast<int>(ga_meta.orig_x * x_ratio);
                ga_meta.y       = static_cast<int>(ga_meta.orig_y * y_ratio);
                ga_meta.width   = static_cast<int>(ga_meta.orig_width  * x_ratio);
                ga_meta.height  = static_cast<int>(ga_meta.orig_height * y_ratio);
            }
        }

        void run() override{
            double best = 0.;
            double best_tmp;

            context window_{width_, height_, win_name_, true};
            opengl_drawer drawer_;

            window_.set_resize_callback(no_resize_callback);
            window_.bind();

            while(running_)
            {
                auto begin = std::chrono::high_resolution_clock().now();

                window_.bind();
                window_.set_viewport();
                if (window_.should_close())
                {
                    running_ = false;
                    break;
                }

                drawer_.enable_clear(true);
                drawer_.clear({0, 0, 0, 1});
                drawer_.enable_clear(false);

                for (auto& ga_meta : genetic_algorithms_){
                    std::unique_lock<std::mutex> ul{ ga_meta.genetic_algorithm->get_solution_mutex() };
                    drawer_.set_viewport(ga_meta.x, ga_meta.y, ga_meta.width, ga_meta.height);
                    ga_meta.genetic_algorithm->get_solution()->draw(drawer_);
                }

                std::cout << std::string(30, 8) << genetic_algorithms_[0].genetic_algorithm->get_iteration() << ". " << std::flush;
                window_.swap_buffers();

                std::chrono::duration<double, std::milli> total_duration = std::chrono::high_resolution_clock().now() - begin;
                double seconds = (this->sleep_time_ - total_duration).count() / 1000.;
                seconds = seconds < 0 ? 0. : seconds;
                window_.wait_events(seconds);
            }

            running_ = false;

            svg_drawer svgd(ga_width, ga_height);
            svgd.clear({0, 0, 0, 1});
            svgd.enable_clear(false);

            for (size_t i = 0; i < genetic_algorithms_.size(); ++i){
                auto& ga_meta = genetic_algorithms_[i];
                std::unique_lock<std::mutex> ul{ ga_meta.genetic_algorithm->get_solution_mutex() };
                svgd.create_group("G" + std::to_string(i));
                svgd.set_viewport(ga_meta.orig_x, ga_meta.orig_y, ga_meta.orig_width, ga_meta.orig_height);
                ga_meta.genetic_algorithm->get_solution()->draw(svgd);
                svgd.close_group();
            }

            svgd.save("output.svg");
        }

    private:
        int ga_width{}, ga_height{};

        static void no_resize_callback(GLFWwindow* window, int width, int height)
        {
            double x_ratio = static_cast<double>(width)/static_cast<double>(mdt_global_pointer->ga_width);
            double y_ratio = static_cast<double>(height)/static_cast<double>(mdt_global_pointer->ga_height);
            mdt_global_pointer->width_ = width;
            mdt_global_pointer->height_ = height;

            for(auto& ga_meta : mdt_global_pointer->genetic_algorithms_){
                ga_meta.x       = static_cast<int>(ga_meta.orig_x * x_ratio);
                ga_meta.y       = static_cast<int>(ga_meta.orig_y * y_ratio);
                ga_meta.width   = static_cast<int>(ga_meta.orig_width  * x_ratio);
                ga_meta.height  = static_cast<int>(ga_meta.orig_height * y_ratio);
            }
        }

        std::vector<ga_metadata>& genetic_algorithms_;

        int width_, height_;
        std::string win_name_;
        bool& running_;
        std::chrono::milliseconds sleep_time_;

    };
}




#endif //MEEGERENGEN_MULTI_DRAWER_THREAD_H
