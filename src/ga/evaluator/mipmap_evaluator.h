//
// Created by kookie on 29/03/2020.
//

#ifndef MEEGERENGEN_MIPMAP_EVALUATOR_H
#define MEEGERENGEN_MIPMAP_EVALUATOR_H


#include <memory>
#include "evaluator.h"
#include "../../graphics/framebuffer.h"
#include "../../graphics/drawer/opengl_drawer.h"
#include <glad/glad.h>

namespace meg{

    template <typename sol_t>
    class mipmap_evaluator : public evaluator<cps>
    {
    public:
        mipmap_evaluator(
                std::shared_ptr<meg::texture>& goal_image,
                std::shared_ptr<meg::opengl_drawer>& drawer,
                int width,
                int height,
                int mipmap_level);

        ~mipmap_evaluator() override = default;

        void evaluate(sol_t* sol) override;

        void set_mipmap_level(unsigned mipmap_level);

    private:
        void init();

        unsigned width_, height_, total_;
        std::shared_ptr<opengl_drawer> drawer_;

        meg::framebuffer canvas_framebuffer_;
        meg::framebuffer sub_framebuffer_;

        std::shared_ptr<meg::texture> goal_image_;

        unsigned mipmap_level_;
        int mipmap_level_width_{ 0 };
        int mipmap_level_height_{ 0 };
        std::vector<float> data_;
    };

    template <typename sol_t>
    mipmap_evaluator<sol_t>::mipmap_evaluator
        (   std::shared_ptr<meg::texture>& goal_image,
            std::shared_ptr<meg::opengl_drawer>& drawer,
            int width,
            int height,
            int mipmap_level)
            :   width_(width),
                height_(height),
                total_(width * height),
                drawer_(drawer),
                canvas_framebuffer_(std::make_shared<texture>(width_, height_)),
                sub_framebuffer_(std::make_shared<texture>(width_, height_)),
                goal_image_(goal_image),
                mipmap_level_(mipmap_level)
    {
        init();
    }

    template <typename sol_t>
    void mipmap_evaluator<sol_t>::init()
    {
        KKI_PROFILE_FUNCTION;
        sub_framebuffer_.get_texture()->bind();
        glGenerateMipmap(GL_TEXTURE_2D);

        glGetTexLevelParameteriv(GL_TEXTURE_2D, mipmap_level_, GL_TEXTURE_WIDTH, &mipmap_level_width_);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, mipmap_level_, GL_TEXTURE_HEIGHT, &mipmap_level_height_);
        total_ = mipmap_level_width_ * mipmap_level_height_;
        data_.resize(4 * static_cast<size_t>(total_));
    }


    template <typename sol_t>
    void mipmap_evaluator<sol_t>::set_mipmap_level(unsigned mipmap_level)
    {
        KKI_PROFILE_FUNCTION;
        mipmap_level_ = mipmap_level;
        init();
    }

    template <typename sol_t>
    void mipmap_evaluator<sol_t>::evaluate(sol_t* sol)
    {
        KKI_PROFILE_FUNCTION;
        if (sol->evaluated) return;

        {
            KKI_PROFILE_SCOPE("Drawing");
            canvas_framebuffer_.bind();
            drawer_->clear({0.f, 0.f, 0.f, 1.f});
            drawer_->set_viewport(width_, height_);
            sol->draw(*drawer_);
            canvas_framebuffer_.get_texture()->generate_mip_map();

            sub_framebuffer_.bind();
            drawer_->clear({0.f, 0.f, 0.f, 1.f});
            drawer_->subtract(*canvas_framebuffer_.get_texture(), *goal_image_);
            sub_framebuffer_.get_texture()->generate_mip_map();

            sub_framebuffer_.get_texture()->bind();
            glGetTexImage(GL_TEXTURE_2D, mipmap_level_, GL_RGBA, GL_FLOAT, data_.data());
        }

        {
            KKI_PROFILE_SCOPE("Histogram");
            double histogram{0};
            for (unsigned i = 0; i < total_; ++i)
            {
                histogram += data_[4 * i + 1] + data_[4 * i + 0] + data_[4 * i + 2];
            }

            double val = histogram / static_cast<double>(this->total_) / 3.;
            sol->value = val;
            sol->fitness = -sol->value;
            sol->evaluated = true;
        }
    }
}

#endif //MEEGERENGEN_MIPMAP_EVALUATOR_H
