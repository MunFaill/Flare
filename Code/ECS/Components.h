#pragma once

#include <string>
#include <glm/glm.hpp>

enum AmbientType {
    Color = 0,
    Sky = 1
};

enum PhysicsType {
    StaticBody = 0,
    DynamicBody = 1,
    KinematicBody = 2
};

enum CameraType {
    Perspective = 0,
    Orthogonal = 1
};

enum ShapeType {
    BoxShape = 0
};

struct TransformComponent {
    glm::vec3 Position{0.0f};
    glm::vec3 Rotation{0.0f};
    glm::vec3 Scale{1.0f};

    glm::mat4 GetLocalTransform() const;
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
    CameraType Type = Perspective;
    float FOV = 60.0f;
    float Near = 0.1f;
    float Far = 1000.0f;
    bool Current = true;
};

struct AmbientComponent {
    AmbientType Type = Color;
    std::string TextureID = "SkyTexture";
    std::string ShaderID = "Sky";
    glm::vec3 AmbientColor{1.0f};
    float Exposure = 1.0f;
};

struct PostProcessComponent {
    std::string ShaderID = "PostProcess";
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

struct CollisionComponent {
    bool Enabled = true;
    PhysicsType BodyType = StaticBody;
    ShapeType CollisonShape = BoxShape;
    glm::vec3 LinearForce{0.0f};
    glm::vec3 LinearVelocity{0.0f};
    float Density = 1.0f;
    float Friction = 0.3f;
    bool LockAngularX = false;
    bool LockAngularY = false;
    bool LockAngularZ = false;
};