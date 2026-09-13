#include "app.h"

static flecs::entity camera;
static flecs::entity cube;
static flecs::entity sun;
static flecs::entity ambient;

static float rotation = 0.0f;

void App::OnSetup() {
    Modules->WindowModule->Title = "Sandbox";
}

void App::OnStart() {
    std::vector<std::string> FilesContaienr = Modules->FileSystemModule->ScanFolder("Sandbox/Assets"); // Scan for files (Assets like images, shaders and models)
    Modules->AssetProcessorModule->Process(FilesContaienr);

    camera = World.entity("Camera");
    cube =  World.entity("Cube");
    sun =  World.entity("Sun");
    ambient =  World.entity("Ambient");

    camera.set<TransformComponent>({{0.0f, 0.0f, 5.0f}}); // Customize parameters
    camera.add<CameraComponent>(); // Default parameters

    cube.add<TransformComponent>();
    cube.set<ModelComponent>({"Cube"});

    sun.set<TransformComponent>({{}, {-5.0f, -5.0f, -5.0f}});
    sun.add<DirectionalLightComponent>();

    ambient.set<AmbientComponent>({Sky, "SkyTexture"});
}

void App::OnUpdate(float delta)
{
    rotation += 50.0f * delta;

    cube.set<TransformComponent>({{}, {rotation, rotation, rotation}});
}

void App::OnShutdown() {

}
