#include <lw/helpers/LoadObj.h>
#include <lw/scene/MeshGeometry.h>

#include <fstream>
#include <vector>
#include <cstdio>

namespace lw
{

Mesh loadObj(std::string filename)
{
    if (filename.substr(filename.find_last_of(".") + 1) != "obj")
        return Mesh({}, Material());

    std::ifstream in(filename);
    if (!in.is_open())
        return Mesh({}, Material());

    std::vector<MeshVertex> vertices;
    std::vector<uint32_t> indices;

    std::string line;
    while (std::getline(in, line))
    {
        if (line.size() < 2)
            continue;

        if (line[0] == 'v' && line[1] == ' ')
        {
            float x = 0, y = 0, z = 0;
            sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
            MeshVertex vertex;
            vertex.position = Vector3(x, y, z);
            vertices.push_back(vertex);
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            char t[4][64] = {};
            int count = sscanf(line.c_str(), "f %63s %63s %63s %63s", t[0], t[1], t[2], t[3]);

            // берём только первое число перед / - индекс вершин
            auto vertexIndex = [](const char* token) -> uint32_t {
                int v = 0;
                sscanf(token, "%d", &v);
                return static_cast<uint32_t>(v - 1);
            };

            if (count >= 3) {
                uint32_t idx[4];
                for (int i = 0; i < count; i++)
                    idx[i] = vertexIndex(t[i]);

                indices.push_back(idx[0]);
                indices.push_back(idx[1]);
                indices.push_back(idx[2]);

                // Triangulate quad into two triangles (fan)
                if (count == 4) {
                    indices.push_back(idx[0]);
                    indices.push_back(idx[2]);
                    indices.push_back(idx[3]);
                }
            }
        }
    }

    MeshGeometry geometry;
    geometry.vertices = vertices;
    geometry.indices = indices;

    return Mesh(geometry, Material());
}

} // namespace lw
