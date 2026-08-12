#include "Core/Application.h"
#include "Platform/Window/WindowBackend.h"

static Window InternDefaultWindow = {};

void Application::Run() {
    WindowBackend::Init();

    Context.DefaultWindow = &InternDefaultWindow;

    if (WindowTilte != "") Context.DefaultWindow->Title = WindowTilte;
    if (WindowWidth != 0) Context.DefaultWindow->Width = WindowWidth;
    if (WIndowHeight != 0) Context.DefaultWindow->Height = WIndowHeight;

    Context.DefaultWindow->Init();

    OnStart();

    while(Running) {

        if (Context.DefaultWindow->CloseEvent()) Running = false;

        OnUpdate();

        Context.DefaultWindow->Update();
    }

    OnStop();

    Context.DefaultWindow->Shutdown();
    
    WindowBackend::Shutdown();
}
