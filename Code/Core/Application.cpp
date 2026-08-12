#include "Core/Application.h"
#include "Platform/window/WindowBackend.h"

static Window InternDefaultWindow = {};

void Application::Run() {
    WindowBackend::Init();

    Context.DefaultWindow = &InternDefaultWindow;

    OnStart();

    Context.DefaultWindow->Init();

    while(Running) {

        if (Context.DefaultWindow->CloseEvent()) Running = false;

        OnUpdate();

        Context.DefaultWindow->Update();
    }

    OnStop();

    Context.DefaultWindow->Shutdown();
    
    WindowBackend::Shutdown();
}
