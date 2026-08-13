#pragma once

#include <string>
#include <glm/glm.hpp>

struct TagComponent {
    std::string Tag;
};

struct TransformComponent {
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
};

struct MeshComponent {
    std::string MeshID;
};