#pragma once

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

    void Execute(Scene& scene, const RenderFrame& frame);

private:
    void RenderModel(Entity& entity, const RenderFrame& frame);
    void RenderMesh(Entity& entity, const RenderFrame& frame);
    void RenderMeshPart(Entity& entity, const RenderFrame& frame, Mesh& mesh, Material& material, Shader& shader);

    void SetupLights(Shader& shader, const RenderFrame& frame);
    void SetupMaterial(Shader& shader, const Material& material);

private:
    DeviceContext& m_Context;
};