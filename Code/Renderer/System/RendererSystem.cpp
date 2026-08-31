#include "Renderer/System/RendererSystem.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>
#include <Renderer/Device/Device.h>

#include "Platform/IO/AssetSystem/Asset.h"
#include "Platform/Windowing/Window.h"
#include "Scene/Entity/Entity.h"
#include "Scene/Scene.h"

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

    Shader* baseShader = Assets::Shaders.Has("Base") ? Assets::Shaders.Get("Base") : nullptr;

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);

    scene.Each<TransformComponent, CameraComponent>(
        [&](Entity entity, TransformComponent& transform, CameraComponent& camera) {
            if (!camera.Current) return;

            viewMatrix = glm::inverse(transform.GetTransform());
            float aspectRatio = static_cast<float>(internWindow.Width) / static_cast<float>(internWindow.Height);
            projectionMatrix = glm::perspective(glm::radians(camera.Fov), aspectRatio, camera.Near, camera.Far);

            if (baseShader) {
                baseShader->SetVec3("viewPos", transform.Position);
            }
        }
    );

    glm::mat4 viewProjection = projectionMatrix * viewMatrix;

    if (baseShader) {
        baseShader->Bind();
        baseShader->SetMat4("u_ViewProjection", viewProjection);
    }

    scene.Each<AmbientComponent>(
        [&](Entity entity, AmbientComponent& ambient) {
            if (baseShader) {
                baseShader->SetVec4("environment.AmbientColor", ambient.AmbientColor);
            }
        }
    );

    scene.Each<DirectionalLightComponent, TransformComponent>(
        [&](Entity entity, DirectionalLightComponent& light, TransformComponent& transform) {
            if (baseShader) {
                baseShader->SetVec3("dirlight.LightDirection", transform.Rotation);
                baseShader->SetVec3("dirlight.LightColor", light.LightColor);
                baseShader->SetVec3("dirlight.Diffuse", light.Diffuse);
                baseShader->SetVec3("dirlight.Specular", light.Specular);
            }
        }
    );

    scene.Each<PointLightComponent, TransformComponent>(
        [&](Entity entity, PointLightComponent& light, TransformComponent& transform) {
            if (baseShader) {
                baseShader->SetVec3("pointlight.LightDirection", transform.Position);
                baseShader->SetVec3("pointlight.LightColor", light.LightColor);
                baseShader->SetVec3("pointlight.Diffuse", light.Diffuse);
                baseShader->SetVec3("pointlight.Specular", light.Specular);
                baseShader->SetFloat("pointlight.Constant", light.Constant);
                baseShader->SetFloat("pointlight.Linear", light.Linear);
                baseShader->SetFloat("pointlight.Quadratic", light.Quadratic);
            }
        }
    );

    scene.Each<MeshComponent, TransformComponent>(
        [this, &scene, baseShader](Entity entity, MeshComponent& mesh, TransformComponent& transform) {
            Mesh* model = Assets::Meshes.Get(mesh.MeshID);
            if (!model) return;

            if (scene.HasComponent<MaterialComponent>(entity)) {
                MaterialComponent* mat = scene.GetComponent<MaterialComponent>(entity);
                if (baseShader && Assets::Textures.Has(mat->DiffuseID)) {
                    baseShader->SetInt("material.Diffuse", 0);
                    baseShader->SetVec4("material.Albedo", mat->Albedo);
                    Assets::Textures.Get(mat->DiffuseID)->Bind(0);
                }
            } else {
                if (baseShader && Assets::Textures.Has("Default")) {
                    Assets::Textures.Get("Default")->Bind(0);
                    baseShader->SetInt("material.Diffuse", 0);
                    baseShader->SetVec4("material.Albedo", glm::vec4(1.0f));
                    baseShader->SetFloat("material.Specular", 0.5f);
                }
            }

            if (baseShader) {
                baseShader->SetMat4("u_Model", transform.GetTransform());
            }

            model->Bind();
            m_Context->DrawCall(model->GetIndexCount());
        }
    );
}

void RendererSystem::Shutdown() {
    m_Context.reset();
}