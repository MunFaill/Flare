#pragma once

#include "Scene/Entity/ComponentStorage.h"
#include "Scene/Entity/Components.h"
#include "Scene/Entity/Entity.h"

class Scene {
    public:
        inline Entity CreateEntity() {
            return m_NextEntity++;
        }

        template<typename T>
        inline T& AddComponent(Entity entity) {}
    private:
        template<typename T>
        inline ComponentStorage<T>& GetStorage() {
            return std::get<ComponentStorage<T>>(m_Components);
        }

        Entity m_NextEntity = 0;

        std::tuple<
            ComponentStorage<TagComponent>,
            ComponentStorage<TransformComponent>,
            ComponentStorage<MeshComponent>
        > m_Components;
};