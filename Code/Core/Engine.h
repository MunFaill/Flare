#pragma once

#include "Platform/Window/Window.h"
#include "Renderer/System/RendererSystem.h"
#include "Scene/Scene.h"
#include "Math/Time.h"

#define ENTRY_POINT(App)          \
int main(int argc, char **argv) { \
    App app;                      \
    app.Run();                    \
}                                 \

struct EngineContext {
    Scene* DefaultScene = nullptr;
    Window* DefaultWindow = nullptr;
    RendererSystem* DefaultRenderer = nullptr;
    Time* Delta = nullptr;
};
