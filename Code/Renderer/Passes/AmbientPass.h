#pragma once

#include "Renderer/Passes/RendererPass.h"

class DeviceContext;

class AmbientPass : public RenderPass {
public:
    explicit AmbientPass(DeviceContext& context);

    void Execute(const flecs::world& World, const RenderFrame& frame) override;

private:
    DeviceContext& m_Context;
};