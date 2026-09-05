#pragma once

#include "Renderer/Passes/RendererPass.h"

class DeviceContext;

class AmbientPass : public RenderPass {
public:
    explicit AmbientPass(DeviceContext& context);

    void Execute(Scene& scene, const RenderFrame& frame) override;

private:
    DeviceContext& m_Context;
};