#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <vector>

struct CameraData {
    glm::mat4 View{1.0f};
    glm::mat4 Projection{1.0f};
    glm::mat4 ViewProjection{1.0f};

    glm::vec3 Position{0.0f};

    float FOV = 60.0f;
    float Near = 0.1f;
    float Far = 1000.0f;
};

struct DirectionalLightData {
    glm::vec3 Direction{0.0f};
    glm::vec3 Color{1.0f};
    glm::vec3 Diffuse{1.0f};
    glm::vec3 Specular{1.0f};
};

struct PointLightData {
    glm::vec3 Position{0.0f};
    glm::vec3 Color{1.0f};

    glm::vec3 Diffuse{1.0f};
    glm::vec3 Specular{1.0f};

    float Constant = 1.0f;
    float Linear = 0.0f;
    float Quadratic = 0.0f;
};

struct RenderFrame {
    CameraData Camera;

    bool HasCamera = false;

    DirectionalLightData DirectionalLight;
    bool HasDirectionalLight = false;

    std::vector<PointLightData> PointLights;
};