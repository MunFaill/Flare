#include "Renderer/System/RendererPipeline.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>
#include <Renderer/Device/Device.h>

#include "Platform/IO/AssetSystem/Assets.h"
#include "Platform/Windowing/Window.h"
#include "Scene/Entities/Components.h"
#include "Scene/Entities/Entity.h"
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
    m_Scene = &scene;
    m_Context->Clear({0.0f, 0.0f, 0.0f, 1.0f});

    AmbientPass();
    GeometryPass();
}

void RendererSystem::Shutdown() {
    m_Context.reset();
}

void RendererSystem::AmbientPass() {
    glm::mat4 ViewMatrix(1.0f);
    glm::mat4 ProjectionMatrix(1.0f);
    glm::vec3 CameraPosition(0.0f);

    for (auto& entity : m_Scene->GetEntities()) {
        if (!entity->HasComponent<CameraComponent>() || !entity->HasComponent<TransformComponent>()) continue;
        CameraComponent* Cam = entity->GetComponent<CameraComponent>();
        TransformComponent* CamTrans = entity->GetComponent<TransformComponent>();

        CameraPosition = CamTrans->Position;
        ViewMatrix = glm::inverse(CamTrans->GetTransform());

        float AspectRatio = static_cast<float>(m_Window->Width) / static_cast<float>(m_Window->Height);
        ProjectionMatrix = glm::perspective(glm::radians(Cam->FOV), AspectRatio, Cam->Near, Cam->Far);
    }

    glm::mat4 ViewProjection = ProjectionMatrix * ViewMatrix;
    if (Assets::Shaders.Has("Base")) {
        Assets::Shaders.Get("Base")->Bind();
        Assets::Shaders.Get("Base")->SetMat4("u_ViewProjection", ViewProjection);
        Assets::Shaders.Get("Base")->SetVec3("viewPos", CameraPosition);
    }

    for (auto& entity : m_Scene->GetEntities()) {
        if (!entity->HasComponent<AmbientComponent>()) continue;
        AmbientComponent* Ambient = entity->GetComponent<AmbientComponent>();
        Shader* SkyShader = Assets::Shaders.Get(Ambient->ShaderID);
        Texture* HDRITexture = Assets::Textures.Get(Ambient->TextureID);

        if (!SkyShader || !HDRITexture) continue;

        SkyShader->Bind();
        SkyShader->SetMat4("u_InverseProjection", glm::inverse(ProjectionMatrix));

        glm::mat4 viewRotOnly = glm::mat4(glm::mat3(ViewMatrix));

        SkyShader->SetMat4("u_InverseView", glm::inverse(viewRotOnly));
        SkyShader->SetFloat("u_Exposure", Ambient->Exposure);
        SkyShader->SetInt("u_SkyTexture", 0);

        HDRITexture->Bind(0);

        m_Context->SetDepthFunc(DEPTH_LEQUAL);
        m_Context->DrawArrays(3);
        m_Context->SetDepthFunc(DEPTH_LESS);
    }
}

void RendererSystem::GeometryPass() {
    for (auto& entity : m_Scene->GetEntities()) {
        if (!entity->HasComponent<TransformComponent>() || !entity->HasComponent<MeshComponent>()) continue;

        MeshComponent* _MeshComponent = entity->GetComponent<MeshComponent>();
        TransformComponent* Transform = entity->GetComponent<TransformComponent>();
        Mesh* _Mesh = Assets::Meshes.Get(entity->GetComponent<MeshComponent>()->MeshID);
        MaterialComponent* mat = &_MeshComponent->Material;
        Shader* _Shader = Assets::Shaders.Get(mat->ShaderID);

        if (!_Mesh || !_Shader) continue;

        _Shader->Bind();

        _Shader->SetMat4("u_Model", Transform->GetTransform());
        _Shader->SetVec4("environment.AmbientColor", glm::vec4(1.0f)); // TODO: Use color from Ambient Component

        for (auto& entity : m_Scene->GetEntities()) {
            if (!entity->HasComponent<TransformComponent>() || !entity->HasComponent<DirectionalLightComponent>()) continue;
            TransformComponent* LightTransform = entity->GetComponent<TransformComponent>();
            DirectionalLightComponent* DirLight = entity->GetComponent<DirectionalLightComponent>();

            _Shader->SetVec3("dirlight.LightDirection", LightTransform->Rotation);
            _Shader->SetVec3("dirlight.LightColor", DirLight->LightColor);
            _Shader->SetVec3("dirlight.Diffuse", DirLight->Diffuse);
            _Shader->SetVec3("dirlight.Specular", DirLight->Specular);
        }

        int PointLightCount = 0;
        for (auto& entity : m_Scene->GetEntities()) {
            if (!entity->HasComponent<TransformComponent>() || !entity->HasComponent<PointLightComponent>()) continue;
            TransformComponent* LightTransform = entity->GetComponent<TransformComponent>();
            PointLightComponent* PointLight = entity->GetComponent<PointLightComponent>();
             if (PointLightCount < 8) {
                std::string baseName = "pointlights[" + std::to_string(PointLightCount) + "].";
                _Shader->SetVec3(baseName + "LightPosition", LightTransform->Position);
                _Shader->SetVec3(baseName + "LightColor", PointLight->LightColor);
                _Shader->SetVec3(baseName + "Diffuse", PointLight->Diffuse);
                _Shader->SetVec3(baseName + "Specular", PointLight->Specular);
                _Shader->SetFloat(baseName + "Constant", PointLight->Constant);
                _Shader->SetFloat(baseName + "Linear", PointLight->Linear);
                _Shader->SetFloat(baseName + "Quadratic", PointLight->Quadratic);
                PointLightCount++;
            }
        }
        _Shader->SetInt("u_NumPointLights", PointLightCount);

        _Shader->SetInt("material.Diffuse", 0);
        _Shader->SetInt("material.Specular", 1);
        _Shader->SetFloat("material.SpecularPower", mat->SpecularPower);
        _Shader->SetVec4("material.Albedo", mat->Albedo);

        if (Assets::Textures.Has(mat->DiffuseID)) {
            Assets::Textures.Get(mat->DiffuseID)->Bind(0);
        }

        if (Assets::Textures.Has(mat->SpecularID)) {
            Assets::Textures.Get(mat->SpecularID)->Bind(1);
        } else if (Assets::Textures.Has("Default")) {
            Assets::Textures.Get("Default")->Bind(1);
        }

        _Mesh->Bind();

        m_Context->DrawCall(_Mesh->GetIndexCount());
    }
}