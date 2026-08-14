#pragma once

#include <string>
#include <glm/glm.hpp>

struct TagComponent {
    std::string Tag = "Object";
};

struct CameraComponent {
    float Fov = 70.0f, Near = 0.1f, Far = 1000.0f;
    bool Current = true;
};

struct TransformComponent {
    glm::vec3 Position{0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation{0.0f, 0.0f, 0.0f};
    glm::vec3 Scale{1.0f, 1.0f, 1.0f};

    glm::mat4 GetTransform() const;
};

struct MeshComponent {
    std::string MeshID = "";
};

struct MaterialComponent {
    std::string TextureID = "Default";
    glm::vec4 Albedo = {1.0f, 1.0f, 1.0f, 1.0f};
};