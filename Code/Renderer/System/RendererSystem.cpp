#include "Renderer/System/RendererSystem.h"
#include "Platform/IO/AssetSystem/Asset.h"
#include "Platform/Window/Window.h"
#include "Scene/Entity/Components.h"
#include "Scene/Scene.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"

#include <Renderer/Device/Device.h>

static Window internWindow;

void RendererSystem::Init(Window& window) {
    m_Context = DeviceContext::Create();
    m_Context->Initialize(window);
    m_Context->CullFaces(true);
    m_Context->DepthTest(true);
    m_Context->Blend(true);
    internWindow = window;
}

void RendererSystem::Update(Scene& scene) {
    m_Context->Clear({0.0f, 0.0f, 0.0f, 1.0f});

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    bool foundCamera = false;

    scene.Each<TransformComponent, CameraComponent>(
        [&](Entity entity, TransformComponent& transform, CameraComponent& camera) {
            if (!camera.Current) return;

            viewMatrix = glm::inverse(transform.GetTransform());
            float aspectRatio = (float)internWindow.Width / (float)internWindow.Height;
            projectionMatrix = glm::perspective(glm::radians(camera.Fov), aspectRatio, camera.Near, camera.Far);
        }
    );

    glm::mat4 viewProjection = projectionMatrix * viewMatrix;

    Assets::Shaders.Get("Base")->Bind();

    Assets::Shaders.Get("Base")->SetMat4("u_ViewProjection", viewProjection);

    scene.Each<MeshComponent, TransformComponent>(
        [this, &scene](Entity entity, MeshComponent& mesh, TransformComponent& transform)
        {
            Mesh* model = Assets::Meshes.Get(mesh.MeshID);

            if (!model)
                return;

            if (scene.HasComponent<MaterialComponent>(entity)) {
                MaterialComponent* mat = scene.GetComponent<MaterialComponent>(entity);
                Assets::Textures.Get(mat->TextureID)->Bind(0);
                Assets::Shaders.Get("Base")->SetVec4("mat.Albedo", mat->Albedo);
            } else {
                Assets::Textures.Get("Default")->Bind(0);
                Assets::Shaders.Get("Base")->SetVec4("mat.Albedo", glm::vec4(1.0f));
            }

            Assets::Shaders.Get("Base")->SetMat4("u_Model", transform.GetTransform());

            model->Bind();
            m_Context->DrawCall(model->GetIndexCount());
        }
    );
}

void RendererSystem::Shutdown() {
    m_Context.reset();
}
