#include "Renderer/System/RendererSystem.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>
#include <Renderer/Device/Device.h>

#include "Platform/IO/AssetSystem/Asset.h"
#include "Platform/Windowing/Window.h"
#include "Scene/Entity/Entity.h"
#include "Scene/Scene.h"

void RendererSystem::Init(Window& window) {
    m_Context = DeviceContext::Create();
    m_Context->Initialize(window);
    m_Context->CullFaces(true);
    m_Context->DepthTest(true);
    m_Context->Blend(true);
    
    m_Window = &window;
}

void RendererSystem::Update(Scene& scene) {
    m_Context->Clear({0.0f, 0.0f, 0.0f, 1.0f});

    glm::mat4 viewMatrix(1.0f);
    glm::mat4 projectionMatrix(1.0f);
    glm::vec3 cameraPos(0.0f);

    scene.Each<TransformComponent, CameraComponent>(
        [&](Entity entity, TransformComponent& transform, CameraComponent& camera) {
            if (!camera.Current) return;

            cameraPos = transform.Position;
            viewMatrix = glm::inverse(transform.GetTransform());

            float aspectRatio = static_cast<float>(m_Window->Width) / static_cast<float>(m_Window->Height);
            projectionMatrix = glm::perspective(glm::radians(camera.Fov), aspectRatio, camera.Near, camera.Far);
        }
    );

    glm::mat4 viewProjection = projectionMatrix * viewMatrix;

    scene.Each<MeshComponent, TransformComponent>(
        [this, &scene, &viewProjection, &cameraPos](Entity entity, MeshComponent& mesh, TransformComponent& transform) {
            Mesh* model = Assets::Meshes.Get(mesh.MeshID);
            MaterialComponent* mat = &mesh.Material;
            Shader* shader = Assets::Shaders.Get(mat->ShaderID);

            if (!model || !shader) return;

            shader->Bind();

            shader->SetMat4("u_ViewProjection", viewProjection);
            shader->SetMat4("u_Model", transform.GetTransform());
            shader->SetVec3("viewPos", cameraPos);

            scene.Each<AmbientComponent>([&](Entity e, AmbientComponent& ambient) {
                shader->SetVec4("environment.AmbientColor", ambient.AmbientColor);
            });

            scene.Each<DirectionalLightComponent, TransformComponent>(
                [&](Entity e, DirectionalLightComponent& light, TransformComponent& lightTransform) {
                    shader->SetVec3("dirlight.LightDirection", lightTransform.Rotation);
                    shader->SetVec3("dirlight.LightColor", light.LightColor);
                    shader->SetVec3("dirlight.Diffuse", light.Diffuse);
                    shader->SetVec3("dirlight.Specular", light.Specular);
                }
            );

            int pointLightCount = 0;
            scene.Each<PointLightComponent, TransformComponent>(
                [&](Entity e, PointLightComponent& light, TransformComponent& lightTransform) {
                    if (pointLightCount < 8) {
                        std::string baseName = "pointlights[" + std::to_string(pointLightCount) + "].";
                        shader->SetVec3(baseName + "LightPosition", lightTransform.Position);
                        shader->SetVec3(baseName + "LightColor", light.LightColor);
                        shader->SetVec3(baseName + "Diffuse", light.Diffuse);
                        shader->SetVec3(baseName + "Specular", light.Specular);
                        shader->SetFloat(baseName + "Constant", light.Constant);
                        shader->SetFloat(baseName + "Linear", light.Linear);
                        shader->SetFloat(baseName + "Quadratic", light.Quadratic);
                        pointLightCount++;
                    }
                }
            );
            shader->SetInt("u_NumPointLights", pointLightCount);

            shader->SetInt("material.Diffuse", 0);
            shader->SetInt("material.Specular", 1);
            shader->SetFloat("material.SpecularPower", mat->SpecularPower);
            shader->SetVec4("material.Albedo", mat->Albedo);

            if (Assets::Textures.Has(mat->DiffuseID)) {
                Assets::Textures.Get(mat->DiffuseID)->Bind(0);
            }
            if (Assets::Textures.Has(mat->SpecularID)) {
                Assets::Textures.Get(mat->SpecularID)->Bind(1);
            } else if (Assets::Textures.Has("Default")) {
                Assets::Textures.Get("Default")->Bind(1);
            }

            model->Bind();
            m_Context->DrawCall(model->GetIndexCount());
        }
    );
}

void RendererSystem::Shutdown() {
    m_Context.reset();
}