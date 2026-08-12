#include "app.h"

#include <memory>
#include <string>
#include <vector>

static std::unique_ptr<DeviceContext> DContext;

App::App() {
    WindowTilte = "Testing";
}

App::~App() {

}

void App::OnStart() {
    DContext = DeviceContext::Create();
    DContext->Initialize(*Context.DefaultWindow);
    std::vector<std::string> assets_files = ScanFiles("Sandbox/Assets");
    AssetProcessor(assets_files);
}

void App::OnUpdate() {
    Assets::Shaders.Get("Base")->Bind();
    Assets::Meshes.Get("Cube")->Bind();
    DContext->DrawCall(Assets::Meshes.Get("Cube")->GetIndexCount());
}

void App::OnStop() {
    Assets::Meshes.Clear();
    Assets::Shaders.Clear();
    Assets::Textures.Clear();
}
