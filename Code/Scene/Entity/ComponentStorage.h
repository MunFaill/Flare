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
    private:
        std::unordered_map<Entity, T> m_Components;
};
