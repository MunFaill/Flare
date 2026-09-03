#pragma once

#include "Platform/Windowing/Window.h"
#include "Platform/IO/InputSystem/Input.h"
#include "Platform/IO/AssetSystem/AssetProcessor.h"
#include "Platform/IO/FileSystem/File.h"
#include "Scene/Scene.h"

#include <memory>

#define ENTRY(class) \
int main() {         \
    class app;       \
    app.Run();       \
    return 0;        \
}                    \

struct EngineModules {
    std::unique_ptr<Window> WindowModule;
    std::unique_ptr<Input> InputModule;
    std::unique_ptr<AssetProcessor> AssetProcessorModule;
    std::unique_ptr<File> FileSystemModule;
    std::unique_ptr<Scene> SceneModule;
};