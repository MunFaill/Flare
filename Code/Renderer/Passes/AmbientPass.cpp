#include "Renderer/Passes/AmbientPass.h"
#include "Renderer/Frames/RendererFrame.h"

#include <glm/mat4x4.hpp>

#include "Platform/IO/AssetSystem/Assets.h"
#include "Renderer/Device/DeviceContext.h"
#include "Scene/Scene.h"
#include "Scene/Entities/Components.h"

AmbientPass::AmbientPass(DeviceContext& context) : m_Context(context) {
}

void AmbientPass::Execute(Scene& scene, const RenderFrame& frame) {
    if (!frame.HasCamera)
        return;

    for (auto& entity : scene.GetEntities()) {
        if (!entity->HasComponent<AmbientComponent>())
            continue;

        auto* ambient = entity->GetComponent<AmbientComponent>();

        Shader* shader = Assets::Shaders.Get(ambient->ShaderID);
        Texture* texture = Assets::Textures.Get(ambient->TextureID);

        if (!shader || !texture)
            continue;

        shader->Bind();

        shader->SetMat4("u_InverseProjection", glm::inverse(frame.Camera.Projection));

        glm::mat4 viewRotation =
            glm::mat4(glm::mat3(frame.Camera.View));

        shader->SetMat4("u_InverseView", glm::inverse(viewRotation));

        shader->SetFloat("u_Exposure", ambient->Exposure);

        shader->SetInt("u_SkyTexture", 0);

        texture->Bind(0);

        m_Context.SetDepthFunc(DEPTH_LEQUAL);

        m_Context.DrawArrays(3);

        m_Context.SetDepthFunc(DEPTH_LESS);
    }
}