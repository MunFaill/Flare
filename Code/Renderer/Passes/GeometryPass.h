#pragma once

#include "Renderer/Passes/RendererPass.h"

class DeviceContext;
class MaterialComponent;

class GeometryPass : public RenderPass {
public:
    explicit GeometryPass(DeviceContext& context);

    void Execute(Scene& scene, const RenderFrame& frame) override;

private:
    void SetupLights(Shader& shader, const RenderFrame& frame);

    void SetupMaterial(Shader& shader, const MaterialComponent& material);

private:
    DeviceContext& m_Context;
};