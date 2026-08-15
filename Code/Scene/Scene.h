#pragma once

#include "Scene/Entity/Entity.h"

#include <tuple>

struct Scene{
    inline Entity CreateEntity() {
        return m_NextEntity++;
    }
    
    template<typename T>
    inline T& AddComponent(Entity entity) {
        return GetStorage<T>().Add(entity);
    }

    template<typename T>
    inline bool HasComponent(Entity entity) {
        return GetStorage<T>().Get(entity) != nullptr; 
    }

    template<typename T>
    inline T* GetComponent(Entity entity) {
        return GetStorage<T>().Get(entity);
    }

    template<typename T>
    inline void RemoveComponent(Entity entity) {
        GetStorage<T>().Remove(entity);
    }

    template<typename T, typename... OtherTypes, typename Func>
    inline void Each(Func&& function) {
        ComponentStorage<T>& storage = GetStorage<T>();
    
        for (auto& [entity, component] : storage) {
            if ((this->template HasComponent<OtherTypes>(entity) && ...)) {
                function(
                    entity, 
                    component, 
                    *this->template GetComponent<OtherTypes>(entity)... 
                );
            }
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
            ComponentStorage<MeshComponent>,
            ComponentStorage<CameraComponent>,
            ComponentStorage<MaterialComponent>,
            ComponentStorage<DirectionalLightComponent>
        > m_Components;
};