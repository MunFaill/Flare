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

    OnSetup(); // OnSetup is called once after creation and before initialization
    // Initialize modules
    WindowBackend::Init();
    Modules->WindowModule->Init();
    Modules->InputModule->Initialize(*Modules->WindowModule);
    Pipeline.Init(*Modules->WindowModule);

    OnStart(); // OnStart is called once after creation and initialization

    while (Running) { // Called every frame
        _time.Update();
        
        Modules->InputModule->Update();
        
        OnUpdate(_time.Delta);
        if (Modules->WindowModule->CloseEvent()) Running = false;

        Pipeline.Update(World);
        Modules->WindowModule->SwapBuffers();
    }

    OnShutdown();

    Pipeline.Shutdown();

    Assets::Textures.Clear();
    Assets::Shaders.Clear();
    Assets::Meshes.Clear();

    Modules->InputModule.reset();
    Modules->AssetProcessorModule.reset();

    Modules->WindowModule->Shutdown();
    Modules->WindowModule.reset();

    WindowBackend::Shutdown();

    Modules.reset();
}