#include "app.h"

#include <string>
#include <vector>

App::App() {
    WindowTilte = "Testing";
}

App::~App() {

}

void App::OnStart() {
    std::vector<std::string> assets_files = ScanFiles("Sandbox/Assets");
    AssetProcessor(assets_files);

    Entity Cube = Context.DefaultScene->CreateEntity();

    Context.DefaultScene->AddComponent<MeshComponent>(Cube).MeshID = "Cube";
}

void App::OnUpdate() {
    
}

void App::OnStop() {
    Assets::Meshes.Clear();
    Assets::Shaders.Clear();
    Assets::Textures.Clear();
}
