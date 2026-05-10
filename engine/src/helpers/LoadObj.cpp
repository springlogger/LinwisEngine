#include <lw/helpers/LoadObj.h>
#include <lw/scene/MeshGeometry.h>
#include <lw/math/Vector2.h>
#include <lw/math/Vector3.h>

#include <unordered_map>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>

namespace lw
{

static void RecalculateSmoothNormals(
    std::vector<MeshVertex>& vertices,
    const std::vector<uint32_t>& indices,
    const std::vector<int>& vertexPositionIndices,
    std::size_t positionCount)
{
    std::vector<Vector3> accumulatedNormals(positionCount);

    for (std::size_t i = 0; i + 2 < indices.size(); i += 3)
    {
        const uint32_t i0 = indices[i];
        const uint32_t i1 = indices[i + 1];
        const uint32_t i2 = indices[i + 2];

        if (i0 >= vertices.size() ||
            i1 >= vertices.size() ||
            i2 >= vertices.size())
        {
            continue;
        }

        Vector3 faceNormal =
            (vertices[i1].position - vertices[i0].position)
                .cross(vertices[i2].position - vertices[i0].position);

        if (faceNormal.isNearZero()) {
            continue;
        }

        const Vector3 importedNormal = vertices[i0].normal;
        if (!importedNormal.isNearZero() && faceNormal.dot(importedNormal) < 0.0f) {
            faceNormal *= -1.0f;
        }

        const uint32_t triangleIndices[3] = { i0, i1, i2 };
        for (uint32_t vertexIndex : triangleIndices)
        {
            const int positionIndex = vertexPositionIndices[vertexIndex];
            if (positionIndex >= 0 &&
                static_cast<std::size_t>(positionIndex) < accumulatedNormals.size())
            {
                accumulatedNormals[positionIndex] += faceNormal;
            }
        }
    }

    for (std::size_t i = 0; i < vertices.size(); ++i)
    {
        const int positionIndex = vertexPositionIndices[i];
        if (positionIndex < 0 ||
            static_cast<std::size_t>(positionIndex) >= accumulatedNormals.size())
        {
            continue;
        }

        const Vector3 normal = accumulatedNormals[positionIndex].normalized();
        if (!normal.isNearZero()) {
            vertices[i].normal = normal;
        }
    }
}

Mesh loadObj(const std::string& filename)
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
    std::vector<int> vertexPositionIndices;
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
                    faceIndices[i] = it->second;
                }
                else
                {
                    int posIdx = 0, uvIdx = 0, normalIdx = 0;
                    sscanf(tokens[i], "%d/%d/%d", &posIdx, &uvIdx, &normalIdx);

                    MeshVertex vertex;
                    if (posIdx > 0)                            vertex.position = positions[posIdx - 1];
                    if (uvIdx > 0 && !tuxtureCoords.empty())   vertex.uv       = tuxtureCoords[uvIdx - 1];
                    if (normalIdx > 0 && !normals.empty())     vertex.normal   = normals[normalIdx - 1];

                    uint32_t newIndex = static_cast<uint32_t>(uniqueVertices.size());
                    uniqueVertices.push_back(vertex);
                    vertexPositionIndices.push_back(posIdx > 0 ? posIdx - 1 : -1);
                    vertexCache[tokens[i]] = newIndex;
                    faceIndices[i] = newIndex;
                }
            }

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

    RecalculateSmoothNormals(
        uniqueVertices,
        indices,
        vertexPositionIndices,
        positions.size()
    );

    MeshGeometry geometry;
    geometry.vertices = uniqueVertices;
    geometry.indices  = indices;

    return Mesh(geometry, Material());
}

Mesh loadObj(const std::string& filename, const std::string& texturePath)
{
    Mesh mesh = loadObj(filename);
    mesh.setTexture(texturePath);
    return mesh;
}

} // namespace lw
