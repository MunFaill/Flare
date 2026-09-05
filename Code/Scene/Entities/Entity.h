#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include "Scene/Entities/Components.h"

class Scene;

class Entity {
    private:
        struct IComponent {
            virtual ~IComponent() = default;
        };

        template<typename T>
        struct ComponentHolder : IComponent {
            T Data;

            template<typename... Args>
            ComponentHolder(Args&&... args)
                : Data(std::forward<Args>(args)...) {}
        };

        std::unordered_map<std::type_index, std::unique_ptr<IComponent>> m_Components;

    public:
        std::string Label = "UnnamedEntity";

        Entity* Parent = nullptr;
        Scene* RootScene = nullptr;

        std::vector<Entity*> Children;

        Entity() = default;
        virtual ~Entity() = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            const std::type_index type = typeid(T);

            auto it = m_Components.find(type);

            if (it != m_Components.end()) {
                return static_cast<ComponentHolder<T>*>(it->second.get())->Data;
            }

            auto component = std::make_unique<ComponentHolder<T>>(
                std::forward<Args>(args)...
            );

            T& reference = component->Data;

            m_Components[type] = std::move(component);

            return reference;
        }

        template<typename T>
        T* GetComponent() {
            auto it = m_Components.find(typeid(T));

            if (it == m_Components.end())
                return nullptr;

            return &static_cast<ComponentHolder<T>*>(it->second.get())->Data;
        }

        template<typename T>
        bool HasComponent() const {
            return m_Components.contains(typeid(T));
        }

        template<typename T>
        void RemoveComponent() {
            m_Components.erase(typeid(T));
        }

        void AddChild(Entity* child);
        Entity* GetChild(const std::string& label);
        void RemoveChild(Entity* child);
        glm::mat4 GetWorldTransform();
};