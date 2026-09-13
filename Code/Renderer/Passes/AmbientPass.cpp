#include "Renderer/Passes/AmbientPass.h"
#include "Renderer/Frames/RendererFrame.h"
#include "IO/AssetSystem/Assets.h"
#include "Renderer/Device/DeviceContext.h"
#include "ECS/Components.h"

#include <glm/mat4x4.hpp>

AmbientPass::AmbientPass(DeviceContext& context) : m_Context(context) {
}

void AmbientPass::Execute(const flecs::world& World, const RenderFrame& frame) {
    if (!frame.HasCamera)
        return;

    World.each([&](flecs::entity e, AmbientComponent& Ambient){

        if (Ambient.Type == AmbientType::Sky) {
            Shader* shaderBase = Assets::Shaders.Get("Base");
            Shader* shader = Assets::Shaders.Get(Ambient.ShaderID);
            Texture* texture = Assets::Textures.Get(Ambient.TextureID);

            if (!shader || !texture || !shaderBase)
                return;

            shaderBase->Bind();
            shader->Bind();

            shaderBase->SetVec3("environment.AmbientColor", Ambient.AmbientColor);
            shader->SetMat4("u_InverseProjection", glm::inverse(frame.Camera.Projection));

            glm::mat4 viewRotation =
                glm::mat4(glm::mat3(frame.Camera.View));

            shader->SetMat4("u_InverseView", glm::inverse(viewRotation));

            shader->SetFloat("u_Exposure", Ambient.Exposure);

            shader->SetInt("u_SkyTexture", 0);

            texture->Bind(0);

            m_Context.SetBlendFuncLEqual();

            m_Context.DrawArrays(3);

            m_Context.SetBlendFuncLess();
        } else {
            Shader* shader = Assets::Shaders.Get("Base");

            if (!shader)
                return;

            shader->Bind();

            shader->SetVec3("environment.AmbientColor", Ambient.AmbientColor);
        };
    });
}