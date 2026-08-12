#include "Core/Application.h"

void Application::Run() {
    OnStart();

    while(Running) {
        OnUpdate();
    }

    OnStop();
}
