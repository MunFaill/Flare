#include "app.h"

static flecs::entity camera;
static flecs::entity cube;
static flecs::entity ground;
static flecs::entity sun;
static flecs::entity ambient;

void App::OnSetup() {
    Modules->WindowModule->Title = "Sandbox";
}

void App::OnStart() {
    std::vector<std::string> FilesContaienr = Modules->FileSystemModule->ScanFolder("Sandbox/Assets"); // Scan for files (Assets like images, shaders and models)
    Modules->AssetProcessorModule->Process(FilesContaienr);

    Assets::Textures.Get("DefaultDiffuse")->Mipmaps(true);

    camera = World.entity("Camera");
    cube =  World.entity("Cube");
    ground = World.entity("Gorund");
    sun =  World.entity("Sun");
    ambient =  World.entity("Ambient");

    camera.set<TransformComponent>({{0.0f, -2.0f, 5.0f}}); // Customize parameters
    camera.add<CameraComponent>(); // Default parameters

    cube.set<TransformComponent>({{0.0f, 5.0f, 0.0f}, {45.0f, 45.0f, 45.0f}});
    cube.set<ModelComponent>({"Cube"});
    cube.set<BoxCollisionComponent>({DynamicBody});

    ground.set<TransformComponent>({{0.0f, -5.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {10.0f, 1.0f, 10.0f}});
    ground.set<ModelComponent>({"Cube"});
    ground.set<BoxCollisionComponent>({StaticBody});

    sun.set<TransformComponent>({{}, {-5.0f, -5.0f, -5.0f}});
    sun.add<DirectionalLightComponent>();

    ambient.set<AmbientComponent>({Sky, "SkyTexture"});
}

void App::OnUpdate(float delta)
{
    // Object with physics cannot move by changing it's position value yet
}

void App::OnShutdown() {

}
