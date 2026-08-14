#include "app.h"
#include "Scene/Entity/Components.h"

#include <string>
#include <vector>

static Entity Camera;
static Entity Cube;

App::App() {
    WindowTilte = "Testing";
}

App::~App() {

}

void App::OnStart() {
    std::vector<std::string> assets_files = ScanFiles("Sandbox/Assets");
    AssetProcessor(assets_files);

    Assets::Textures.Get("Default")->Mipmaps(true);

    Camera = Context.DefaultScene->CreateEntity();
    Cube = Context.DefaultScene->CreateEntity();

    Context.DefaultScene->AddComponent<TransformComponent>(Camera).Position = {0.0f, 0.0f, 5.0f};
    Context.DefaultScene->AddComponent<CameraComponent>(Camera).Current = true;

    Context.DefaultScene->AddComponent<TransformComponent>(Cube).Position = {0.0f, 0.0f, 0.0f};
    Context.DefaultScene->AddComponent<MeshComponent>(Cube).MeshID = "Cube";
}

float y = 0.0f;

void App::OnUpdate() {
    Context.DefaultScene->GetComponent<TransformComponent>(Cube)->Rotation = {0.0f, y++, 0.0f};
}

void App::OnStop() {
    Assets::Meshes.Clear();
    Assets::Shaders.Clear();
    Assets::Textures.Clear();
}
