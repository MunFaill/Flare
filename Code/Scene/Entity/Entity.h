#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

using Entity = std::uint32_t;

struct TagComponent {
    std::string Tag = "Object";
};

struct TransformComponent {
    glm::vec3 Position{0.0f};
    glm::vec3 Rotation{0.0f};
    glm::vec3 Scale{1.0f};

    glm::mat4 GetTransform() const;
};

struct MeshComponent {
    std::string MeshID = "";
};

struct AmbientComponent {
    glm::vec4 AmbientColor{1.0f};
};

struct MaterialComponent {
    std::string DiffuseID = "Default";
    glm::vec4 Albedo{1.0f};
    float Specular = 0.5;
};

struct DirectionalLightComponent {
    glm::vec3 LightColor{1.0f};
    glm::vec3 Diffuse{0.5f};
    glm::vec3 Specular{0.5f};
};

struct PointLightComponent {
    glm::vec3 LightColor{1.0f};
    glm::vec3 Diffuse{1.0f};
    glm::vec3 Specular{0.5f};
    float Constant = 1.0f;
    float Linear = 1.0f;
    float Quadratic = 0.5f;
};

struct CameraComponent {
    float Fov = 75.0f, Near = 0.1f, Far = 1000.0f;
    bool Current = true;
};

template<typename T>
struct ComponentStorage {
    inline T& Add(Entity entity) {
        return m_Components[entity];
    }

    inline T* Get(Entity entity) {
        auto it = m_Components.find(entity);

        if (it == m_Components.end())
            return nullptr;

        return &it->second;
    }

    inline void Remove(Entity entity) {
        m_Components.erase(entity);
    }

    inline auto begin() {
        return m_Components.begin();
    }

    inline auto end() {
        return m_Components.end();
    }
    private:
        std::unordered_map<Entity, T> m_Components;
};