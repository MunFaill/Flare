#include "Renderer/Pipeline/RendererPipeline.h"
#include "Renderer/Device/DeviceBuffers.h"
#include "Renderer/Device/DeviceTexture.h"
#include "Renderer/Frames/RendererFrame.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "IO/Windowing/Window.h"
#include "ECS/Components.h"

static std::unique_ptr<Texture> PostPorcessTextureObj;
static std::unique_ptr<FrameBuffer> FrameBufferObj;
static std::unique_ptr<RenderBuffer> RenderBufferObj;

static uint32_t m_RenderWidth = 0;
static uint32_t m_RenderHeight = 0;

void RenderPipeline::Init(Window& window) {
    m_Context = DeviceContext::Create();
    m_Context->Initialize(window);

    m_Context->CullFaces(true);
    m_Context->DepthTest(true);
    m_Context->Blend(true);

    m_Window = &window;

    // Create the Frame Buffer texture
    PostPorcessTextureObj = Texture::Create();
    PostPorcessTextureObj->ClampToEdge(true); // Avoid texture leak

    // Create and configure Frame Buffer
    FrameBufferObj = FrameBuffer::Create();
    FrameBufferObj->AttachTexture(PostPorcessTextureObj->GetID());
    FrameBufferObj->Bind();

    // Render Buffer (Depth Test etc...)
    RenderBufferObj = RenderBuffer::Create();
    RenderBufferObj->Attach();

    RenderBufferObj->Unbind();

    m_AmbientPass = std::make_unique<AmbientPass>(*m_Context);
    m_GeometryPass = std::make_unique<GeometryPass>(*m_Context);
    m_PostProcessPass = std::make_unique<PostProcessPass>(*m_Context);
}

void RenderPipeline::Update(flecs::world& world) {

    if (m_RenderWidth != m_Window->Width || m_RenderHeight != m_Window->Height) {
        m_RenderWidth = m_Window->Width;
        m_RenderHeight = m_Window->Height;

        PostPorcessTextureObj->SendData(nullptr, m_RenderWidth, m_RenderHeight);

        RenderBufferObj->SendData(m_RenderWidth, m_RenderHeight);
    }

    RenderFrame frame = BuildFrame(world);

    FrameBufferObj->Bind();

    m_Context->Clear({0.0f, 0.0f, 0.0f, 1.0f});
    m_Context->DepthTest(true);
    m_Context->CullFaces(true);

    m_AmbientPass->Execute(world, frame);
    m_GeometryPass->Execute(world, frame);

    FrameBufferObj->Unbind();
    m_Context->DepthTest(false);
    m_Context->CullFaces(false);
    PostPorcessTextureObj->Bind(0);
    m_PostProcessPass->Execute(world, frame);
}

void RenderPipeline::Shutdown() {
    m_AmbientPass.reset();
    m_GeometryPass.reset();
    m_PostProcessPass.reset();
    PostPorcessTextureObj.reset();
    FrameBufferObj.reset();
    RenderBufferObj.reset();

    m_Context.reset();
}

RenderFrame RenderPipeline::BuildFrame(flecs::world& world) {
    RenderFrame frame;

    float aspect = static_cast<float>(m_Window->Width) / static_cast<float>(m_Window->Height);

    // Camera
    world.each([&](flecs::entity e, TransformComponent& Transform, CameraComponent& Camera) {

        if (e.parent()) {
            const TransformComponent& ParentTransform = e.get<TransformComponent>();
            frame.Camera.Position = glm::vec3(Transform.GetLocalTransform()[3] * ParentTransform.GetLocalTransform()[3]);
        } else {
            frame.Camera.Position = glm::vec3(Transform.GetLocalTransform()[3]);
        }

        frame.Camera.View = glm::inverse(Transform.GetLocalTransform());

        switch (Camera.Type) {
            case Perspective:
                frame.Camera.Projection = glm::perspective(glm::radians(Camera.FOV), aspect, Camera.Near, Camera.Far);
                break;
            case Orthogonal:
                frame.Camera.Projection = glm::ortho(0.0f, static_cast<float>(m_Window->Width), 0.0f, static_cast<float>(m_Window->Height));
        }

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

        if (e.parent()) {
            const TransformComponent& ParentTransform = e.get<TransformComponent>();
            data.Position = glm::vec3(Transform.GetLocalTransform()[3] * ParentTransform.GetLocalTransform()[3]);
        } else {
            data.Position = glm::vec3(Transform.GetLocalTransform()[3]);
        }

        data.Color = Light.LightColor;
        data.Specular = Light.Specular;

        data.Constant = Light.Constant;
        data.Linear = Light.Linear;
        data.Quadratic = Light.Quadratic;

        frame.PointLights.push_back(data);
    });

    return frame;
}