cmake_minimum_required(VERSION 3.15)
project(MeegerenGen)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        vendor/glad/include
        vendor/stb
)

#add_definitions(-DKKI_PROFILE)

add_executable(MeegerenGen
        #Main
        src/main.cpp

        #Util
        src/graphics/util/graphics_error.h
        src/util/types.h

        #Graphics
        src/graphics/glad.c
        src/graphics/context.cpp
        src/graphics/context.h

            #Util
            src/graphics/util/image_loader.cpp
            src/graphics/util/image_loader.h

            #Drawers
            src/graphics/drawer/drawer.h
            src/graphics/drawer/opengl_drawer.cpp
            src/graphics/drawer/opengl_drawer.h
            src/graphics/drawer/svg_drawer.cpp
            src/graphics/drawer/svg_drawer.h

            #Additionals
            src/graphics/texture.cpp
            src/graphics/texture.h
            src/graphics/shader.cpp
            src/graphics/shader.h
            src/graphics/framebuffer.cpp
            src/graphics/framebuffer.h

        #Genetic algorithm
        src/ga/genetic_algorithm.h
        src/ga/mbga.h
        src/ga/mutation/mutation.h
        src/ga/solution/solution.h
        src/ga/evaluator/evaluator.h
        src/ga/selection/selection.h
        src/ga/drawable_genetic_algorithm.h
        src/ga/self_adjustable_ga.h
        src/ga/cleaner_saga.h
        src/ga/cleaner_saga.cpp

        #Conditions
        src/ga/util/ga_bool_ref_condition.h
        src/ga/util/ga_time_condition.h

        #Selections
        src/ga/selection/tournament_selection.h
        src/ga/selection/random_selection.h
        src/ga/selection/roulette_selection.h


        #General mutations
        src/ga/mutation/obligatory_composite_mutation.h
        src/ga/mutation/composite_mutation.h
        src/ga/mutation/self_adjustable_mutation.h

        #Solution
            src/ga/solution/cps/cps.cpp
            src/ga/solution/cps/cps.h

            #Mutations
            src/ga/solution/cps/mutations/add_polygon_mutation.cpp
            src/ga/solution/cps/mutations/add_polygon_mutation.h
            src/ga/solution/cps/mutations/add_vertex_mutation.cpp
            src/ga/solution/cps/mutations/add_vertex_mutation.h
            src/ga/solution/cps/mutations/background_color_mutation.cpp
            src/ga/solution/cps/mutations/background_color_mutation.h
            src/ga/solution/cps/mutations/color_mutation.cpp
            src/ga/solution/cps/mutations/color_mutation.h
            src/ga/solution/cps/mutations/curving_vertex_mutation.cpp
            src/ga/solution/cps/mutations/curving_vertex_mutation.h
            src/ga/solution/cps/mutations/point_mutation.cpp
            src/ga/solution/cps/mutations/point_mutation.h
            src/ga/solution/cps/mutations/remove_polygon_mutation.cpp
            src/ga/solution/cps/mutations/remove_polygon_mutation.h
            src/ga/solution/cps/mutations/remove_vertex_mutation.cpp
            src/ga/solution/cps/mutations/remove_vertex_mutation.h
            src/ga/solution/cps/mutations/swap_mutation.cpp
            src/ga/solution/cps/mutations/swap_mutation.h

        #Evaluator
        src/ga/evaluator/mipmap_evaluator.h

        #Programs
        src/programs/image_ga.cpp
        src/programs/image_ga.h
        src/programs/self_adjustuble_ga_program.cpp
        src/programs/self_adjustuble_ga_program.h
        src/programs/cleaner_saga_program.h
        src/programs/cleaner_saga_program.cpp
        src/programs/creator_programs.cpp
        src/programs/creator_programs.h

        #Util
        src/util/drawer_thread.h
        src/util/data_loader.cpp
        src/util/data_loader.h
        src/util/multi_drawer_thread.h
        )

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/glfw-3.3.2)
target_link_libraries(MeegerenGen glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(MeegerenGen OpenGL::GL)






