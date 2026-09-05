#include "Scene/Entities/Entity.h"

void Entity::AddChild(Entity* child) {
    if (!child) return;

    if (child->Parent) child->Parent->RemoveChild(child);

    child->Parent = this;
    Children.push_back(child);
}

Entity* Entity::GetChild(const std::string& label) {
    for (Entity* child : Children) {
        if (child->Label == label) {
            return child;
        }
    }
    return nullptr;
}

void Entity::RemoveChild(Entity* child) {
    if (!child) return;

    std::erase(Children, child);

    if (child->Parent == this)
        child->Parent = nullptr;
}

glm::mat4 Entity::GetWorldTransform() {
    if (!HasComponent<TransformComponent>()) {
        return glm::mat4(1.0f);
    }

    glm::mat4 LTransform = GetComponent<TransformComponent>()->GetTransform();

    if (Parent != nullptr) {
        return Parent->GetWorldTransform() * LTransform;
    }

    return LTransform;
}