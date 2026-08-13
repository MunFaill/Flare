#pragma once

#include "Renderer/Device/DeviceContext.h"

class GLContext : public DeviceContext {
    public:
        inline GLContext() {}
        inline ~GLContext() override {}

        void Initialize(Window& window) override;
        void DrawCall(uint32_t IndexCount) override;
        void DepthTest(bool State) override;
        void CullFaces(bool State) override;
        void Blend(bool State) override;
        void Clear(glm::vec4 Color) override;
};
