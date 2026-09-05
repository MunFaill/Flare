#pragma once

#include "Renderer/Device/DeviceContext.h"

#include "Renderer/Passes/AmbientPass.h"
#include "Renderer/Passes/GeometryPass.h"

#include <memory>

struct Window;
class Scene;

class RendererSystem {
public:
    void Init(Window& window);
    void Update(Scene& scene);
    void Shutdown();

private:
    RenderFrame BuildFrame(Scene& scene);

private:
    std::unique_ptr<DeviceContext> m_Context;

    std::unique_ptr<AmbientPass> m_AmbientPass;
    std::unique_ptr<GeometryPass> m_GeometryPass;

    Window* m_Window = nullptr;
};