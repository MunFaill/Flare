#pragma once

#include "Scene/Entity/Entity.h"

#include <unordered_map>

template<typename T>
class ComponentStorage {
    public:
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
