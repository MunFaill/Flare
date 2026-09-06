#include "Renderer/System/RendererPipeline.h"
#include "Renderer/Frames/RendererFrame.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>

#include "Platform/Windowing/Window.h"
#include "Scene/Entities/Components.h"
#include "Scene/Scene.h"

void RendererSystem::Init(Window& window) {
    m_Context = DeviceContext::Create();
    m_Context->Initialize(window);

    m_Context->CullFaces(true);
    m_Context->DepthTest(true);
    m_Context->Blend(true);

    m_Window = &window;

    m_AmbientPass =
        std::make_unique<AmbientPass>(*m_Context);

    m_GeometryPass =
        std::make_unique<GeometryPass>(*m_Context);
}

void RendererSystem::Update(Scene& scene) {
    RenderFrame frame = BuildFrame(scene);

    m_Context->Clear({0.0f, 0.0f, 0.0f, 1.0f});

    m_AmbientPass->Execute(scene, frame);
    m_GeometryPass->Execute(scene, frame);
}

void RendererSystem::Shutdown() {
    m_GeometryPass.reset();
    m_AmbientPass.reset();

    m_Context.reset();
}

RenderFrame RendererSystem::BuildFrame(Scene& scene) {
    RenderFrame frame;

    float aspect = static_cast<float>(m_Window->Width) / static_cast<float>(m_Window->Height);

    // Camera
    for (auto& entity : scene.GetEntities()) {
        if (!entity->HasComponent<CameraComponent>() ||
            !entity->HasComponent<TransformComponent>()) {
            continue;
        }

        CameraComponent* camera = entity->GetComponent<CameraComponent>();

        glm::mat4 world = entity->GetWorldTransform();

        frame.Camera.Position = glm::vec3(world[3]);

        frame.Camera.View = glm::inverse(world);

        frame.Camera.Projection = glm::perspective(     glm::radians(camera->FOV), aspect, camera->Near, camera->Far);

        frame.Camera.ViewProjection = frame.Camera.Projection * frame.Camera.View;

        frame.Camera.FOV = camera->FOV;
        frame.Camera.Near = camera->Near;
        frame.Camera.Far = camera->Far;

        frame.HasCamera = true;

        break;
    }

    // Directional Light
    for (auto& entity : scene.GetEntities()) {
        if (!entity->HasComponent<DirectionalLightComponent>() ||
            !entity->HasComponent<TransformComponent>()) {
            continue;
        }

        TransformComponent* transform = entity->GetComponent<TransformComponent>();

        DirectionalLightComponent* light = entity->GetComponent<DirectionalLightComponent>();

        frame.DirectionalLight.Direction = transform->Rotation;

        frame.DirectionalLight.Color = light->LightColor;

        frame.DirectionalLight.Specular = light->Specular;

        frame.HasDirectionalLight = true;

        break;
    }

    // Point Lights
    for (auto& entity : scene.GetEntities()) {
        if (!entity->HasComponent<PointLightComponent>() ||
            !entity->HasComponent<TransformComponent>()) {
            continue;
        }

        if (frame.PointLights.size() >= 8)
            break;

        TransformComponent* transform = entity->GetComponent<TransformComponent>();

        PointLightComponent* light = entity->GetComponent<PointLightComponent>();

        PointLightData data;

        data.Position = glm::vec3(entity->GetWorldTransform()[3]);

        data.Color = light->LightColor;
        data.Specular = light->Specular;

        data.Constant = light->Constant;
        data.Linear = light->Linear;
        data.Quadratic = light->Quadratic;

        frame.PointLights.push_back(data);
    }

    return frame;
}