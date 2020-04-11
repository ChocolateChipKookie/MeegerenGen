//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_CPS_H
#define MEEGERENGEN_CPS_H

#include <vector>
#include "../../../util/types.h"
#include "../solution.h"

namespace meg{
    /// Concave polygon solution
    class cps : public solution{
    public:
        struct polygon
        {
            // Can only be used for simple polygons
            // In case the polygon has intersecting sides, the area won't be correct
            float area();

            color col;
            std::vector<vertex> vertices;
        };

        explicit cps(unsigned size = 0)
        {
            polygons.reserve(size);
        }

        void draw(meg::drawer& drawer) override;
        cps* clone() override;

        size_t vertices();

        color background{ 0.f, 0.f, 0.f, 0.f };
        std::vector<polygon> polygons;
    };
}



#endif //MEEGERENGEN_CPS_H
