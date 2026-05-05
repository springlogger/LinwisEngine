#include <lw/helpers/LoadObj.h>
#include <lw/scene/MeshGeometry.h>
#include <lw/math/Vector2.h>
#include <lw/math/Vector3.h>

#include <unordered_map>
#include <fstream>
#include <vector>
#include <cstdio>

namespace lw
{

Mesh loadObj(std::string filename)
{
    if (filename.substr(filename.find_last_of(".") + 1) != "obj")
        return Mesh({}, Material());

    std::ifstream file(filename);
    if (!file.is_open())
        return Mesh({}, Material());

    std::vector<Vector3> positions;
    std::vector<Vector2> tuxtureCoords;
    std::vector<Vector3> normals;

    std::unordered_map<std::string, uint32_t> vertexCache;
    std::vector<MeshVertex> uniqueVertices;
    std::vector<uint32_t> indices;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.size() < 2)
            continue;

        if (line[0] == 'v' && line[1] == ' ')
        {
            float x = 0, y = 0, z = 0;
            sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
            positions.push_back(Vector3(x, y, z));
        }
        else if (line[0] == 'v' && line[1] == 't' && line[2] == ' ')
        {
            float u = 0, v = 0;
            sscanf(line.c_str(), "vt %f %f", &u, &v);
            tuxtureCoords.push_back(Vector2(u, v));
        }
        else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
        {
            float x = 0, y = 0, z = 0;
            sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
            normals.push_back(Vector3(x, y, z));
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            char tokens[4][64] = {};
            int count = sscanf(line.c_str(), "f %63s %63s %63s %63s",
                tokens[0], tokens[1], tokens[2], tokens[3]);

            if (count < 3)
                continue;

            uint32_t faceIndices[4] = {};
            for (int i = 0; i < count; i++)
            {
                auto it = vertexCache.find(tokens[i]);
                if (it != vertexCache.end())
                {
                    faceIndices[i] = it->second; // сохраняем индекс из словаря
                }
                else
                {
                    int posIdx = 0, uvIdx = 0, normalIdx = 0;
                    sscanf(tokens[i], "%d/%d/%d", &posIdx, &uvIdx, &normalIdx);

                    MeshVertex vertex;
                    if (posIdx > 0)                        vertex.position = positions[posIdx - 1];
                    if (uvIdx > 0 && !tuxtureCoords.empty())   vertex.uv       = tuxtureCoords[uvIdx - 1];
                    if (normalIdx > 0 && !normals.empty()) vertex.normal   = normals[normalIdx - 1];

                    uint32_t newIndex = static_cast<uint32_t>(uniqueVertices.size());
                    uniqueVertices.push_back(vertex);
                    vertexCache[tokens[i]] = newIndex;
                    faceIndices[i] = newIndex;
                }
            }

            // First triangle
            indices.push_back(faceIndices[0]);
            indices.push_back(faceIndices[1]);
            indices.push_back(faceIndices[2]);

            // Quad → fan-triangulate into second triangle
            if (count == 4)
            {
                indices.push_back(faceIndices[0]);
                indices.push_back(faceIndices[2]);
                indices.push_back(faceIndices[3]);
            }
        }
    }

    MeshGeometry geometry;
    geometry.vertices = uniqueVertices;
    geometry.indices  = indices;

    return Mesh(geometry, Material());
}

} // namespace lw
