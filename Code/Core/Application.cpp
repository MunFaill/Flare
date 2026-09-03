#include "Core/Application.h"
#include "Math/Time.h"
#include "Platform/IO/AssetSystem/Assets.h"
#include "Platform/Windowing/WindowBackend.h"
#include "Renderer/System/RendererPipeline.h"

static Time _time;
static RendererSystem Pipeline;

void Application::Run() {
    // Setup and create the modules
    Modules = std::make_unique<EngineModules>();
    _time.Init();

    Modules->WindowModule = std::make_unique<Window>();
    Modules->InputModule = std::make_unique<Input>();
    Modules->AssetProcessorModule = std::make_unique<AssetProcessor>();
    Modules->FileSystemModule = std::make_unique<File>();
    Modules->SceneModule = std::make_unique<Scene>();

    OnSetup(); // OnSetup is called once after creation and before initialization
    // Initialize modules
    WindowBackend::Init();
    Modules->WindowModule->Init();
    Pipeline.Init(*Modules->WindowModule);

    OnStart(); // OnStart is called once after creation and initialization

    while (Running) { // Called every frame
        OnUpdate(_time.Delta);
        if (Modules->WindowModule->CloseEvent()) Running = false;
        _time.Update();
        Pipeline.Update(*Modules->SceneModule);

        Modules->InputModule->Update(*Modules->WindowModule);
        Modules->WindowModule->SwapBuffers();
    }

    OnShutdown();

    Assets::Textures.Clear();
    Assets::Shaders.Clear();
    Assets::Meshes.Clear();

    Modules->SceneModule.reset();
    Modules->InputModule.reset();
    Modules->AssetProcessorModule.reset();
    Modules->InputModule.reset();

    Pipeline.Shutdown();

    Modules->WindowModule->Shutdown();
    Modules->WindowModule.reset();

    WindowBackend::Shutdown();

    Modules.reset();
}