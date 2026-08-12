#pragma once

#include "Platform/window/Window.h"

#define ENTRY_POINT(App)          \
int main(int argc, char **argv) { \
    App app;                      \
    app.Run();                    \
}                                 \

struct EngineContext {
    Window* DefaultWindow = nullptr;
};
