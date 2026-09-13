#pragma once

#include <flecs.h>

class DeviceContext;
class Entity;
class Material;
class Mesh;
class Shader;
class Scene;
class RenderFrame;

class GeometryPass {
    public:
        GeometryPass(DeviceContext& context);

        void Execute(const flecs::world& World, const RenderFrame& frame);

    private:
        void RenderModel(flecs::entity& entity, const RenderFrame& frame);
        void RenderMesh(flecs::entity& entity, const RenderFrame& frame);
        void RenderMeshPart(flecs::entity& entity, const RenderFrame& frame, Mesh& mesh, Material& material, Shader& shader);

        void SetupLights(Shader& shader, const RenderFrame& frame);
        void SetupMaterial(Shader& shader, const Material& material);

    private:
        DeviceContext& m_Context;
};