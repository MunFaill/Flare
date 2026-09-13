#include "Renderer/Pipeline/RendererPipeline.h"
#include "Renderer/Frames/RendererFrame.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>

#include "IO/Windowing/Window.h"
#include "ECS/Components.h"

void RenderPipeline::Init(Window& window) {
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

void RenderPipeline::Update(flecs::world& world) {
    RenderFrame frame = BuildFrame(world);

    m_Context->Clear({0.0f, 0.0f, 0.0f, 1.0f});

    m_AmbientPass->Execute(world, frame);
    m_GeometryPass->Execute(world, frame);
}

void RenderPipeline::Shutdown() {
    m_GeometryPass.reset();
    m_AmbientPass.reset();

    m_Context.reset();
}

RenderFrame RenderPipeline::BuildFrame(flecs::world& world) {
    RenderFrame frame;

    float aspect = static_cast<float>(m_Window->Width) / static_cast<float>(m_Window->Height);

    // Camera
    world.each([&](flecs::entity e, TransformComponent& Transform, CameraComponent& Camera) {

        frame.Camera.Position = glm::vec3(Transform.GetTransform()[3]);

        frame.Camera.View = glm::inverse(Transform.GetTransform());

        frame.Camera.Projection = glm::perspective(glm::radians(Camera.FOV), aspect, Camera.Near, Camera.Far);

        frame.Camera.ViewProjection = frame.Camera.Projection * frame.Camera.View;

        frame.Camera.FOV = Camera.FOV;
        frame.Camera.Near = Camera.Near;
        frame.Camera.Far = Camera.Far;

        frame.HasCamera = true;
    });

    // Directional Light
    world.each([&](flecs::entity e, TransformComponent& Transform, DirectionalLightComponent& Light) {
        frame.DirectionalLight.Direction = Transform.Rotation;

        frame.DirectionalLight.Color = Light.LightColor;

        frame.DirectionalLight.Specular = Light.Specular;

        frame.HasDirectionalLight = true;
    });

    // Point Lights
    world.each([&](flecs::entity e, TransformComponent& Transform, PointLightComponent& Light) {
        if (frame.PointLights.size() >= 8)
            return;

        PointLightData data;

        data.Position = glm::vec3(Transform.GetTransform()[3]);

        data.Color = Light.LightColor;
        data.Specular = Light.Specular;

        data.Constant = Light.Constant;
        data.Linear = Light.Linear;
        data.Quadratic = Light.Quadratic;

        frame.PointLights.push_back(data);
    });

    return frame;
}