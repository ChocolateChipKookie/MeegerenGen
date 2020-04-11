//
// Created by kookie on 01/04/2020.
//

#include "data_loader.h"
#include "../graphics/util/graphics_error.h"
#include <iostream>
#include <array>
#include <map>
#include <unordered_map>
#include <stb_image_write.h>
#include "../graphics/util/image_loader.h"

meg::data_loader::data_loader(std::string image_path, std::string mask_path, unsigned masks, std::string intermediate_file)
        :   image_path_(std::move(image_path)),
            mask_path_(std::move(mask_path)),
            save_intermed(!intermediate_file.empty()),
            intermediate_file_(std::move(intermediate_file)),
            n_masks_(masks) {}

void meg::data_loader::load() {
    auto image = meg::image_loader::load_image(image_path_);
    image_ = image.pixels;

    if(!image_){
        throw graphics_error{"Image with given filepath doesn't exist"};
    }

    auto mask = meg::image_loader::load_image(mask_path_);
    mask_ = mask.pixels;

    if(!mask_){
        throw graphics_error{"Image with given filepath doesn't exist"};
    }

    if(image.width != mask.width || image.height != mask.height){
        throw graphics_error{"Image and mask are not same size"};
    }

    image_channels_ = image.channels;
    mask_channels_ = mask.channels;

    width_ = image.width;
    height_ = image.height;
}

/* Generate masks:
 *      Provjerava ako je broj maski odgovarajuc.
 *      Ako nije, da nije javlja se useru o situaciji
 *
 *      Top n boja tj maski se sejvaju u mapu: Generated masks
 *      Takoder se sejvaju u vektor maski masks
 */

namespace std {

    template <>
    struct hash<std::array<unsigned char, 3>>
    {
        std::size_t operator()(const std::array<unsigned char, 3>& k) const
        {
            return k[0] << 16 | k[1] << 8 | k[2] ;
        }
    };

}


void meg::data_loader::generate_masks() {
    std::unordered_map<std::array<unsigned char, 3>, size_t> colors;

    size_t bytes = height_ * width_ * mask_channels_;

    for (size_t i = 0; i < bytes; i += mask_channels_){
        colors[{mask_[i], mask_[i + 1], mask_[i + 2]}]++;
    }

    if (colors.size() != n_masks_){
        std::cout << "Mask number not the same as the number of colors in picture! Defined: " << n_masks_ << " Found: " << colors.size() << std::endl;
        std::cout << "Check intermediate forlder if the generated masks are OK" << std::endl;
    }

    std::vector<std::pair<std::array<unsigned char, 3>, size_t>> color_vec_numbers;
    std::copy(colors.begin(), colors.end(), std::back_inserter(color_vec_numbers));
    std::sort(color_vec_numbers.begin(), color_vec_numbers.end(), [](auto& c1, auto& c2){return c1.second > c2.second;});

    std::vector<std::array<unsigned char, 3>> color_vec;
    color_vec.reserve(n_masks_);

    for(size_t i = 0; i < n_masks_; ++i){
        color_vec.emplace_back(color_vec_numbers[i].first);
    }

    unsigned char* data = nullptr;
    if(save_intermed){
        data = new unsigned char[width_ * height_];
        stbi_flip_vertically_on_write(1);
    }

    masks.resize(n_masks_);

    for (unsigned index = 0; index < n_masks_; ++index){
        //  Za svaku boju:
        masks[index].resize(height_ * width_);

        for (unsigned i = 0, mask_i = 0; i < height_ * width_ ; ++i, mask_i += mask_channels_){
            bool is_color =
                    color_vec[index][0] ==  mask_[mask_i] &&
                    color_vec[index][1] ==  mask_[mask_i + 1] &&
                    color_vec[index][2] ==  mask_[mask_i + 2];
            masks[index][i] = is_color;

            if (save_intermed){
                data[i] = is_color ? 255 : 0;
            }
        }

        if (save_intermed){
            std::string file = intermediate_file_ + "/mask_" + std::to_string(index) + ".png";
            stbi_write_png(file.c_str(), width_, height_, 1, data, width_);
        }
    }

    delete[]data;
}

void meg::data_loader::generate_subimages() {
    for(unsigned i = 0; i < masks.size(); ++i){
        generate_image(masks[i], i);
    }
}

void meg::data_loader::generate_image(std::vector<bool> &mask, size_t index) {
    int min_x, min_y, max_x, max_y;

    for (min_x = 0; min_x < width_; ++min_x) {
        size_t location = min_x;
        for (unsigned i = 0; i < height_; ++i, location += width_) {
            if (mask[location]) {
                goto min_x_end;
            }
        }
    }
    min_x_end:

    for (min_y = 0; min_y < height_; ++min_y) {
        size_t location = min_y * width_;
        for (unsigned i = 0; i < width_; ++i, ++location) {
            if (mask[location]) {
                goto min_y_end;
            }
        }
    }
    min_y_end:

    for (max_x = width_ - 1; max_x >= 0; --max_x) {
        size_t location = max_x;
        for (unsigned i = 0; i < height_; ++i, location += width_) {
            if (mask[location]) {
                goto max_x_end;
            }
        }
    }
    max_x_end:
    ++max_x;

    for (max_y = height_ - 1; max_y < height_; --max_y) {
        size_t location = max_y * width_;
        for (unsigned i = 0; i < width_; ++i, ++location) {
            if (mask[location]) {
                goto max_y_end;
            }
        }
    }
    max_y_end:
    ++max_y;

    int new_width = max_x - min_x;
    if(new_width % 4 != 0){
        //Sirina zaslona
        if(width_ - new_width < 4){
            new_width = new_width / 4 * 4;
            int diff = max_x - min_x - new_width;
            min_x += diff/2;
        }
        else{
            new_width = (new_width / 4 + 1) * 4;
            int diff = (new_width - (max_x - min_x));
            int shift_right = diff/2;
            int shift_left = diff - shift_right;

            //Skroz desno
            if(min_x < shift_right){
                shift_right = min_x;
            }
            else if((width_ - max_x) < shift_left){
                shift_right -= (shift_left - (width_ - max_x));
            }
            min_x -= shift_right;
        }
    }
    int new_height = max_y - min_y;
    new_height = new_height / 4 * 4;
    const int channels = 3;

    subimages.emplace_back(min_x, min_y, new_width, new_height, channels);
    subimage &si = subimages.back();


    for (int y = 0; y < new_height; ++y) {
        int offset = y * new_width;
        int offset_image = (y + min_y) * width_ + min_x;

        for (int x = 0; x < new_width; ++x) {
            int index_subimage = (offset + x) * channels;
            if (mask[offset_image + x]) {
                int index_image = (offset_image + x) * image_channels_;
                si.data[index_subimage] = image_[index_image];
                si.data[index_subimage + 1] = image_[index_image + 1];
                si.data[index_subimage + 2] = image_[index_image + 2];
            } else {
                si.data[index_subimage] = 0;
                si.data[index_subimage + 1] = 0;
                si.data[index_subimage + 2] = 0;
            }
        }
    }

    if (save_intermed) {
        std::string file = intermediate_file_ + "/image_" + std::to_string(index) + ".png";
        stbi_write_png(file.c_str(), new_width, new_height, channels, si.data, new_width * channels);
    }

}

void meg::data_loader::free_all() {
    free_image();
    free_mask();
    free_masks();
    free_subimages();
}

void meg::data_loader::free_image() {
    meg::image_loader::free_image(image_);
    image_ = nullptr;
}

void meg::data_loader::free_mask() {
    meg::image_loader::free_image(mask_);
    mask_ = nullptr;
}

void meg::data_loader::free_masks() {
    for(auto& mask : masks){
        mask.resize(0);
        mask.shrink_to_fit();
    }
}

void meg::data_loader::free_subimages() {
    for(auto& subimage : subimages){
        subimage.free();
    }
}
