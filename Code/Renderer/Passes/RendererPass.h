#pragma once

#include <flecs.h>

struct RenderFrame;
class Scene;
class Shader;

class RenderPass {
public:
    virtual ~RenderPass() = default;

    virtual void Execute(const flecs::world& World, const RenderFrame& frame) = 0;
};