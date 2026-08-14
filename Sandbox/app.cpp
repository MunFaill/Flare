#include "app.h"
#include "Scene/Entity/Components.h"
#include "Scene/Entity/Entity.h"

#include <string>
#include <vector>

static Entity Camera;
static Entity Cube;
static Entity Light;

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
    Light = Context.DefaultScene->CreateEntity();

    Context.DefaultScene->AddComponent<TransformComponent>(Camera).Position = {0.0f, 0.0f, 5.0f};
    Context.DefaultScene->AddComponent<CameraComponent>(Camera).Current = true;

    Context.DefaultScene->AddComponent<TransformComponent>(Cube).Position = {-1.0f, 0.0f, 0.0f};
    Context.DefaultScene->AddComponent<MeshComponent>(Cube).MeshID = "Cube";

    Context.DefaultScene->AddComponent<TransformComponent>(Light).Position = {0.0f, 5.0f, 2.0f};
    Context.DefaultScene->AddComponent<LightComponent>(Light).LightColor = {1.0f, 0.0f, 1.0f};
}

void App::OnUpdate() {
    Context.DefaultScene->GetComponent<TransformComponent>(Cube)->Rotation.y += 50 * Context.Delta->DeltaTime;
    Context.DefaultScene->GetComponent<TransformComponent>(Cube)->Rotation.z += 50 * Context.Delta->DeltaTime;
}

void App::OnStop() {
    Assets::Meshes.Clear();
    Assets::Shaders.Clear();
    Assets::Textures.Clear();
}
