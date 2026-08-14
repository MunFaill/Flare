#include "Core/Application.h"
#include "Platform/Window/WindowBackend.h"
#include "Scene/Scene.h"
#include <print>

static Window InternDefaultWindow = {};
static Scene InternDefaultScene = {};
static RendererSystem InternDefaultRendererSystem = {};

void Application::Run() {
    WindowBackend::Init();

    if (!Context.DefaultWindow) Context.DefaultWindow = &InternDefaultWindow;
    if (!Context.DefaultScene) Context.DefaultScene = &InternDefaultScene;
    if (!Context.DefaultRenderer) Context.DefaultRenderer = &InternDefaultRendererSystem;

    if (WindowTilte != "") Context.DefaultWindow->Title = WindowTilte;
    if (WindowWidth != 0) Context.DefaultWindow->Width = WindowWidth;
    if (WIndowHeight != 0) Context.DefaultWindow->Height = WIndowHeight;

    Context.DefaultWindow->Init();
    Context.DefaultRenderer->Init(*Context.DefaultWindow);

    std::println("Application initialized");

    OnStart();
    
    std::println("Application loop");
    while(Running) {

        if (Context.DefaultWindow && Context.DefaultWindow->CloseEvent()) Running = false;

        if (Context.DefaultScene) Context.DefaultRenderer->Update(*Context.DefaultScene);

        OnUpdate();

        if (Context.DefaultWindow) Context.DefaultWindow->Update(); // Swap buffers
    }

    OnStop();
    std::println("Application shutdown");

    if (Context.DefaultRenderer) Context.DefaultRenderer->Shutdown();
    if (Context.DefaultWindow) Context.DefaultWindow->Shutdown();
    
    WindowBackend::Shutdown();
}
