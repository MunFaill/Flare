#include "Renderer/Passes/GeometryPass.h"
#include "Renderer/Frames/RendererFrame.h"

#include "Platform/IO/AssetSystem/Assets.h"
#include "Renderer/Device/DeviceContext.h"
#include "ECS/Components.h"

#include <algorithm>
#include <string>

GeometryPass::GeometryPass(DeviceContext& context) : m_Context(context) {}

void GeometryPass::Execute(const flecs::world& world, const RenderFrame& frame) {
    if (!frame.HasCamera)
        return;

    world.each([&](flecs::entity e, TransformComponent& transform, ModelComponent& model) {
        RenderModel(e, frame);
    });

    world.each([&](flecs::entity e, TransformComponent& transform, MeshComponent& mesh) {
        RenderMesh(e, frame);
    });
}

void GeometryPass::RenderModel(flecs::entity& Entity, const RenderFrame& frame) {
    const ModelComponent& modelComponent = Entity.get<ModelComponent>();

    Model* model = Assets::Models.Get(modelComponent.ModelID);

    if (!model)
        return;

    for (const MeshPart& part : model->MeshParts) {
        Mesh* mesh = Assets::Meshes.Get(part.MeshID);
        Material* material = Assets::Materials.Get(part.MaterialID);

        if (!mesh || !material)
            continue;

        Shader* shader = Assets::Shaders.Get(material->ShaderID);

        if (!shader)
            continue;

        RenderMeshPart(Entity, frame, *mesh, *material, *shader);
    }
}

void GeometryPass::RenderMesh(flecs::entity& Entity, const RenderFrame& frame) {
    if (!Entity.has<MeshComponent>() || !Entity.has<MaterialComponent>())
        return;

    const MeshComponent& meshComponent = Entity.get<MeshComponent>();
    Mesh* mesh = Assets::Meshes.Get(meshComponent.MeshID);
    if (!mesh) return;

    const MaterialComponent& materialComponent = Entity.get<MaterialComponent>();
    Material* material = Assets::Materials.Get(materialComponent.MaterialID);
    if (!material) return;

    Shader* shader = Assets::Shaders.Get(material->ShaderID);
    if (!shader) return;

    RenderMeshPart(Entity, frame, *mesh, *material, *shader);

}

void GeometryPass::RenderMeshPart(flecs::entity& Entity, const RenderFrame& frame, Mesh& mesh, Material& material, Shader& shader) {
    shader.Bind();

    const TransformComponent& transform = Entity.get<TransformComponent>();
    shader.SetMat4("u_Model", transform.GetTransform());

    shader.SetMat4("u_ViewProjection", frame.Camera.ViewProjection);

    shader.SetVec3("viewPos", frame.Camera.Position);

    SetupLights(shader, frame);
    SetupMaterial(shader, material);

    mesh.Bind();

    m_Context.DrawIndices(mesh.GetIndexCount());
}

void GeometryPass::SetupLights(Shader& shader, const RenderFrame& frame) {
    if (frame.HasDirectionalLight) {
        const DirectionalLightData& light = frame.DirectionalLight;

        shader.SetVec3("dirlight.LightDirection", light.Direction);

        shader.SetVec3( "dirlight.LightColor",light.Color);

        shader.SetVec3("dirlight.Specular", light.Specular);
    }

    const int maxLights = 8;

    int count = static_cast<int>(std::min(frame.PointLights.size(), static_cast<size_t>(maxLights)));

    for (int i = 0; i < count; ++i) {
        const PointLightData& light = frame.PointLights[i];

        std::string prefix = "pointlights[" +std::to_string(i) +"].";

        shader.SetVec3(prefix + "LightPosition", light.Position);

        shader.SetVec3(prefix + "LightColor", light.Color);

        shader.SetVec3(prefix + "Specular", light.Specular);

        shader.SetFloat(prefix + "Constant", light.Constant);

        shader.SetFloat(prefix + "Linear", light.Linear);

        shader.SetFloat(prefix + "Quadratic", light.Quadratic);
    }

    shader.SetInt("u_NumPointLights", count);
}

void GeometryPass::SetupMaterial(Shader& shader, const Material& material) {
    shader.SetInt("material.Diffuse", 0);
    shader.SetInt("material.Specular", 1);

    shader.SetVec4("material.Albedo", material.Albedo);

    if (Assets::Textures.Has(material.DiffuseTextureID)) {
        Assets::Textures.Get(material.DiffuseTextureID)->Bind(0);
    } else if (Assets::Textures.Has("Default")) {
        Assets::Textures.Get("Default")->Bind(0);
    }

    if (Assets::Textures.Has(material.SpecularTextureID)) {
        Assets::Textures.Get(material.SpecularTextureID)->Bind(1);
    } else if (Assets::Textures.Has("Default")) {
        Assets::Textures.Get("Default")->Bind(1);
    }
}