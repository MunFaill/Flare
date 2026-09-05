#include "app.h"
#include "Scene/Entities/Components.h"

// Probably insecure!
static Entity* camera;
static Entity* cube;
static Entity* sun;
static Entity* ambient;
static Entity* light;
static Entity* light2;
static Entity* light3;

void App::OnSetup() {
    Modules->WindowModule->Title = "Sandbox";
}

void App::OnStart() {
    std::vector<std::string> FilesContaienr = Modules->FileSystemModule->ScanFolder("Sandbox/Assets"); // Scan for files (Assets like images, shaders and models)
    Modules->AssetProcessorModule->Process(FilesContaienr);

    camera = Modules->SceneModule->CreateEntity("Camera");
    cube = Modules->SceneModule->CreateEntity("Cube");
    sun = Modules->SceneModule->CreateEntity("Sun");
    ambient = Modules->SceneModule->CreateEntity("Ambient");
    light = Modules->SceneModule->CreateEntity("PointLight");

    camera->AddComponent<TransformComponent>().Position.z = 5.0f; // Customize parameters
    camera->AddComponent<CameraComponent>(); // Default parameters

    cube->AddComponent<TransformComponent>();
    cube->AddComponent<MeshComponent>().MeshID = "Cube";

    sun->AddComponent<TransformComponent>().Rotation = {-5.0f, -5.0f, -5.0f};
    sun->AddComponent<DirectionalLightComponent>();

    ambient->AddComponent<AmbientComponent>().TextureID = "SkyTexture";

    light->AddComponent<TransformComponent>().Position = {1.0f, -1.0f, 2.0f};
    light->AddComponent<PointLightComponent>().LightColor = {1.0f, 0.0f, 0.0f};
}

void App::OnUpdate(float delta) {
    cube->GetComponent<TransformComponent>()->Rotation.x += 50.0f * delta;
    cube->GetComponent<TransformComponent>()->Rotation.y += 50.0f * delta;
    cube->GetComponent<TransformComponent>()->Rotation.z += 50.0f * delta;
}

void App::OnShutdown() {

}
