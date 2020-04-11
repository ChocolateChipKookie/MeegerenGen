//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_SHADER_H
#define MEEGERENGEN_SHADER_H

#include <string>

namespace meg{
    class shader {
    public:
        explicit shader(const std::string& shader_path, bool save_code = false);
        ~shader();

        unsigned get_id() const {return id_;};

        // activate the shader
        void use() const;

        // utility uniform functions
        void set_bool(const std::string& name, bool value) const;
        void set_int(const std::string& name, int value) const;
        void set1f(const std::string& name, float value) const;
        void set2f(const std::string& name, float x, float y) const;
        void set3f(const std::string& name, float x, float y, float z) const;
        void set4f(const std::string& name, float x, float y, float z, float w) const;

    private:
        unsigned int id_;
        static void check_compile_errors(unsigned int shader, const std::string& type);

        std::string vertex_code;
        std::string fragment_code;
    };
}




#endif //MEEGERENGEN_SHADER_H
