#pragma once

#include <unordered_map>

class Entity;

template<typename T>
class ComponentStorage {
public:
    T& Add(Entity* entity) {
        return m_Components[entity];
    }

    T* Get(Entity* entity) {
        auto it = m_Components.find(entity);

        if (it == m_Components.end())
            return nullptr;

        return &it->second;
    }

    bool Has(Entity* entity) const {
        return m_Components.contains(entity);
    }

    void Remove(Entity* entity) {
        m_Components.erase(entity);
    }

private:
    std::unordered_map<Entity*, T> m_Components;
};