#include "Renderer/Passes/PostProcessPass.h"
#include "Renderer/Frames/RendererFrame.h"
#include "IO/AssetSystem/Assets.h"
#include "Renderer/Device/DeviceContext.h"
#include "ECS/Components.h"

#include <glm/mat4x4.hpp>

PostProcessPass::PostProcessPass(DeviceContext& context) : m_Context(context) {}

void PostProcessPass::Execute(const flecs::world& World, const RenderFrame& frame) {
    if (!frame.HasCamera)
        return;

    World.each([&](flecs::entity e, PostProcessComponent& PostProcess){

        if (!PostProcess.ShaderID.empty()) {

            Shader* PostProcessShader = Assets::Shaders.Get(PostProcess.ShaderID);
            PostProcessShader->Bind();

            m_Context.SetDepthFuncLEqual();
            m_Context.DrawArrays(3);
            m_Context.SetDepthFuncLess();

        } else {
            return;
        }
    });
}