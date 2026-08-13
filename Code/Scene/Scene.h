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
        inline T& AddComponent(Entity entity) {
            return GetStorage<T>().Add(entity);
        }

        template<typename T>
        T* GetComponent(Entity entity)
        {
            return GetStorage<T>().Get(entity);
        }

        template<typename T>
        void RemoveComponent(Entity entity)
        {
            GetStorage<T>().Remove(entity);
        }

        template<typename T, typename Func>
        void Each(Func&& function) {
            ComponentStorage<T>& storage = GetStorage<T>();

            for (auto& [entity, component] : storage) {
                function(entity, component);
            }
        }

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