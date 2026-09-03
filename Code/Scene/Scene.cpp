#include "Scene/Scene.h"
#include "Scene/Entities/Entity.h"
#include <memory>

Entity* Scene::CreateEntity(const std::string& Label) {
    auto entity = std::make_unique<Entity>();
    entity->Label = Label;
    entity->RootScene = this;

    Entity* ptr = entity.get();

    m_Entities.push_back(std::move(entity));

    return ptr;
}

void Scene::DeleteEntity(Entity* entity)
{
    if (!entity)
        return;

    if (entity->Parent)
        entity->Parent->RemoveChild(entity);

    std::erase_if(m_Entities,
        [entity](const std::unique_ptr<Entity>& e)
        {
            return e.get() == entity;
        });
}