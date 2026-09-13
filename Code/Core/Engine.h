#pragma once

#include "IO/Windowing/Window.h"
#include "IO/InputSystem/Input.h"
#include "IO/AssetSystem/AssetProcessor.h"
#include "IO/FileSystem/File.h"

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
};