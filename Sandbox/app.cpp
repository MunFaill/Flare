#include "app.h"

void Sandbox::OnSetUp() {
    // Called once after the creation of the systems and before their execution.
    Context.window->Title = "Sandbox";
}

void Sandbox::OnStart() {
    // Called once after the creation and execution of the systems.

    std::vector<std::string> FilesContainer = Context.file->Scan("Sandbox/Assets");
    Context.assetProcessor->Process(FilesContainer);

    // Create entities
    Entity camera = Context.scene->CreateEntity();
    Entity cube = Context.scene->CreateEntity();
    Entity light = Context.scene->CreateEntity();

    // Add component to entities
    Context.scene->AddComponent<TransformComponent>(camera).Position = {0.0f, 0.0f, 5.0f}; // Set custom configs
    Context.scene->AddComponent<CameraComponent>(camera); // Use the default configs

    Context.scene->AddComponent<TransformComponent>(cube);
    Context.scene->AddComponent<MeshComponent>(cube).MeshID = "Cube";

    Context.scene->AddComponent<TransformComponent>(light).Position = {0.0f, 5.0f, 2.0f};
    Context.scene->AddComponent<LightComponent>(light).LightColor = {1.0f, 1.0f, 1.0f};
}

void Sandbox::OnUpdate() {
    // Called every frame
}

void Sandbox::OnStop() {
    // Called once before the destruction of the systems
}
