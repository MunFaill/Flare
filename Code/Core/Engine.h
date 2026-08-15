#pragma once

#include "Platform/Windowing/Window.h"
#include "Math/Time.h"
#include "Platform/IO/FileSystem/File.h"
#include "Platform/IO/AssetSystem/Asset.h"
#include "Scene/Scene.h"
#include "Renderer/System/RendererSystem.h"
#include <memory>

#define ENTRY_POINT(App)            \
int main(int argc, char** argv) {   \
    App app;                        \
    app.Run();                      \
}                                   \

struct EngineContext {
    std::unique_ptr<Window> window;
    std::unique_ptr<Time> time;
    std::unique_ptr<File> file;
    std::unique_ptr<Assets> assets;
    std::unique_ptr<AssetProcessor> assetProcessor;
    std::unique_ptr<Scene> scene;
    std::unique_ptr<RendererSystem> renderer;
};