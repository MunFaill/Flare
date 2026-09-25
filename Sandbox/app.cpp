#include "app.h"
#include "ECS/Components.h"

static flecs::entity camera;
static flecs::entity cube;
static flecs::entity ground;
static flecs::entity sun;
static flecs::entity ambient;
static flecs::entity postProcess; // Chromatic Aberration!

void App::OnSetup() {
    Modules->WindowModule->Title = "Sandbox";
}

void App::OnStart() {
    std::vector<std::string> FilesContaienr = Modules->FileSystemModule->ScanFolder("Sandbox/Assets"); // Scan for files (Assets like images, shaders and models)
    Modules->AssetProcessorModule->Process(FilesContaienr);

    Assets::Textures.Get("DefaultDiffuse")->Mipmaps(true);

    camera = World.entity("Camera");
    cube = World.entity("Cube");
    ground = World.entity("Gorund");
    sun = World.entity("Sun");
    ambient = World.entity("Ambient");
    postProcess = World.entity("PostProcess");

    camera.set<TransformComponent>({{0.0f, -2.0f, 5.0f}}); // Customize parameters
    camera.add<CameraComponent>(); // Default parameters

    cube.set<TransformComponent>({{0.0f, 5.0f, 0.0f}});
    cube.set<ModelComponent>({"Cube"});
    cube.set<CollisionComponent>({true, DynamicBody});

    ground.set<TransformComponent>({{0.0f, -5.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {10.0f, 1.0f, 10.0f}});
    ground.set<ModelComponent>({"Cube"});
    ground.set<CollisionComponent>({true, StaticBody});

    sun.set<TransformComponent>({{}, {-5.0f, -5.0f, -5.0f}});
    sun.add<DirectionalLightComponent>();

    ambient.set<AmbientComponent>({Sky, "SkyTexture"});

    postProcess.add<PostProcessComponent>();
}

void App::OnUpdate(float DeltaTime, float FramesPerSecond){
    if (Modules->InputModule->KeyJustPressed(KEY_SPACE) && Modules->PhysicsFunctionsModule->IsOnFloor(cube)) {
        cube.get_mut<CollisionComponent>().LinearVelocity.y += 7.0f; // add an impulse on Y axis
    }
}

void App::OnShutdown() {

}
