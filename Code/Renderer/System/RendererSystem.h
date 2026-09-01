#pragma once

#include "Renderer/Device/DeviceContext.h"

#include <memory>

struct Window;
class Scene;

struct RendererSystem {
    void Init(Window& window);
    void Update(Scene& scene);
    void Shutdown();
    private:
        std::unique_ptr<DeviceContext> m_Context;
        Window* m_Window = nullptr;
};