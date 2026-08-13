#include "Scene/Entity/Components.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"

glm::mat4 TransformComponent::GetTransform() const {
    glm::mat4 rotation = glm::yawPitchRoll(glm::radians(Rotation.y), glm::radians(Rotation.x), glm::radians(Rotation.z));
    return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
}