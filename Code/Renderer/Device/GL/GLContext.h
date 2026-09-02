#pragma once

#include "Renderer/Device/DeviceContext.h"

class GLContext : public DeviceContext {
    public:
        inline GLContext() {}
        inline ~GLContext() override {}

        void Initialize(Window& window) override;
        void DrawArrays(int x) override;
        void DrawCall(uint32_t IndexCount) override;
        void SetDepthFunc(uint32_t func) override;
        void SetBlendFuncs(uint32_t func1, uint32_t func2) override;
        void DepthTest(bool State) override;
        void CullFaces(bool State) override;
        void Blend(bool State) override;
        void Clear(glm::vec4 Color) override;
};
