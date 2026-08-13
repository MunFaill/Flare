#include "Renderer/System/RendererSystem.h"
#include "Platform/IO/AssetSystem/Asset.h"
#include "Platform/Window/Window.h"
#include "Scene/Entity/Components.h"
#include "Scene/Scene.h"

#include <Renderer/Device/Device.h>

void RendererSystem::Init(Window& window) {
    m_Context = DeviceContext::Create();
    m_Context->Initialize(window);
    m_Context->CullFaces(false); // Change this later!!
    m_Context->DepthTest(false);
    m_Context->Blend(true);
}

void RendererSystem::Update(Scene& scene) {
    m_Context->Clear({0.0f, 0.0f, 0.0f, 1.0f});
    Assets::Shaders.Get("Base")->Bind();
    scene.Each<MeshComponent>(
        [this](Entity entity, MeshComponent& mesh)
        {
            Mesh* model = Assets::Meshes.Get(mesh.MeshID);

            if (!model)
                return;

            model->Bind();

            m_Context->DrawCall(
                model->GetIndexCount()
            );
        }
    );
}

void RendererSystem::Shutdown() {
    m_Context.reset();
}
