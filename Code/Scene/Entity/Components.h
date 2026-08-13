#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"
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

    inline glm::mat4 GetTransform() const {
        glm::mat4 rotation = glm::yawPitchRoll(glm::radians(Rotation.y), glm::radians(Rotation.x), glm::radians(Rotation.z));
        return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
    }
};

struct MeshComponent {
    std::string MeshID = "";
};