#pragma once

#include "Renderer/Device/DeviceContext.h"

#include <memory>

struct Window;
class Scene;

struct RendererSystem {
    void Init(Window& window);
    void Update(Scene& scene);
    void Shutdown();

    void AmbientPass();
    void GeometryPass();
    private:
        std::unique_ptr<DeviceContext> m_Context;
        Scene* m_Scene = nullptr;
        Window* m_Window = nullptr;
};