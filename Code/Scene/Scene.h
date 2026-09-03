#pragma once

#include "Scene/Entities/Entity.h"

#include <memory>

class Scene {
    public:
        Entity* CreateEntity(const std::string& Label);
        inline std::vector<std::unique_ptr<Entity>>& GetEntities() {return m_Entities;}
        void DeleteEntity(Entity* entity);
    private:
        std::vector<std::unique_ptr<Entity>> m_Entities;
};