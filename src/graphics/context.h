//
// Created by kookie on 27/03/2020.
//

#ifndef MEEGERENGEN_CONTEXT_H
#define MEEGERENGEN_CONTEXT_H
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace meg{
    class context {
    public:
        // Context without window
        explicit context(bool alpha, int width = 10, int height = 10, const std::string& name = "");
        // Windowed context
        context(int width, int height, const std::string& name, bool alpha);
        // Destructor
        ~context();

        bool should_close() const;

        void set_process_input(void(*function)(GLFWwindow*, int, int, int, int)) const;
        void set_resize_callback(void(*function)(GLFWwindow*, int, int)) const;
        void set_should_close(bool value) const;
        void swap_buffers() const;
        void poll_events();
        //Blocks thread, waits for seconds amount of seconds for new event
        //Unblocks when event occurs
        //If seconds is < 0 it blocks the thread completly
        void wait_events(double seconds = -1.);
        void set_viewport() const;
        void resize(int width, int height) const;
        unsigned get_width() const;
        unsigned get_height() const;
        void bind();

    private:

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

        GLFWwindow* context_{nullptr};
    };
}





#endif //MEEGERENGEN_CONTEXT_H
