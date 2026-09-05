#pragma once

struct RenderFrame;
class Scene;
class Shader;

class RenderPass {
public:
    virtual ~RenderPass() = default;

    virtual void Execute(Scene& scene, const RenderFrame& frame) = 0;
};