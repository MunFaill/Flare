#pragma once

#include <string>
#include <glm/glm.hpp>

enum AmbientType {
    Color = 0,
    Sky = 1
};

struct TransformComponent {
    glm::vec3 Position{0.0f};
    glm::vec3 Rotation{0.0f};
    glm::vec3 Scale{1.0f};

    glm::mat4 GetTransform() const;
};

struct MaterialComponent {
    std::string MaterialID = "";
};

struct MeshComponent {
    std::string MeshID = "";
};

struct ModelComponent {
    std::string ModelID = "";
};

struct CameraComponent {
    float FOV = 60.0f;
    float Near = 0.1f;
    float Far = 1000.0f;
    bool Current = true;
};

struct AmbientComponent {
    glm::vec3 AmbientColor{1.0f};
    float Exposure = 1.0f;
    std::string TextureID = "SkyTexture";
    std::string ShaderID = "Sky";
    AmbientType Type = Color;
};

struct DirectionalLightComponent {
    glm::vec3 Direction{0.0f, -1.0f, 0.0f};
    glm::vec3 LightColor{1.0f};
    glm::vec3 Specular{0.5f};
};

struct PointLightComponent {
    glm::vec3 LightColor{1.0f};
    glm::vec3 Specular{0.5f};
    float Constant = 1.0f;
    float Linear = 1.0f;
    float Quadratic = 0.5f;
};