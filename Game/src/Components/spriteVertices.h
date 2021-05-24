#pragma once

#include <vector>

// TODO: Vector is more memory than an array so it would be good to change.
struct SpriteVertices {
    SpriteVertices(std::vector<float> verts = {}, std::vector<unsigned int> inds = {}) : vertices(verts), indices(inds) {
        // Default size
        if (verts.empty())
            vertices = {
                -25.0f, -25.0f, 0.0f, 0.0f,
                 25.0f, -25.0f, 1.0f, 0.0f,
                 25.0f,  25.0f, 1.0f, 1.0f,
                -25.0f,  25.0f, 0.0f, 1.0f
            };

        if (inds.empty())
            indices = {
                0,1,2,
                2,3,0
            };
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};
