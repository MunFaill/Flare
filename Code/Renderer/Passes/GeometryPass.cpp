#include "Renderer/Passes/GeometryPass.h"
#include "Renderer/Frames/RendererFrame.h"

#include "Platform/IO/AssetSystem/Assets.h"
#include "Renderer/Device/DeviceContext.h"
#include "Scene/Scene.h"
#include "Scene/Entities/Components.h"
#include "Scene/Entities/Entity.h"

#include <algorithm>
#include <string>

GeometryPass::GeometryPass(DeviceContext& context) : m_Context(context) {
}

void GeometryPass::Execute(Scene& scene, const RenderFrame& frame) {
    if (!frame.HasCamera)
        return;

    for (std::unique_ptr<Entity>& entity : scene.GetEntities()) {
        if (!entity->HasComponent<TransformComponent>())
            continue;

        if (entity->HasComponent<ModelComponent>()) {
            RenderModel(*entity, frame);
        }
        else if (entity->HasComponent<MeshComponent>()) {
            RenderMesh(*entity, frame);
        }
    }
}

void GeometryPass::RenderModel(Entity& entity, const RenderFrame& frame) {
    ModelComponent* modelComponent = entity.GetComponent<ModelComponent>();

    if (!modelComponent)
        return;

    Model* model = Assets::Models.Get(modelComponent->ModelID);

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

        RenderMeshPart(entity, frame, *mesh, *material, *shader);
    }
}

void GeometryPass::RenderMesh(Entity& entity, const RenderFrame& frame) {
    MeshComponent* meshComponent = entity.GetComponent<MeshComponent>();

    if (!meshComponent)
        return;

    Mesh* mesh = Assets::Meshes.Get(meshComponent->MeshID);

    if (!mesh)
        return;

    if (!entity.HasComponent<MaterialComponent>())
        return;

    MaterialComponent* materialComponent = entity.GetComponent<MaterialComponent>();

    if (!materialComponent)
        return;

    Material* material = Assets::Materials.Get(materialComponent->MaterialID);

    if (!material)
        return;

    Shader* shader = Assets::Shaders.Get(material->ShaderID);

    if (!shader)
        return;

    RenderMeshPart(entity, frame, *mesh, *material, *shader);
}

void GeometryPass::RenderMeshPart(Entity& entity, const RenderFrame& frame, Mesh& mesh, Material& material, Shader& shader) {
    shader.Bind();

    shader.SetMat4("u_Model", entity.GetWorldTransform());

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

    int count = static_cast<int>(
        std::min(
            frame.PointLights.size(),
            static_cast<size_t>(maxLights)
        )
    );

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

    shader.SetVec4(
        "material.Albedo",
        material.Albedo
    );

    if (Assets::Textures.Has(material.DiffuseTextureID)) {
        Assets::Textures.Get(material.DiffuseTextureID)->Bind(0);
    }

    if (Assets::Textures.Has(material.SpecularTextureID)) {
        Assets::Textures.Get(material.SpecularTextureID)->Bind(1);
    }
    else if (Assets::Textures.Has("Default")) {
        Assets::Textures.Get("Default")->Bind(1);
    }
}