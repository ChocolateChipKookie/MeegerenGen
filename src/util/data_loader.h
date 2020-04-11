//
// Created by kookie on 01/04/2020.
//

#ifndef MEEGERENGEN_DATA_LOADER_H
#define MEEGERENGEN_DATA_LOADER_H


/* Prima path na sliku i masku te broj maski na slici
 * Load:
 *      Ucitava sliku i masku
 *      Provjerava ako su dimenzije podudarne
 *
 * Generate masks:
 *      Provjerava ako je broj maski odgovarajuc.
 *      Ako nije, da nije javlja se useru o situaciji
 *
 *      Top n boja tj maski se sejvaju u mapu: Generated masks
 *      Takoder se sejvaju u vektor maski masks
 *
 * Struct subimage:
 *      DATA:   data
 *      x, y, width, height: ocito je o cemu se radi
 *
 * Generate subimage
 *      Na temelju dane maske se prvo odreduju dimenzije podslike i njena lokacija
 *      Stvara se struktura subimage
 *      Alocira se memorija za podsliku
 *      Generira se podslika
 *      Sejva se u folder subimages i u vektor podslika
 *
 * Free all:
 *      Svi alocirani resursi se oslobadaju
 */

#include <string>
#include <utility>
#include <vector>

namespace meg{
    class data_loader {
    public:

        struct subimage {

            unsigned char* data;
            int x, y, width, height, channels;

            subimage(int x, int y, int width, int height, int channels = 3)
                    : x(x), y(y), width(width), height(height), channels(channels), data(new unsigned char[width * height * channels]) {}

            subimage(int width, int height, int channels = 3)
                    : subimage(0, 0, width, height, channels){}

            subimage(subimage&& other) noexcept
                : x(other.x), y(other.y), width(other.width), height(other.height), channels(other.channels), data(other.data)
            {
                other.data = nullptr;
            }

            subimage(const subimage& other) = delete;
            subimage operator=(const subimage& other) = delete;
            subimage operator=(subimage&& other) = delete;

            ~subimage(){
                free();
            }

            void free(){
                delete[]data;
                data = nullptr;
            }
        };

        data_loader(std::string image_path, std::string mask_path, unsigned masks, std::string intermediate_file = "");

        void load();
        void generate_masks();
        void generate_subimages();
        void free_all();
        void free_image();
        void free_mask();
        void free_masks();
        void free_subimages();

        std::vector<subimage>& get_subimages(){return subimages;}

    private:

        void generate_image(std::vector<bool>& mask, size_t index);

        std::string image_path_;
        std::string mask_path_;
        bool save_intermed;
        std::string intermediate_file_;

        unsigned n_masks_;

        unsigned char* image_{};
        unsigned char* mask_{};

        int width_{}, height_{};
        int image_channels_{};
        int mask_channels_{};

        std::vector<std::vector<bool>> masks;
        std::vector<subimage> subimages;
    };
}



#endif //MEEGERENGEN_DATA_LOADER_H
