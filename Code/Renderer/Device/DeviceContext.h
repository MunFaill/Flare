#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <memory>

enum class Devices {
    None = 0, OpenGL = 1, Vulkan = 2
};

inline uint32_t None = 0, DEPTH_LESS = 0x0201, DEPTH_LEQUAL = 0x0203, BLEND_SRC_ALPHA = 0x0302, BLEND_ONE_MINUS_SRC_ALPHA = 0x0303;

struct Window;

class DeviceContext {
    public:
        virtual ~DeviceContext() = default;

        virtual void Initialize(Window& window) = 0;
        virtual void DrawArrays(int x) = 0;
        virtual void DrawCall(uint32_t IndexCount) = 0;
        virtual void SetDepthFunc(uint32_t func) = 0;
        virtual void SetBlendFuncs(uint32_t func1, uint32_t func2) = 0;
        virtual void DepthTest(bool State) = 0;
        virtual void CullFaces(bool State) = 0;
        virtual void Blend(bool State) = 0;
        virtual void Clear(glm::vec4 Color) = 0;

        bool IsNone();
        bool IsOpenGL();
        bool IsVulkan();

        static std::unique_ptr<DeviceContext> Create();
};