#include "Renderer/Passes/GeometryPass.h"
#include "Renderer/Frames/RendererFrame.h"

#include "Platform/IO/AssetSystem/Assets.h"
#include "Renderer/Device/DeviceContext.h"
#include "Scene/Scene.h"
#include "Scene/Entities/Components.h"
#include "Scene/Entities/Entity.h"

GeometryPass::GeometryPass(DeviceContext& context)
    : m_Context(context) {
}

void GeometryPass::Execute(Scene& scene, const RenderFrame& frame) {
    if (!frame.HasCamera)
        return;

    for (auto& entity : scene.GetEntities()) {
        if (!entity->HasComponent<TransformComponent>() ||
            !entity->HasComponent<MeshComponent>()) {
            continue;
        }

        auto* meshComponent =
            entity->GetComponent<MeshComponent>();

        Mesh* mesh =
            Assets::Meshes.Get(meshComponent->MeshID);

        MaterialComponent& material =
            meshComponent->Material;

        Shader* shader =
            Assets::Shaders.Get(material.ShaderID);

        if (!mesh || !shader)
            continue;

        shader->Bind();

        shader->SetMat4(
            "u_Model",
            entity->GetWorldTransform()
        );

        shader->SetMat4(
            "u_ViewProjection",
            frame.Camera.ViewProjection
        );

        shader->SetVec3(
            "viewPos",
            frame.Camera.Position
        );

        SetupLights(*shader, frame);
        SetupMaterial(*shader, material);

        mesh->Bind();

        m_Context.DrawCall(
            mesh->GetIndexCount()
        );
    }
}

void GeometryPass::SetupLights(Shader& shader, const RenderFrame& frame) {
    if (frame.HasDirectionalLight) {
        const auto& light = frame.DirectionalLight;

        shader.SetVec3(
            "dirlight.LightDirection",
            light.Direction
        );

        shader.SetVec3(
            "dirlight.LightColor",
            light.Color
        );

        shader.SetVec3(
            "dirlight.Diffuse",
            light.Diffuse
        );

        shader.SetVec3(
            "dirlight.Specular",
            light.Specular
        );
    }

    const int maxLights = 8;

    int count = static_cast<int>(
        std::min(
            frame.PointLights.size(),
            static_cast<size_t>(maxLights)
        )
    );

    for (int i = 0; i < count; ++i) {
        const auto& light = frame.PointLights[i];

        std::string prefix =
            "pointlights[" +
            std::to_string(i) +
            "].";

        shader.SetVec3(
            prefix + "LightPosition",
            light.Position
        );

        shader.SetVec3(
            prefix + "LightColor",
            light.Color
        );

        shader.SetVec3(
            prefix + "Diffuse",
            light.Diffuse
        );

        shader.SetVec3(
            prefix + "Specular",
            light.Specular
        );

        shader.SetFloat(
            prefix + "Constant",
            light.Constant
        );

        shader.SetFloat(
            prefix + "Linear",
            light.Linear
        );

        shader.SetFloat(
            prefix + "Quadratic",
            light.Quadratic
        );
    }

    shader.SetInt("u_NumPointLights", count);
}

void GeometryPass::SetupMaterial(Shader& shader, const MaterialComponent& material) {
    shader.SetInt("material.Diffuse", 0);
    shader.SetInt("material.Specular", 1);

    shader.SetFloat(
        "material.SpecularPower",
        material.SpecularPower
    );

    shader.SetVec4(
        "material.Albedo",
        material.Albedo
    );

    if (Assets::Textures.Has(material.DiffuseID)) {
        Assets::Textures
            .Get(material.DiffuseID)
            ->Bind(0);
    }

    if (Assets::Textures.Has(material.SpecularID)) {
        Assets::Textures
            .Get(material.SpecularID)
            ->Bind(1);
    }
    else if (Assets::Textures.Has("Default")) {
        Assets::Textures
            .Get("Default")
            ->Bind(1);
    }
}