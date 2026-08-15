#include "app.h"

static Entity camera;
static Entity cube;
static Entity light;

void Sandbox::OnSetUp() {
    // Called once after the creation of the systems and before their execution.
    Context.window->Title = "Sandbox";
}

void Sandbox::OnStart() {
    // Called once after the creation and execution of the systems.

    std::vector<std::string> FilesContainer = Context.file->Scan("Sandbox/Assets");
    Context.assetProcessor->Process(FilesContainer);

    // Create entities
    camera = Context.scene->CreateEntity();
    cube = Context.scene->CreateEntity();
    light = Context.scene->CreateEntity();

    // Add component to entities
    Context.scene->AddComponent<TransformComponent>(camera).Position = {0.0f, 0.0f, 5.0f}; // Set custom configs
    Context.scene->AddComponent<CameraComponent>(camera); // Use the default configs

    Context.scene->AddComponent<TransformComponent>(cube);
    Context.scene->AddComponent<MeshComponent>(cube).MeshID = "Cube";

    Context.scene->AddComponent<TransformComponent>(light).Rotation = {-0.2f, -1.0f, -0.3f};
    Context.scene->AddComponent<DirectionalLightComponent>(light).LightColor = {1.0f, 1.0f, 1.0f};
}

void Sandbox::OnUpdate() {
    // Update cube rotation every frame
    Context.scene->GetComponent<TransformComponent>(cube)->Rotation.x += 50 * Context.time->DeltaTime;
    Context.scene->GetComponent<TransformComponent>(cube)->Rotation.z += 50 * Context.time->DeltaTime;
}

void Sandbox::OnStop() {
    // Called once before the destruction of the systems
}
