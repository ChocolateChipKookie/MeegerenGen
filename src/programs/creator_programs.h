//
// Created by kookie on 02/04/2020.
//

#ifndef MEEGERENGEN_CREATOR_PROGRAMS_H
#define MEEGERENGEN_CREATOR_PROGRAMS_H

#include <memory>
#include "../ga/drawable_genetic_algorithm.h"
#include "../ga/solution/cps/cps.h"
#include "../graphics/texture.h"
#include "../graphics/drawer/opengl_drawer.h"

std::shared_ptr<meg::drawable_genetic_algorithm<meg::cps>> create_cps_ga(std::shared_ptr<meg::texture> target_tex, std::shared_ptr<meg::opengl_drawer> ogld);

void creator_main(const std::string& image_path);

void multiple_creator_main(const std::string& image_path, const std::string& mask_path, unsigned masks);
void multiple_weighted_creator_main(const std::string& image_path, const std::string& mask_path, unsigned masks);

#endif //MEEGERENGEN_CREATOR_PROGRAMS_H
