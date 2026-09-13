#pragma once

#include "Renderer/Device/DeviceContext.h"

#include "Renderer/Passes/AmbientPass.h"
#include "Renderer/Passes/GeometryPass.h"

#include <memory>
#include <flecs.h>

struct Window;

class RenderPipeline {
public:
    void Init(Window& window);
    void Update(flecs::world& world);
    void Shutdown();

private:
    RenderFrame BuildFrame(flecs::world& world);

private:
    std::unique_ptr<DeviceContext> m_Context;

    std::unique_ptr<AmbientPass> m_AmbientPass;
    std::unique_ptr<GeometryPass> m_GeometryPass;

    Window* m_Window = nullptr;
};