#include "app.h"
#include "Scene/Entity/Components.h"

#include <string>
#include <vector>

static Entity Camera;
static Entity Suzane;

App::App() {
    WindowTilte = "Testing";
}

App::~App() {

}

void App::OnStart() {
    std::vector<std::string> assets_files = ScanFiles("Sandbox/Assets");
    AssetProcessor(assets_files);

    Camera = Context.DefaultScene->CreateEntity();
    Suzane = Context.DefaultScene->CreateEntity();

    Context.DefaultScene->AddComponent<TransformComponent>(Camera).Position = {0.0f, 0.0f, 5.0f};
    Context.DefaultScene->AddComponent<CameraComponent>(Camera).Current = true;

    Context.DefaultScene->AddComponent<TransformComponent>(Suzane).Position = {0.0f, 0.0f, 0.0f};
    Context.DefaultScene->AddComponent<MeshComponent>(Suzane).MeshID = "Suzane";
}

float y = 0.0f;

void App::OnUpdate() {
    Context.DefaultScene->GetComponent<TransformComponent>(Suzane)->Rotation = {0.0f, y++, 0.0f};
}

void App::OnStop() {
    Assets::Meshes.Clear();
    Assets::Shaders.Clear();
    Assets::Textures.Clear();
}
