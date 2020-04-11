//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_SOLUTION_H
#define MEEGERENGEN_SOLUTION_H
#include "../../graphics/drawer/drawer.h"

namespace meg{
    class solution {
    public:
        virtual void draw(meg::drawer& drawer) = 0;
        virtual solution* clone() = 0;
        virtual ~solution() = default;
        double fitness{0};
        double value{0};
        bool evaluated{false};
    };
}

#endif //MEEGERENGEN_SOLUTION_H
