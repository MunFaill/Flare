#include "Core/Application.h"
#include "Platform/Windowing/WindowBackend.h"

#include <print>

void Application::Run() {
    std::println("Application initializing");

    WindowBackend::Init();
    
    Context.window = std::make_unique<Window>();
    Context.time = std::make_unique<Time>();
    Context.file = std::make_unique<File>();
    Context.assets = std::make_unique<Assets>();
    Context.input = std::make_unique<Input>();
    Context.assetProcessor = std::make_unique<AssetProcessor>();
    Context.scene = std::make_unique<Scene>();
    Context.renderer = std::make_unique<RendererSystem>();

    OnSetUp();

    Context.window->Init();
    Context.time->InitTime();
    Context.renderer->Init(*Context.window);

    OnStart();

    std::println("Application loop");
    // After-start setup
    while (Running) {
        if (Context.window->CloseEvent()) Running = false;
        Context.input->Update(Context.window->Handle);
        Context.time->CalculateTime();
        Context.renderer->Update(*Context.scene);

        OnUpdate();

        Context.window->SwapBuffers();
    }

    std::println("Application shtudown");
    OnStop();
    // Shutdown here

    Context.assets->Shaders.Clear();
    Context.assets->Textures.Clear();
    Context.assets->Meshes.Clear();

    Context.renderer->Shutdown();
    Context.window->Shutdown();
    WindowBackend::Shutdown();

    Context.window.reset();
    Context.time.reset();
    Context.file.reset();
    Context.assets.reset();
    Context.input.reset();
    Context.assetProcessor.reset();
    Context.scene.reset();
    Context.renderer.reset();
}