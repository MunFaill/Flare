#include "Core/Application.h"
#include "Platform/Window/WindowBackend.h"
#include "Renderer/System/RendererSystem.h"
#include "Scene/Scene.h"

static Window InternDefaultWindow = {};
static Scene InternDefaultScene = {};
static RendererSystem InternRendererSystem = {};

void Application::Run() {
    WindowBackend::Init();

    Context.DefaultWindow = &InternDefaultWindow;
    Context.DefaultScene = &InternDefaultScene;

    if (WindowTilte != "") Context.DefaultWindow->Title = WindowTilte;
    if (WindowWidth != 0) Context.DefaultWindow->Width = WindowWidth;
    if (WIndowHeight != 0) Context.DefaultWindow->Height = WIndowHeight;

    Context.DefaultWindow->Init();

    InternRendererSystem.Init(*Context.DefaultWindow);

    OnStart();

    while(Running) {

        if (Context.DefaultWindow->CloseEvent()) Running = false;

        if (Context.DefaultScene) InternRendererSystem.Update(*Context.DefaultScene);

        OnUpdate();

        Context.DefaultWindow->Update();
    }

    OnStop();

    InternRendererSystem.Shutdown();
    Context.DefaultWindow->Shutdown();
    
    WindowBackend::Shutdown();
}
