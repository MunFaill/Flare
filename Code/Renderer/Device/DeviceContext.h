#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <memory>

enum Devices {
    None = 0, OpenGL = 1, Vulkan = 2
};

struct Window;

class DeviceContext {
    public:
        virtual ~DeviceContext() = default;

        virtual void Initialize(Window& window) = 0;
        virtual void DrawCall(uint32_t IndexCount) = 0;
        virtual void DepthTest(bool State) = 0;
        virtual void CullFaces(bool State) = 0;
        virtual void Blend(bool State) = 0;
        virtual void Clear(glm::vec4 Color) = 0;

        bool IsNone();
        bool IsOpenGL();
        bool IsVulkan();

        static std::unique_ptr<DeviceContext> Create();
};