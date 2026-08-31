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

    int pointLightCount = 0;
    const int MAX_POINT_LIGHTS = 8;

    scene.Each<PointLightComponent, TransformComponent>(
        [&](Entity entity, PointLightComponent& light, TransformComponent& transform) {
            if (baseShader && pointLightCount < MAX_POINT_LIGHTS) {
                std::string baseName = "pointlights[" + std::to_string(pointLightCount) + "].";

                baseShader->SetVec3(baseName + "LightPosition", transform.Position);
                baseShader->SetVec3(baseName + "LightColor", light.LightColor);
                baseShader->SetVec3(baseName + "Diffuse", light.Diffuse);
                baseShader->SetVec3(baseName + "Specular", light.Specular);
                baseShader->SetFloat(baseName + "Constant", light.Constant);
                baseShader->SetFloat(baseName + "Linear", light.Linear);
                baseShader->SetFloat(baseName + "Quadratic", light.Quadratic);

                pointLightCount++;
            }
        }
    );

    if (baseShader) {
        baseShader->SetInt("u_NumPointLights", pointLightCount);
    }

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