#pragma once

#include "Renderer/Passes/RendererPass.h"

class DeviceContext;

class PostProcessPass : public RenderPass {
public:
    explicit PostProcessPass(DeviceContext& context);

    void Execute(const flecs::world& World, const RenderFrame& frame) override;

private:
    DeviceContext& m_Context;
};