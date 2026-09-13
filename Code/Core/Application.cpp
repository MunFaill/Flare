#include "Core/Application.h"
#include "Math/Time.h"
#include "IO/AssetSystem/Assets.h"
#include "IO/Windowing/WindowBackend.h"
#include "Renderer/Pipeline/RendererPipeline.h"

static Time _time;
static RenderPipeline Pipeline;

void Application::Run() {
    // Setup and create the modules
    Setup();
    // Initialize modules
    Start();
    // Called every frame
    Update();
    // Shutdown modules
    Shutdown();
}

void Application::Setup() {
    Modules = std::make_unique<EngineModules>();
    _time.Init();

    Modules->WindowModule = Window::Create();
    Modules->InputModule = std::make_unique<Input>();
    Modules->AssetProcessorModule = std::make_unique<AssetProcessor>();
    Modules->FileSystemModule = std::make_unique<File>();

    OnSetup(); // OnSetup is called once after creation and before initialization
}

void Application::Start() {
    WindowBackend::Init();
    Modules->WindowModule->Init();
    Modules->InputModule->Initialize(*Modules->WindowModule);
    Pipeline.Init(*Modules->WindowModule);

    OnStart(); // OnStart is called once after creation and initialization
}

void Application::Update() {
    while (Running) {
        _time.Update();
        
        Modules->InputModule->Update();
        
        OnUpdate(_time.Delta);
        if (Modules->WindowModule->CloseEvent()) Running = false;

        Pipeline.Update(World);
        Modules->WindowModule->SwapBuffers();
    }
}

void Application::Shutdown() {
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