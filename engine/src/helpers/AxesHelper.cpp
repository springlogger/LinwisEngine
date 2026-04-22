#include <lw/helpers/AxesHelper.h>

namespace lw 
{
    static LineGeometry createAxesGeometry()
    {
        LineGeometry geometry;

        geometry.vertices = {
            {{0, 0, 0}, 0x00FF0000}, {{1, 0, 0}, 0x00FF0000},
            {{0, 0, 0}, 0x0000FF00}, {{0, 1, 0}, 0x0000FF00},
            {{0, 0, 0}, 0x000000FF}, {{0, 0, 1}, 0x000000FF}
        };

        geometry.indices = {0, 1, 2, 3, 4, 5};

        return geometry;
    }

    AxesHelper::AxesHelper()
        : LineSegments(createAxesGeometry())
    {
    }
}